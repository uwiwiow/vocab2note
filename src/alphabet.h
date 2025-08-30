#ifndef ALPHABET_H
#define ALPHABET_H

typedef enum {
    NOTE_C, NOTE_Cs, NOTE_D, NOTE_Ds,
    NOTE_E, NOTE_F, NOTE_Fs, NOTE_G,
    NOTE_Gs, NOTE_A, NOTE_As, NOTE_B,
    NOTE_Bb, NOTE_Eb, NOTE_Ab, NOTE_Es
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
    [NOTE_Bb] = 466.16,
    [NOTE_Eb] = 311.13,
    [NOTE_Ab] = 415.30,
    [NOTE_Es] = 349.23
};


typedef enum { VOCAL, CONSONANT } LetterType;

typedef struct {
    char letter;
    LetterType type;
    int notes[3];
    int noteCount;  // 1 = note, 3 = chord
} LetterSound;

static const LetterSound alphabet[] = {
    { 'A', VOCAL, { NOTE_C }, 1 },
    { 'E', VOCAL, { NOTE_D }, 1 },
    { 'I', VOCAL, { NOTE_E }, 1 },
    { 'O', VOCAL, { NOTE_G }, 1 },
    { 'U', VOCAL, { NOTE_A }, 1 },

    { 'B', CONSONANT, { NOTE_C, NOTE_E, NOTE_G }, 3 },
    { 'C', CONSONANT, { NOTE_D, NOTE_F, NOTE_A }, 3 },
    { 'D', CONSONANT, { NOTE_D, NOTE_Fs, NOTE_A }, 3 },
    { 'F', CONSONANT, { NOTE_F, NOTE_A, NOTE_C }, 3 },
    { 'G', CONSONANT, { NOTE_G, NOTE_B, NOTE_D }, 3 },
    { 'H', CONSONANT, { NOTE_A, NOTE_C, NOTE_E }, 3 },
    { 'J', CONSONANT, { NOTE_B, NOTE_D, NOTE_F }, 3 },
    { 'K', CONSONANT, { NOTE_E, NOTE_Gs, NOTE_B }, 3 },
    { 'L', CONSONANT, { NOTE_A, NOTE_Cs, NOTE_E }, 3 },
    { 'M', CONSONANT, { NOTE_E, NOTE_G, NOTE_B }, 3 },
    { 'N', CONSONANT, { NOTE_G, NOTE_Bb, NOTE_D }, 3 },
    { ' ', CONSONANT, { NOTE_Fs, NOTE_A, NOTE_Cs }, 3 },
    { 'P', CONSONANT, { NOTE_B, NOTE_D, NOTE_Fs }, 3 },
    { 'Q', CONSONANT, { NOTE_Cs, NOTE_Es, NOTE_Gs }, 3 }, // E# ≈ F
    { 'R', CONSONANT, { NOTE_F, NOTE_Ab, NOTE_C }, 3 },
    { 'S', CONSONANT, { NOTE_Ds, NOTE_Fs, NOTE_As }, 3 },
    { 'T', CONSONANT, { NOTE_Gs, NOTE_C, NOTE_Ds }, 3 },
    { 'V', CONSONANT, { NOTE_As, NOTE_D, NOTE_F }, 3 },
    { 'W', CONSONANT, { NOTE_C, NOTE_Eb, NOTE_G }, 3 },
    { 'X', CONSONANT, { NOTE_E, NOTE_G, NOTE_Bb }, 3 },
    { 'Y', CONSONANT, { NOTE_B, NOTE_Ds, NOTE_Fs }, 3 },
    { 'Z', CONSONANT, { NOTE_Fs, NOTE_As, NOTE_Cs }, 3 }
};

#endif //ALPHABET_H
