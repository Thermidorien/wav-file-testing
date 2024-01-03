#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "math.h"

#define M_PI (3.14159265358979323846)

struct soundhdr {
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

struct soundhdr wavh;


// frequencies can be found here: https://pages.mtu.edu/~suits/notefreqs.html
// chords used for In the Aeroplane Over the Sea

// G chord 1
const float G4 = 392.00;
const float B4 = 493.88;
const float D5 = 587.33;
const float G5 = 783.99;	

// Em chord 2
const float E4 = 329.63; 
const float E5 = 659.25;
// G4, already defined
// B4, already defined

// C chord 3
const float C5 = 523.25;
// E4, already defined
// G4, already defined
// E5, already defined

// D chord 4
const float Fs4 = 369.99;
const float A4 = 440.00;
const float Fs5 = 739.99;
// D5, already defined

const int num_measures_to_play = 4 * 4;
const int sample_rate = 16000;
const int beats_per_measure = 4;
const int ms_per_beat = 500;
// IF YOU MODIFY ABOVE< MODIFY BELOW
const int samples_per_beat = (16000 * 500) / 1000;    // samples_per_beat = (sample_rate * ms_per_beat) / 1000;, have to write manually because otherwise it won't compile (initialization of variables at file scope (outside any function) requires constant expressions)
const int header_length = 44;
const int buffer_size = (4*4) * (4) * ((16000*500)/1000); // num_measures_to_play * beats_per_measure * samples_per_beat;, save reason as above
short int *buffer;


// Function defining the action of playing a single note
void play(float freq, float duration, int measure, float beat) 
{
    // freq = frequency, duration = total number of beats
    // measure = measure we are on, beat = beat we are on in the measure
    float current_beat = beats_per_measure * measure + beat;
    int start_index = current_beat * samples_per_beat;
    int end_index = start_index + duration * samples_per_beat;


    // Defining amplitude according to approximated sound plot found here https://www.teachmeaudio.com/application/files/7615/8662/7043/adsr.png
    // Though we set it up so that there is no sustain phase
    /*for (int i = start_index; i < end_index; i++) 
    {
        float percent_through_note = ((float)i - start_index) / (end_index - start_index);
        float amplitude_multiplier = 1.0f;  // f to indicate that it is a single-precision floating-point constant

        if (percent_through_note < 0.25)        // Attack
        {
            amplitude_multiplier = (percent_through_note / 0.25);
        }
        else if (percent_through_note < 0.5f)   // Decay
        {
            amplitude_multiplier = (1 - ((percent_through_note - 0.25) / 0.25)) * 0.8 + 0.2;
        }
        else                                    // Release
        {
            amplitude_multiplier = 1 - percent_through_note;
        }
        
        buffer[i] += (short int)(amplitude_multiplier * 50* cos((2 * M_PI * freq * i) / sample_rate));
    }*/

    ///////////////////////////////////////////// TEMPORARY, BELOW TO BE REMOVED
    for (int i = start_index; i < end_index; i++) 
    {
        buffer[i] = (short int)(50 * cos((2 * M_PI * freq * i) / sample_rate));
    }
    ///////////////////////////////////////////// TEMPORARY, ABOVE TO BE REMOVED

}

// chord functions
// freq = frequency, duration = total number of beats
// measure = measure we are on, beat = beat we are on in the measure
// G chord 1,  G4, B4, D5, G5
void GMajor (float duration, int measure, float beat) 
{
    play(G4, duration, measure, beat);
    play(B4, duration, measure, beat);
    play(D5, duration, measure, beat);
    play(G5, duration, measure, beat);
    
}

// Em chord 2, E4, G4, B4, E5
void EMinor (float duration, int measure, float beat) 
{
    play(E4, duration, measure, beat);
    play(G4, duration, measure, beat);
    play(B4, duration, measure, beat);
    play(E5, duration, measure, beat);
    
}

// C chord 3,  E4, G4, C5, E5
void CMajor (float duration, int measure, float beat) 
{
    play(E4, duration, measure, beat);
    play(G4, duration, measure, beat);
    play(C5, duration, measure, beat);
    play(E5, duration, measure, beat);
    
}

// D chord 4, Fs4, A4, Fs5, D5
void DMajor (float duration, int measure, float beat) 
{
    play(Fs4, duration, measure, beat);
    play(A4, duration, measure, beat);
    play(Fs5, duration, measure, beat);
    play(D5, duration, measure, beat);
    
}

int main() 
{
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
    wavh.data_length = buffer_size * wavh.bytes_per_sample;
    wavh.file_length = wavh.data_length + header_length;


    // Allocate memory for buffer dynamically based on buffer_size
    buffer = malloc(buffer_size * sizeof(short int));
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        return 1; // Return an error code
    }
    
    int measure = 0;

    while (measure < num_measures_to_play)
    {
        play(G4, 4, measure, 0);
        /*GMajor(6, measure, 0);
        GMajor(1, measure, 3);
        GMajor(1, measure, 4);
        GMajor(1, measure, 5);
        GMajor(6, measure, 6);
        GMajor(1, measure, 9);
        GMajor(1, measure, 10);
        GMajor(1, measure, 11);*/

        measure++;
        play(E4, 4, measure, 0);
        /*EMinor(6, measure, 0);
        EMinor(1, measure, 3);
        EMinor(1, measure, 4);
        EMinor(1, measure, 5);
        EMinor(6, measure, 6);
        EMinor(1, measure, 9);
        EMinor(1, measure, 10);
        EMinor(1, measure, 11);*/
        
        measure++;
        play(E4, 4, measure, 0);
        /*CMajor(6, measure, 0);
        CMajor(1, measure, 3);
        CMajor(1, measure, 4);
        CMajor(1, measure, 5);
        CMajor(6, measure, 6);
        CMajor(1, measure, 9);
        CMajor(1, measure, 10);
        CMajor(1, measure, 11);*/
        
        measure++;
        play(Fs4, 4, measure, 0);
        /*DMajor(6, measure, 0);
        DMajor(1, measure, 3);
        DMajor(1, measure, 4);
        DMajor(1, measure, 5);
        DMajor(6, measure, 6);
        DMajor(1, measure, 9);
        DMajor(1, measure, 10);
        DMajor(1, measure, 11);*/
        
        measure++;
    }
    
    FILE *fp = fopen("test2.wav", "w");

    // Taking adress of first item of array / struct, then writing into fp the correct number of elements
    fwrite(&wavh, 1, sizeof(wavh), fp);
    fwrite(&(buffer[0]), 2, buffer_size, fp); // 2 bytes because item is of type short int

    free(buffer); 
    return 0;
}