#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include "alphabet.h"

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

void playLetter(const LetterSound *ls, pa_simple *s) {
    int error;
    const int samples = SAMPLE_RATE * DURATION;
    short *buffer = malloc(samples * sizeof(short));

    for (int i = 0; i < samples; i++) {
        const double t = (double)i / SAMPLE_RATE;
        double sample = 0.0;

        for (int n = 0; n < ls->noteCount; n++) {
            const double freq = noteFrequencies[ ls->notes[n] ];
            sample += sin(2 * M_PI * freq * t);
        }

        sample /= ls->noteCount;
        buffer[i] = (short)(sample * 32767 * 0.3);
    }

    pa_simple_write(s, buffer, samples * sizeof(short), &error);
    free(buffer);
}

int main(const int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s \"palabra\"\n", argv[0]);
        return 1;
    }

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

    char *text = argv[1];
    for (int i = 0; i < strlen(text); i++) {
        const LetterSound *ls = findLetter(text[i]);
        if (ls) {
            printf("Playing %c...\n", text[i]);
            playLetter(ls, s);
        }
    }

    pa_simple_drain(s, &error);
    pa_simple_free(s);

    return 0;
}
