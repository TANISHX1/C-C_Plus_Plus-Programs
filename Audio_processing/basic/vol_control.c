// Volume / Gain control 
// This program reads a .WAV file and gain control on the volumne

/*
Concepts:
    Reading -writing binary audio data
    sample-by-sample processing

Gain Control:
Gain <1 -- Decrease
Gain =1 -- Original
Gain >1 -- increase (may cause Clipping , read more)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../decor.h"

#define MAX_SAMPLES 10000000

typedef struct {
    // RIFF 
    uint8_t riff[4];
    uint32_t file_size;
    uint8_t wave[4];
    }riff_header_t;

typedef struct
    {
    uint8_t chunkid[4];
    uint32_t chunk_size;
    }chunk_header_t;


typedef struct {
    // FORMAT HEADERS
    uint16_t audio_format;
    uint16_t audio_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    }fmt_chunk_t;

typedef struct
    {
    riff_header_t riff;
    fmt_chunk_t fmt;
    uint32_t data_size;
    uint32_t bytes_to_read;
    uint32_t data_offset;
    }wav_header_t;

typedef struct 
{
    char fmt_chunk_id[4];
    int fmt_chunk_size;
    char data_chunk_id[4];
    } __chunks;

__chunks essential_chunks;
void clamp_check(float* sample) {
    // Clamp to prevent clipping 
    if (*sample > 32767.0f) *sample = 32767.0f;
    if (*sample < -32768.0f) *sample = -32768.0f;
    }

// function to read entire .wav file in memory
int8_t* read_wav_data(const char* filename, wav_header_t** header) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("[%sError%s] : File cannot open or not exist '%s'\n", FG_RED, RESET, filename);
        return NULL;
        }

    *header = (wav_header_t*)calloc(1, sizeof(wav_header_t));
    if (!*header) {
        puts("[Error] |Calloc| : Memory Allocation Failed");
        return NULL;
        }
    // Read RIFF HEADER
    if (fread(&((*header)->riff), 1, sizeof(riff_header_t), file) < sizeof(riff_header_t)) {
        puts("[Error] : Failed to read RIFF header\n");
        free(header);
        fclose(file);
        return NULL;
        }
    if (strncmp((char*)(*header)->riff.riff, "RIFF", 4) != 0 || strncmp((char*)(*header)->riff.wave, "WAVE", 4) != 0) {
        puts("[Error] : Not a valid WAV file\n");
        free(*header);
        fclose(file);
        return NULL;
        }

    // Iterate through chunks
    chunk_header_t chunk;
    int fmt_found = 0;
    int data_found = 0;

    // loop to skip unwanted subchunks
    while (fread(&chunk, 1, sizeof(chunk_header_t), file) == sizeof(chunk_header_t)) {

        // Check for fformat chunk (fmt)
        if (strncmp((char*)chunk.chunkid, "fmt ", 4) == 0) {
            uint32_t size_to_read = (chunk.chunk_size < sizeof(fmt_chunk_t)) ? chunk.chunk_size : sizeof(fmt_chunk_t);
            if (fread(&(*header)->fmt, 1, size_to_read, file) < size_to_read) {
                puts("[Error] : Failed to read fmt chunk\n");
                break;
                }

            // skip any extra bytes in fmt chunk 
            if (chunk.chunk_size > sizeof(fmt_chunk_t)) {
                fseek(file, chunk.chunk_size - sizeof(fmt_chunk_t), SEEK_CUR);
                }
            fmt_found = 1;
            memcpy(essential_chunks.fmt_chunk_id, "fmt ",4);
            essential_chunks.fmt_chunk_size = size_to_read;

            }

        else if (strncmp((char*)chunk.chunkid, "data", 4) == 0) {
            (*header)->data_size = chunk.chunk_size;
            (*header)->data_offset = ftell(file);
            data_found = 1;
            memcpy(essential_chunks.data_chunk_id, "data",4);
            break;
            }

        else {
            fseek(file, chunk.chunk_size, SEEK_CUR);
            // means skip N bytes (chunk.chunk_size) from current courser pos
            }

        }
    if (!fmt_found || !data_found) {
        printf("[Error] : Missing critical chunks (fmt: %s, data: %s)\n", fmt_found ? "found" : "MISSING", data_found ? "found" : "MISSING");
        free(*header);
        return NULL;
        }

    // Cal. number of samples
    size_t num_sample = (*header)->data_size / (*header)->fmt.block_align;

    // limit to MAX_SAMPLES
    if (num_sample > MAX_SAMPLES) {
        printf("%sWarning :File is too large, exciding the Sample { MAX_SAMPLE : %d} limit%s\n", FG_YELLOW, MAX_SAMPLES, RESET);
        num_sample = MAX_SAMPLES;
        }

    // Allocate memory for audio data
    /*  block algin will give the size of snapshort / size of the sample (including all channels)
        multiply total samples with block_align to get the no of bytes to read
        */
    (*header)->bytes_to_read = num_sample * (*header)->fmt.block_align;
    /*why used int8_t and why not caused errors?
        malloc(bytes_to_read) reserves a contiguous block of bytes.
        Casting it to int8_t* tells the compiler to treat this block as an array of individual 8-bit bytes,
        which is perfect for binary data or raw data manipulation. */
    int8_t* data = (int8_t*)malloc((*header)->bytes_to_read);
    if (!data) {
        printf("[%sError%s] : Cannot allocate the memory to audio data \n", FG_RED, RESET);
        fclose(file);
        return NULL;
        }

    // Read audio data
    fseek(file, (*header)->data_offset, SEEK_SET);
    size_t bytes_read = fread(data, 1, (*header)->bytes_to_read, file);
    if (bytes_read < (*header)->bytes_to_read) {
        puts("[Error] | Data Reading | Failed to Read Payload\n");
        return NULL;
        }

    fclose(file);

    return data;
    }




