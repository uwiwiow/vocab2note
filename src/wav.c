#include "wav.h"


void saveWav(const char *filename, const short *buffer, const int totalSamples) {
    FILE *f = fopen(filename, "wb");
    if (!f) { perror("fopen"); return; }

    const RIFFHeader riff = { "RIFF", 36 + totalSamples*2, "WAVE" };
    fwrite(&riff, sizeof(riff), 1, f);

    const FmtSubchunk fmt = { "fmt ", 16, 1, 1, 44100, 44100*2, 2, 16 };
    fwrite(&fmt, sizeof(fmt), 1, f);

    const DataSubchunk data = { "data", totalSamples*2 };
    fwrite(&data, sizeof(data), 1, f);

    fwrite(buffer, sizeof(short), totalSamples, f);
    fclose(f);
}
