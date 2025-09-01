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

const LetterSound* findLetter(char c) {
    c = (char)toupper(c);
    for (int i = 0; i < sizeof(alphabet)/sizeof(alphabet[0]); i++) {
        if (alphabet[i].letter == c)
            return &alphabet[i];
    }
    return nullptr;
}

typedef struct play_text_args{
    const char *text;
    pa_simple *s;
}play_text_args;

void* playText(void* arg) {
    const play_text_args *args = arg;
    if (arg == nullptr) return nullptr;
    int error;
    const int totalSamples = (int)(strlen(args->text) * DURATION * SAMPLE_RATE);
    short *buffer = malloc(totalSamples * sizeof(short));
    int pos = 0;

    for (int i = 0; i < strlen(args->text); i++) {
        const LetterSound *ls = findLetter(args->text[i]);
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

    pa_simple_write(args->s, buffer, totalSamples * sizeof(short), &error);
    free(buffer);
    play_text_running = false;
    return nullptr;
}

int main(const int argc, char *argv[]) {

    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = SAMPLE_RATE,
        .channels = 1
    };

    int error;
    pa_simple *s = pa_simple_new(nullptr, "Vocab2Note", PA_STREAM_PLAYBACK, nullptr, "playback", &ss, nullptr, nullptr, &error);
    if (!s) {
        fprintf(stderr, "pa_simple_new() failed: %s\n", pa_strerror(error));
        return 1;
    }

    InitWindow(400, 50, "Vocab2Note");
    SetTraceLogLevel(LOG_WARNING);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);

    pthread_t play_text_pthread;
    play_text_args *args = malloc(sizeof *args);

    char text[50] = "";

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(WHITE);

        if (GuiTextBox((Rectangle){0, 0, 400, 50}, text, 50, true)) {
            args->text = text;
            args->s = s;

            bool expected = false;
            if (atomic_compare_exchange_strong(&play_text_running, &expected, true)) {
                pthread_create(&play_text_pthread, NULL, playText, args);
            }
        }

        EndDrawing();

    }

    CloseWindow();

    free(args);

    pa_simple_drain(s, &error);
    pa_simple_free(s);

    return 0;
}
