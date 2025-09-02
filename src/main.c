#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include "alphabet.h"
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <pthread.h>
#include <stdatomic.h>
#include "wav.h"

#define SAMPLE_RATE 44100
#define DURATION 0.5

atomic_bool play_text_running = false;
bool play_tiles_running = false;
atomic_llong startTime = 0;
pa_simple *sound_stream = nullptr;

int findLetter(char c) {
    c = (char)toupper(c);
    for (int i = 0; i < sizeof(alphabet)/sizeof(alphabet[0]); i++) {
        if (alphabet[i].letter == c)
            return i;
    }
    return -1;
}


void saveAudio(const int* textIndex, char* text) {
    const int text_length = strlen(text);
    if (text_length < 1) return;
    const int totalSamples = (int)(text_length * DURATION * SAMPLE_RATE);
    short *buffer = malloc(totalSamples * sizeof(short));
    int pos = 0;

    for (int i = 0; i < text_length; i++) {
        const LetterSound *ls = &alphabet[textIndex[i]];

        for (int j = 0; j < SAMPLE_RATE * DURATION; j++, pos++) {
            const double t = (double)pos / SAMPLE_RATE;
            double sample = 0.0;

            for (int n = 0; n < ls->noteCount; n++) {
                const double freq = noteFrequencies[ls->notes[n]];
                sample += sin(2*M_PI*freq*t);

            }

            sample /= (ls->noteCount * 1.75);
            buffer[pos] = (short)(sample * 32767 * 0.3);
        }
    }

    saveWav(TextFormat("%s.wav",text), buffer, totalSamples);
    free(buffer);
}


long long nowMs(void) {
    return (long long)(GetTime() * 1000.0);
}

void* playText(void* arg) {
    const int* textIndex = arg;
    int error;
    int text_length = 0;
    for (int i = 0; i < 50; i++) {
        if (textIndex[i] == -1) {
            text_length = i;
            break;
        }
        text_length = 50;
    }
    const int totalSamples = (int)(text_length * DURATION * SAMPLE_RATE);
    short *buffer = malloc(totalSamples * sizeof(short));
    int pos = 0;

    for (int i = 0; i < text_length; i++) {
        const LetterSound *ls = &alphabet[textIndex[i]];

        for (int j = 0; j < SAMPLE_RATE * DURATION; j++, pos++) {
            const double t = (double)pos / SAMPLE_RATE;
            double sample = 0.0;

            for (int n = 0; n < ls->noteCount; n++) {
                const double freq = noteFrequencies[ls->notes[n]];
                sample += sin(2*M_PI*freq*t);

            }

            sample /= (ls->noteCount * 1.75);
            buffer[pos] = (short)(sample * 32767 * 0.3);
        }
    }

    long long localStart = nowMs();
    atomic_store(&startTime, localStart);
    play_tiles_running = true;
    pa_simple_write(sound_stream, buffer, totalSamples * sizeof(short), &error);
    free(buffer);
    play_text_running = false;
    return nullptr;
}

void drawWhites(int* textIndex) {
    if (play_tiles_running) {
        int text_length = 0;
        for (int i = 0; i < 50; i++) {
            if (textIndex[i] == -1) {
                text_length = i;
                break;
            }
            text_length = 50;
        }
        double t0 = atomic_load(&startTime);
        float t = GetTime() - (float) t0 / 1000.0;
        for (int j = 0; j < text_length; j++) {
            if (t >= j*DURATION && t < j*DURATION + DURATION) {
                noteActive[alphabet[textIndex[j]].notes[0]] = true;
                if (alphabet[textIndex[j]].noteCount == 1) break;
                noteActive[alphabet[textIndex[j]].notes[1]] = true;
                noteActive[alphabet[textIndex[j]].notes[2]] = true;
                break;
            }
            noteActive[alphabet[textIndex[j]].notes[0]] = false;
            if (alphabet[textIndex[j]].noteCount == 1) continue;
            noteActive[alphabet[textIndex[j]].notes[1]] = false;
            noteActive[alphabet[textIndex[j]].notes[2]] = false;
        }
    }

    for (int i = 0; i < 12; i++) {
        if (i < 7) {
            DrawRectangle(i * 60, 50, 60, 200, noteActive[i]? LIGHTGRAY: WHITE);
            DrawRectangleLines(i * 60, 50, 60, 200, DARKGRAY);
            continue;
        }
        DrawRectangle((( (i-6 > 2? i+1 : i) -6) * 60)-20, 50, 40, 120, noteActive[i]? DARKGRAY: BLACK);
        DrawRectangleLines((( (i-6 > 2? i+1 : i) -6) * 60)-20, 50, 40, 120, DARKGRAY);
    }

}

int main(const int argc, char *argv[]) {

    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = SAMPLE_RATE,
        .channels = 1
    };

    int error;
    sound_stream = pa_simple_new(nullptr, "Vocab2Note", PA_STREAM_PLAYBACK, nullptr, "playback", &ss, nullptr, nullptr, &error);
    if (!sound_stream) {
        fprintf(stderr, "pa_simple_new() failed: %s\n", pa_strerror(error));
        return 1;
    }

    SetTraceLogLevel(LOG_WARNING);
    InitWindow(420, 250, "Vocab2Note");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);

    pthread_t play_text_pthread;

    char text[50] = "";
    int textIndex[50] = {};

    while (!WindowShouldClose()) {

        if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_S)) {
            saveAudio(textIndex, text);
        }

        BeginDrawing();

        ClearBackground(WHITE);

        if (GuiTextBox((Rectangle){0, 0, 420, 50}, text, 50, true)) {
            for (int i = 0; i < strlen(text); i++)
                textIndex[i] = findLetter(text[i]);

            if (strlen(text) < 50)
                textIndex[strlen(text)] = -1;

            bool expected = false;
            if (atomic_compare_exchange_strong(&play_text_running, &expected, true)) {
                pthread_create(&play_text_pthread, nullptr, playText, textIndex);
            }
        }

        drawWhites(textIndex);

        DrawFPS(10, 10);

        EndDrawing();

    }

    CloseWindow();

    pa_simple_drain(sound_stream, &error);
    pa_simple_free(sound_stream);

    return 0;
}