// Write WAV file with modification data

int write_wav_file(const char* filename, wav_header_t** header, int8_t* data, uint32_t data_size) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("[%sError%s] ; cannot able to open the file '%s'\n", FG_RED, RESET, filename);
        return -1;
        }

    // Update the headers
    (*header)->riff.file_size = 36 + (*header)->bytes_to_read;
    (*header)->data_size = data_size;

    // writing RIFF HEADER 
    fwrite(&(*header)->riff, 1, sizeof(riff_header_t), file);

    // write fmt chunk 
    fwrite(essential_chunks.fmt_chunk_id, 1, 4, file);
    fwrite(&essential_chunks.fmt_chunk_size, 1, 4, file);
    fwrite(&(*header)->fmt, 1, sizeof(fmt_chunk_t), file);

    // write data chunk 
    fwrite(essential_chunks.data_chunk_id, 1, 4, file);
    fwrite(&data_size, 1, 4, file);
    fwrite(data, 1, data_size, file);

    fclose(file);
    return 0;
    }

// 16-bit stereo audio
void apply_gain_stereo(int16_t* samples, size_t num_frames, float gain) {
    // multiplication of 2 in for loop is because in stereo their are 2 channels
    for (size_t i = 0;i < num_frames * 2;i++) {
        float sample = (float)samples[i] * gain;

        clamp_check(&sample);
        samples[i] = (int16_t)(sample + 0.5f);
        }
    }

void apply_gain_mono(int16_t* samples, size_t num_samples, float gain) {

    for (size_t i = 0;i < num_samples;i++) {
        float sample = (float)samples[i] * gain;
        clamp_check(&sample);
        samples[i] = (int16_t)(sample + 0.5f);
        }
    }

// 8-bit audio
void apply_gain_8bit(int8_t* samples, size_t num_samples, float gain) {
    for (size_t i = 0;i < num_samples;i++) {
        float sample = (float)samples[i] * gain;

        // clamp check
        clamp_check(&sample);
        samples[i] = (int8_t)(samples[i] + 0.5f);
        }
    }

void print_progress(const char* input_filename, const char* output_filename, uint8_t gain) {
    printf("\n\t%s%s Volume Control Program %s\n", BOLD, FG_BLUE, RESET);
    puts("============================================");
    puts("Arguments:\n");
    printf("%s - Input WAV file\n", input_filename);
    printf("%s - Output WAV file\n", output_filename);
    printf("Applied Gain : %u\n", gain);
    }

void print_usage() {
    printf("\nVolume Control Program\n");
    printf("======================\n\n");
    printf("Usage: volume <input.wav> <output.wav> <gain>\n\n");
    printf("Arguments:\n");
    printf("  input.wav   - Input WAV file\n");
    printf("  output.wav  - Output WAV file\n");
    printf("  gain        - Volume multiplier (e.g., 0.5 for half, 2.0 for double)\n\n");
    printf("Examples:\n");
    printf("  volume input.wav quiet.wav 0.5       # Reduce volume by half\n");
    printf("  volume input.wav loud.wav 1.5        # Increase volume by 50%%\n");
    printf("  volume input.wav silent.wav 0.0      # Make completely silent\n");
    }

void print_header_info(wav_header_t* header, bool input_file) {
    printf("%s %s File : %s\n", FG_BCYAN, (input_file ? "Input" : "Output"), RESET);
    printf("Audio format : %u\n", header->fmt.audio_format);
    printf("Audio Channel : %u\n", header->fmt.audio_channels);
    printf("bits per sample : %u\n", header->fmt.bits_per_sample);
    printf("Sample rate : %u\n", header->fmt.sample_rate);

    }

int main(int argc, char* argv[]) {
    if (argc != 4) {
        print_usage();
        return 1;
        }

    const char* input_file = argv[1];
    const char* output_file = argv[2];
    float gain = atof(argv[3]);
    if (gain < 0.0f) {
        printf("[%sError%s]  Gain must be Positive\n", FG_RED, RESET);
        return 1;
        }

    print_progress(input_file, output_file, gain);

    printf("%sProcessing...%s\n", FG_GREEN, RESET);

    // Read headers

    wav_header_t* header = NULL;
    int8_t* data = read_wav_data(input_file, &header);
    
    if (!data) { return 1; }
    size_t data_size = header->bytes_to_read;

    print_header_info(header, true);

    // Applying gain in 16 bit stereo 
    if (header->fmt.bits_per_sample == 16 && header->fmt.audio_channels == 2) {
        int16_t* samples = (int16_t*)data;
        size_t num_frames = data_size / 4;
        apply_gain_stereo(samples, num_frames, gain);
        }
    else if (header->fmt.bits_per_sample == 16 && header->fmt.audio_channels == 1) {
        // 16-bit mono
        int16_t* samples = (int16_t*)data;
        size_t num_samples = data_size / 2;
        apply_gain_mono(samples, num_samples, gain);
        }
    else if (header->fmt.bits_per_sample == 8) {
        // 8-bit audio
        apply_gain_8bit((int8_t*)data, data_size, gain);
        }
    else {
        printf("Error: Unsupported format (%u bit, %u channels)\n",
            header->fmt.bits_per_sample, header->fmt.audio_channels);
        free(data);
        return 1;
        }

    // Write output file
    if (write_wav_file(output_file, &header, data, data_size) != 0) {
        free(data);
        return 1;
        }

    puts("Done !");
    print_header_info(header, false);

    free(data);
    return 0;
    }