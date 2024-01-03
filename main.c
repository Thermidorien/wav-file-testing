#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "math.h"

#define M_PI (3.14159265358979323846)

struct wav_header {

    // Look up https://docs.fileformat.com/audio/wav/ for more info on this

    char riff[4];               // "RIFF"
    int32_t file_length;        // file length in bytes
    char wave[4];               // ""WAVEfile length in bytes""
    char format[4];             // "fmt "
    int32_t chunk_size;         // FMT chunk size in bytes (usually 16)
    int16_t format_tag;         // 1=PCM, 257=Mu-Law, 258=A-Law, 259=ADPCM
    int16_t num_channels;       // 1=mono, 2=stereo
    int32_t sample_rate;        // Sampling rate in samples per second
    int32_t bytes_per_second;   // bytes per second = sample_rate * bytes_per_sample
    int16_t bytes_per_sample;   // 2=16-bit mono, 4=16-bit stereo
    int16_t bits_per_sample;    // number of bits per sample
    char data[4];               // "data"
    int32_t data_length;        // data length in bytes (filelength - 44) where 44 is the header length
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
    
    short int *buffer;
    // Allocate memory for buffer dynamically based on buffer_size
    buffer = malloc(buffer_size * sizeof(short int));
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        return 1; // Return an error code
    }


    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = (short int)(20000 * cos((2 * M_PI * 256.0 * i) / sample_rate));
    }
    
    FILE *fp = fopen("test.wav", "w");

    // Taking adress of first item of array / struct, then writing into fp the correct number of elements
    fwrite(&wavh, 1, sizeof(wavh), fp);
    fwrite(&(buffer[0]), 2, buffer_size, fp); // 2 bytes because item is of type short int

    // Freeing allocated memory
    free(buffer);

    return 0;
}