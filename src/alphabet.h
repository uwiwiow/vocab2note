#ifndef ALPHABET_H
#define ALPHABET_H

typedef enum {
    NOTE_C, NOTE_D, NOTE_E, NOTE_F,  NOTE_G, NOTE_A,  NOTE_B,
    NOTE_Cs, NOTE_Ds, NOTE_Fs, NOTE_Gs, NOTE_As,
    
    NOTE_Bb=NOTE_As, NOTE_Eb=NOTE_Ds, NOTE_Ab=NOTE_Gs, NOTE_Es=NOTE_F
} Note;

static constexpr double noteFrequencies[] = {
    [NOTE_C]  = 261.63,
    [NOTE_Cs] = 277.18,
    [NOTE_D]  = 293.66,
    [NOTE_Ds] = 311.13,
    [NOTE_E]  = 329.63,
    [NOTE_F]  = 349.23,
    [NOTE_Fs] = 369.99,
    [NOTE_G]  = 392.00,
    [NOTE_Gs] = 415.30,
    [NOTE_A]  = 440.00,
    [NOTE_As] = 466.16,
    [NOTE_B]  = 493.88,
};

static bool noteActive[] = {
    [NOTE_C]  = false,
    [NOTE_D]  = false,
    [NOTE_E]  = false,
    [NOTE_F]  = false,
    [NOTE_G]  = false,
    [NOTE_A]  = false,
    [NOTE_B]  = false,
    [NOTE_Cs] = false,
    [NOTE_Ds] = false,
    [NOTE_Fs] = false,
    [NOTE_Gs] = false,
    [NOTE_As] = false,
};

typedef struct {
    char letter;
    int notes[3];
    int noteCount;  // 1 = note, 3 = chord
} LetterSound;

static const LetterSound alphabet[] = {
    { 'A', { NOTE_C }, 1 },
    { 'E', { NOTE_D }, 1 },
    { 'I', { NOTE_E }, 1 },
    { 'O', { NOTE_G }, 1 },
    { 'U', { NOTE_A }, 1 },
    { '?', { NOTE_F }, 1 },
    { '!', { NOTE_B }, 1 },
    { '"', { NOTE_Cs }, 1 },
    { ' ', { NOTE_Ds }, 1 },
    { '.', { NOTE_Fs }, 1 },
    { ',', { NOTE_Gs }, 1 },
    { '/', { NOTE_As }, 1 },

    { 'B', { NOTE_C, NOTE_E, NOTE_G }, 3 },
    { 'C', { NOTE_D, NOTE_F, NOTE_A }, 3 },
    { 'D', { NOTE_D, NOTE_Fs, NOTE_A }, 3 },
    { 'F', { NOTE_F, NOTE_A, NOTE_C }, 3 },
    { 'G', { NOTE_G, NOTE_B, NOTE_D }, 3 },
    { 'H', { NOTE_A, NOTE_C, NOTE_E }, 3 },
    { 'J', { NOTE_B, NOTE_D, NOTE_F }, 3 },
    { 'K', { NOTE_E, NOTE_Gs, NOTE_B }, 3 },
    { 'L', { NOTE_A, NOTE_Cs, NOTE_E }, 3 },
    { 'M', { NOTE_E, NOTE_G, NOTE_B }, 3 },
    { 'N', { NOTE_G, NOTE_Bb, NOTE_D }, 3 },
    { 'Ñ', { NOTE_Fs, NOTE_A, NOTE_Cs }, 3 },
    { 'ñ', { NOTE_Fs, NOTE_A, NOTE_Cs }, 3 },
    { 'P', { NOTE_B, NOTE_D, NOTE_Fs }, 3 },
    { 'Q', { NOTE_Cs, NOTE_Es, NOTE_Gs }, 3 }, // E# ≈ F
    { 'R', { NOTE_F, NOTE_Ab, NOTE_C }, 3 },
    { 'S', { NOTE_Ds, NOTE_Fs, NOTE_As }, 3 },
    { 'T', { NOTE_Gs, NOTE_C, NOTE_Ds }, 3 },
    { 'V', { NOTE_As, NOTE_D, NOTE_F }, 3 },
    { 'W', { NOTE_C, NOTE_Eb, NOTE_G }, 3 },
    { 'X', { NOTE_E, NOTE_G, NOTE_Bb }, 3 },
    { 'Y', { NOTE_B, NOTE_Ds, NOTE_Fs }, 3 },
    { 'Z', { NOTE_Fs, NOTE_As, NOTE_Cs }, 3 }
};

#endif //ALPHABET_H
