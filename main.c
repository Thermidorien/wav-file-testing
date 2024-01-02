#include "stdio.h";
#include "stdint.h";

struct wav_header {

    // Look up https://docs.fileformat.com/audio/wav/ for more info on this

    char riff[4];
    int32_t file_length;
    char wave[4];
    char format[4];
    int32_t chunk_size;
    int16_t format_tag;
    int16_t num_channels;
    int32_t sample_rate;
    int32_t bytes_per_second;
    int16_t bytes_per_sample;
    int16_t bits_per_sample;
    char data[4];
    int32_t data_length;
};

