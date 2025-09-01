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

#define SAMPLE_RATE 44100
#define DURATION 0.5

atomic_bool play_text_running = false;
pa_simple *sound_stream = nullptr;

const LetterSound* findLetter(char c) {
    c = (char)toupper(c);
    for (int i = 0; i < sizeof(alphabet)/sizeof(alphabet[0]); i++) {
        if (alphabet[i].letter == c)
            return &alphabet[i];
    }
    return nullptr;
}

void* playText(void* arg) {
    const char* text = arg;
    int error;
    const int totalSamples = (int)(strlen(text) * DURATION * SAMPLE_RATE);
    short *buffer = malloc(totalSamples * sizeof(short));
    int pos = 0;

    for (int i = 0; i < strlen(text); i++) {
        const LetterSound *ls = findLetter(text[i]);
        if (!ls) continue;

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

    pa_simple_write(sound_stream, buffer, totalSamples * sizeof(short), &error);
    free(buffer);
    play_text_running = false;
    return nullptr;
}

void drawWhites() {
    for (int i = 0; i <= 7; i++) {
        DrawRectangle(i * 60, 50, 60, 200, LIGHTGRAY);
        DrawRectangleLines(i * 60, 50, 60, 200, GRAY);
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

    InitWindow(420, 250, "Vocab2Note");
    SetTraceLogLevel(LOG_WARNING);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);

    pthread_t play_text_pthread;

    char text[50] = "";

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(WHITE);

        drawWhites();

        if (GuiTextBox((Rectangle){0, 0, 420, 50}, text, 50, true)) {
            bool expected = false;
            if (atomic_compare_exchange_strong(&play_text_running, &expected, true)) {
                pthread_create(&play_text_pthread, NULL, playText, text);
            }
        }

        EndDrawing();

    }

    CloseWindow();

    pa_simple_drain(sound_stream, &error);
    pa_simple_free(sound_stream);

    return 0;
}
