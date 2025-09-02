#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    char riff[4];        // "RIFF"
    uint32_t size;       // file size - 8
    char wave[4];        // "WAVE"
} RIFFHeader;

typedef struct {
    char fmt[4];         // "fmt "
    uint32_t fmtSize;    // 16
    uint16_t audioFormat;// 1 = PCM
    uint16_t channels;   // 1 = mono
    uint32_t sampleRate; // 44100
    uint32_t byteRate;   // sampleRate * channels * bitsPerSample/8
    uint16_t blockAlign; // channels * bitsPerSample/8
    uint16_t bitsPerSample; // 16
} FmtSubchunk;

typedef struct {
    char data[4];
    uint32_t dataSize;
} DataSubchunk;

void saveWav(const char *filename, const short *buffer, const int totalSamples);

#endif //AUDIO_H
