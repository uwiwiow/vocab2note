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

#define SAMPLE_RATE 44100
#define DURATION 0.5

const LetterSound* findLetter(char c) {
    c = (char)toupper(c);
    for (int i = 0; i < sizeof(alphabet)/sizeof(alphabet[0]); i++) {
        if (alphabet[i].letter == c)
            return &alphabet[i];
    }
    return nullptr;
}

void playText(const char *text, pa_simple *s) {
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

    pa_simple_write(s, buffer, totalSamples * sizeof(short), &error);
    free(buffer);
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

    char text[50] = "";

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(WHITE);

        if (GuiTextBox((Rectangle){0, 0, 400, 50}, text, 50, true)) {
            playText(text, s);
        }

        EndDrawing();

    }

    CloseWindow();

    pa_simple_drain(s, &error);
    pa_simple_free(s);

    return 0;
}
