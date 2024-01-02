#include "stdio.h";
#include "stdint.h";
#include "string.h";
#include "math.h";

#define M_PI (3.14159265358979323846)

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

int main() {
    const int sample_rate = 8000;



    struct wav_header wavh;

    // Look up https://docs.fileformat.com/audio/wav/ for more info on this
    strncpy(wavh.riff,"RIFF",4);
    strncpy(wavh.wave,"WAVE",4);
    strncpy(wavh.format,"fmt ",4);
    strncpy(wavh.data,"data",4);

    wavh.chunk_size = 16;
    wavh.format_tag = 1;
    wavh.num_channels = 1;
    wavh.sample_rate = sample_rate;
    wavh.bits_per_sample = 16;
    wavh.bytes_per_sample = (wavh.bits_per_sample / 8) * wavh.num_channels;
    wavh.bytes_per_second = wavh.sample_rate * wavh.bytes_per_sample;

    const int duration_seconds = 10;
    const int buffer_size = sample_rate * duration_seconds;
    const int header_length = 44;

    wavh.data_length = buffer_size * wavh.bytes_per_sample;
    wavh.file_length = wavh.data_length + header_length;
 
   
    return 0;
}