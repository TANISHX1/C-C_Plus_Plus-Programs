/* WAV File Header Reader
RIFF Header( 12Bytes ): File type , Size
Format chunk (24+ Bytes) : Audio type , channels, sample rate
Data chunk : actual audio samples
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// WAV file header structure
typedef struct {
    // RIFF chunk
    uint8_t  riff[4];        // "RIFF" (4 bytes)
    uint32_t file_size;      // File size minus 8 bytes
    uint8_t  wave[4];        // "WAVE" (4 bytes)

    // Format chunk
    uint8_t  fmt[4];         // "fmt " (4 bytes)
    uint32_t fmt_size;       // Format chunk size (usually 16 for PCM)
    uint16_t audio_format;   // Audio format (1 = PCM)
    uint16_t num_channels;   // Number of channels (1 = mono, 2 = stereo)
    uint32_t sample_rate;    // Samples per second (e.g., 44100)
    uint32_t byte_rate;      // Bytes per second (sample_rate * block_align)
    uint16_t block_align;    // Bytes per sample (channels * bits_per_sample / 8)
    uint16_t bits_per_sample;// Bits per sample (8, 16, 24, 32)

    // Data chunk
    uint8_t  data[4];        // "data" (4 bytes)
    uint32_t data_size;      // Size of audio data
    } wav_header_t;

// Function to display header information
void display_header_info(wav_header_t* header) {
    printf("\n========== WAV File Information ==========\n\n");

    // File validation
    printf("File Validation:\n");
    printf("  RIFF marker:    %.4s (should be 'RIFF')\n", header->riff);
    printf("  WAVE marker:    %.4s (should be 'WAVE')\n", header->wave);

    // Audio format details
    printf("\nAudio Format:\n");
    printf("  Audio Format:   %d ", header->audio_format);
    if (header->audio_format == 1) {
        printf("(PCM - uncompressed)\n");
        }
    else {
        printf("(Compressed format)\n");
        }
    printf("  Channels:       %u (%s)\n", header->num_channels, header->num_channels == 1 ? "Mono" : "Stereo");
    printf("  Sample Rate:    %u Hz\n", header->sample_rate);
    printf("  Bit Depth:      %u bits\n", header->bits_per_sample);

    // Calculate and display additional information
    uint32_t bytes_per_sample = header->bits_per_sample / 8;
    uint32_t bytes_per_second = header->sample_rate * header->num_channels * bytes_per_sample;
    float duration_seconds = (float)header->data_size / bytes_per_second;

    printf("\nTiming Information:\n");
    printf("  Byte Rate:      %u bytes/second\n", bytes_per_second);
    printf("  Block Align:    %u bytes/sample\n", header->block_align);
    printf("  Data Size:      %u bytes\n", header->data_size);
    printf("  Duration:       %.2f seconds\n", duration_seconds);

    // Calculate number of samples
    uint32_t total_samples = header->data_size / header->block_align;
    printf("  Total Samples:  %u\n", total_samples);
    printf("  Per Channel:    %u\n", total_samples / header->num_channels);

    printf("\n==========================================\n\n");
    }

// Function to check if file is valid WAV
int is_valid_wav(wav_header_t* header) {
    return (strncmp((char*)header->riff, "RIFF", 4) == 0 &&
        strncmp((char*)header->wave, "WAVE", 4) == 0 &&
        strncmp((char*)header->fmt, "fmt ", 4) == 0 &&
        strncmp((char*)header->data, "data", 4) == 0 &&
        header->audio_format == 1);  // Only supporting PCM
    }

wav_header_t* read_wav_header(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("[Error] : Cannot open the file '%s'\n", filename);
        return NULL;
        }

    // Allocate memory for header
    wav_header_t* header = (wav_header_t*)malloc(sizeof(wav_header_t));
    if (!header) {
        puts("[Error] : Memory Allocation failed\n");
        fclose(file);
        return NULL;
        }

    // Read entire header (44 Bytes)
    size_t bytes_read = fread(header, 1, sizeof(wav_header_t), file);
    fclose(file);

    if (bytes_read < 44) {
        puts("[Error ] : File too small to be a valid WAV file\n ");
        free(header);
        return NULL;
        }
    return header;

    }


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage:%s <input.wav>\n", argv[0]);
        printf("Example: %s test.wav\n", argv[0]);
        return 1;
        }

    // Read wav file header
    wav_header_t* header = read_wav_header(argv[1]);
    if (!header) {
        return 1;
        }
    if (!is_valid_wav(header)) {
        printf("Error: Invalid WAV file format\n");
        free(header);
        return 1;
        }
    display_header_info(header);

    free(header);
    return 0;
    }