/* WAV File Header Reader
RIFF Header( 12Bytes ): File type , Size
Format chunk (24+ Bytes) : Audio type , channels, sample rate
Data chunk : actual audio samples
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// WAV file header structures
typedef struct {
    uint8_t  riff[4];        // "RIFF" (4 bytes)
    uint32_t file_size;      // File size minus 8 bytes
    uint8_t  wave[4];        // "WAVE" (4 bytes)
} riff_header_t;

typedef struct {
    uint8_t  chunk_id[4];    // Chunk identifier
    uint32_t chunk_size;    // Size of chunk data
} chunk_header_t;

typedef struct {
    uint16_t audio_format;   // Audio format (1 = PCM)
    uint16_t num_channels;   // Number of channels (1 = mono, 2 = stereo)
    uint32_t sample_rate;    // Samples per second (e.g., 44100)
    uint32_t byte_rate;      // Bytes per second (sample_rate * block_align)
    uint16_t block_align;    // Block align (channels * bits_per_sample / 8)
    uint16_t bits_per_sample;// Bits per sample (8, 16, 24, 32)
} fmt_chunk_t;

typedef struct {
    riff_header_t riff;
    fmt_chunk_t fmt;
    uint32_t data_size;
    uint32_t data_offset;
} wav_info_t;

// Function to display header information
void display_header_info(wav_info_t* info) {
    printf("\n========== WAV File Information ==========\n\n");

    // File validation
    printf("File Validation:\n");
    printf("  RIFF marker:    %.4s\n", info->riff.riff);
    printf("  WAVE marker:    %.4s\n", info->riff.wave);

    // Audio format details
    printf("\nAudio Format:\n");
    printf("  Audio Format:   %d ", info->fmt.audio_format);
    if (info->fmt.audio_format == 1) {
        printf("(PCM - uncompressed)\n");
    }
    else {
        printf("(Compressed format)\n");
    }
    printf("  Channels:       %u (%s)\n", info->fmt.num_channels, info->fmt.num_channels == 1 ? "Mono" : "Stereo");
    printf("  Sample Rate:    %u Hz\n", info->fmt.sample_rate);
    printf("  Bit Depth:      %u bits\n", info->fmt.bits_per_sample);

    // Calculate and display additional information
    uint32_t bytes_per_second = info->fmt.byte_rate;
    float duration_seconds = (float)info->data_size / bytes_per_second;

    printf("\nTiming Information:\n");
    printf("  Byte Rate:      %u bytes/second\n", bytes_per_second);
    printf("  Block Align:    %u bytes/sample\n", info->fmt.block_align);
    printf("  Data Size:      %u bytes\n", info->data_size);
    printf("  Duration:       %.2f seconds\n", duration_seconds);

    // Calculate number of samples
    uint32_t total_samples = info->data_size / info->fmt.block_align;
    printf("  Total Samples:  %u\n", total_samples);
    printf("  Per Channel:    %u\n", total_samples / info->fmt.num_channels);

    printf("\n==========================================\n\n");
}

wav_info_t* read_wav_header(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("[Error] : Cannot open the file '%s'\n", filename);
        return NULL;
    }

    wav_info_t* info = (wav_info_t*)calloc(1, sizeof(wav_info_t));
    if (!info) {
        puts("[Error] : Memory Allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Read RIFF Header
    if (fread(&info->riff, 1, sizeof(riff_header_t), file) < sizeof(riff_header_t)) {
        puts("[Error] : Failed to read RIFF header\n");
        free(info);
        fclose(file);
        return NULL;
    }

    if (strncmp((char*)info->riff.riff, "RIFF", 4) != 0 || strncmp((char*)info->riff.wave, "WAVE", 4) != 0) {
        puts("[Error] : Not a valid WAV file\n");
        free(info);
        fclose(file);
        return NULL;
    }

    // Iterate through chunks
    chunk_header_t chunk;
    int fmt_found = 0;
    int data_found = 0;

    while (fread(&chunk, 1, sizeof(chunk_header_t), file) == sizeof(chunk_header_t)) {
        if (strncmp((char*)chunk.chunk_id, "fmt ", 4) == 0) {
            // Read fmt chunk (at least 16 bytes)
            uint32_t size_to_read = (chunk.chunk_size < sizeof(fmt_chunk_t)) ? chunk.chunk_size : sizeof(fmt_chunk_t);
            if (fread(&info->fmt, 1, size_to_read, file) < size_to_read) {
                puts("[Error] : Failed to read fmt chunk\n");
                break;
            }
            // Skip any extra bytes in fmt chunk (e.g. extension bytes)
            if (chunk.chunk_size > sizeof(fmt_chunk_t)) {
                fseek(file, chunk.chunk_size - sizeof(fmt_chunk_t), SEEK_CUR);
            }
            fmt_found = 1;
        } else if (strncmp((char*)chunk.chunk_id, "data", 4) == 0) {
            info->data_size = chunk.chunk_size;
            info->data_offset = ftell(file);
            data_found = 1;
            break; // Usually 'data' is the last interesting chunk, but we could continue
        } else {
            // Skip unrecognized chunks
            fseek(file, chunk.chunk_size, SEEK_CUR);
        }
    }

    fclose(file);

    if (!fmt_found || !data_found) {
        printf("[Error] : Missing critical chunks (fmt: %s, data: %s)\n", fmt_found ? "found" : "MISSING", data_found ? "found" : "MISSING");
        free(info);
        return NULL;
    }

    return info;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage:%s <input.wav>\n", argv[0]);
        printf("Example: %s test.wav\n", argv[0]);
        return 1;
    }

    // Read wav file header
    wav_info_t* info = read_wav_header(argv[1]);
    if (!info) {
        return 1;
    }

    display_header_info(info);

    free(info);
    return 0;
}