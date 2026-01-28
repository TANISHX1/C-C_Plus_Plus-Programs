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
#include "../decor.h"

#define MAX_SAMPLES 10000000

typedef struct
    {
    // RIFF 
    uint8_t riff[4];
    uint32_t file_size;
    uint8_t wave[4];

    // FORMAT HEADERS
    uint8_t fmt[4];
    uint32_t fmt_size;
    uint16_t audio_format;
    uint16_t audio_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;

    // DATA CHUNK / BLOCK / PACKET
    uint8_t data[4];
    uint32_t data_size;
    }wav_header_t;

// function to read entire .wav file in memory
int8_t* read_wav_data(const char* filename, wav_header_t* header, size_t* data_size) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("[%sError%s] : File cannot open or not exist '%s'\n", FG_RED, RESET, filename);
        }

    size_t header_read = fread(header, 1, sizeof(wav_header_t), file);
    if (header_read < 44) {
        printf("[%sError%s] : Invalid WAV file (file size is to small to read headers)");
        fclose(file);
        return NULL;
        }
    // Cal. number of samples
    size_t num_sample = header->data_size / header->block_align;

    // limit to MAX_SAMPLES
    if (num_sample > MAX_SAMPLES) {
        printf("%sWarning :File is too large, exciding the Sample { MAX_SAMPLE : %d} limit%s\n", FG_YELLOW, MAX_SAMPLES, RESET);
        num_sample = MAX_SAMPLES;
        }

    // Allocate memory for audio data
    /*  block algin will give the size of snapshort / size of the sample (including all channels)
        multiply total samples with block_align to get the no of bytes to read
        */
    size_t bytes_to_read = num_sample * header->block_align;
    /*why used int8_t and why not caused errors?
        malloc(bytes_to_read) reserves a contiguous block of bytes.
        Casting it to int8_t* tells the compiler to treat this block as an array of individual 8-bit bytes,
        which is perfect for binary data or raw data manipulation. */
    int8_t* data = (int8_t)malloc(bytes_to_read);
    if (!data) {
        printf("[%sError%s] : Cannot allocate the memory to audio data \n", FG_RED, RESET);
        fclose(file);
        return NULL;
        }

    // Read audio data
    size_t bytes_read = fread(data, 1, bytes_to_read, file);
    fclose(file);

    *data_size = bytes_read;
    return data;
    }

// Write WAV file with modification data

int write_wav_file(const char* filename, wav_header_t* header, int8_t* data, size_t data_size) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("[%sError%s] ; cannot able to open the file '%s'\n", FG_RED, RESET, filename);
        return -1;
        }

    // Update the headers
    header->file_size = 36 + data_size;
    }