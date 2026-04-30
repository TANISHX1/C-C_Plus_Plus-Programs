#include <stdio.h>
#include <portaudio.h>
#include </home/blazex/Documents/git/C-C_Plus_Plus-Programs/Audio_processing/decor.h>
#include <stdlib.h>
#include <time.h>
#define framesperbuffer 300

void device_info(PaDeviceIndex device) {
    const PaDeviceInfo* info = Pa_GetDeviceInfo(device);
    printf(FG_BGREEN "\t\t┌───────────────────────────────────────────────────────────┐\n" RESET);
    printf(FG_BGREEN "\t\t│" RESET " Device [%d] | Device Name : %-30s " FG_BGREEN "│\n" RESET, device, info->name);
    printf(FG_BGREEN "\t\t├───────────────────────────────────────────────────────────┤\n" RESET);
    printf(FG_BGREEN "\t\t│" RESET "    Input:                                                 " FG_BGREEN "│\n" RESET);
    printf(FG_BGREEN "\t\t│" RESET "           Max Input channels:          %10d         " FG_BGREEN "│\n" RESET, info->maxInputChannels);
    printf(FG_BGREEN "\t\t│" RESET "           High Input latency [Default]: %10f        " FG_BGREEN "│\n" RESET, info->defaultHighInputLatency);
    printf(FG_BGREEN "\t\t│" RESET "           Low Input Latency  [Default]: %10f        " FG_BGREEN "│\n" RESET, info->defaultLowInputLatency);
    printf(FG_BGREEN "\t\t│" RESET "    Output:                                                " FG_BGREEN "│\n" RESET);
    printf(FG_BGREEN "\t\t│" RESET "           Max Output channels:         %10d         " FG_BGREEN "│\n" RESET, info->maxOutputChannels);
    printf(FG_BGREEN "\t\t│" RESET "           High Output latency [Default]: %10f       " FG_BGREEN "│\n" RESET, info->defaultHighOutputLatency);
    printf(FG_BGREEN "\t\t│" RESET "           Low Output Latency  [Default]: %10f       " FG_BGREEN "│\n" RESET, info->defaultLowOutputLatency);
    printf(FG_BGREEN "\t\t│" RESET "    Sample Rate: %-10.1f                                " FG_BGREEN "│\n" RESET, info->defaultSampleRate);
    printf(FG_BGREEN "\t\t└───────────────────────────────────────────────────────────┘\n" RESET);

    }
// Global variables
volatile static int time_ms;
volatile static int callback_count = 0;

float get_random(float min, float max) {
    return (((float)rand() / (float)RAND_MAX) * (max - min) + min);
    }

// callback function
static int audiocallback(const void* input, void* output,
    unsigned long framecount, const PaStreamCallbackTimeInfo* timeinfo,
    PaStreamCallbackFlags statusflag, void* userData)
    {
    if (callback_count % 44100 == 0) {
        printf("%d Second elapsed (%d/%d) \n",
            callback_count / 44100,
            callback_count / 44100,
            time_ms / 1000
        );
        }
    callback_count += framesperbuffer;

    float* out = (float*)output;
    for (unsigned long i = 0;i < framecount;i++) {
        out[i] = get_random(-100,100); //zero = silence
        }
    return paContinue;
    }

int main(int argv, char* argc[]) {

    if (argv < 2 || argv>2) {
        printf(FG_BRED" Usage : silence_v2.c <silence (in sec)>..."RESET"\n");
        return 1;
        }
    if (argv == 2) {
        time_ms = atoi(argc[1]) * 1000;
        }
    // seeding generator
    srand(time(NULL));

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        fprintf(stdout, FG_BRED"PortAudio initilization Failed!"RESET"\n");
        return 1;
        }
    puts("Portaudio initilized Successfully");
    PaDeviceIndex device = Pa_GetDefaultOutputDevice();
    if (device == paNoDevice) {
        fprintf(stdout, FG_BRED"No output Device Available"RESET);
        return 1;
        }
    device_info(device);

    // configure device
    PaStreamParameters outputparamaters = { 0 };
    outputparamaters.device = device;
    outputparamaters.channelCount = Pa_GetDeviceInfo(device)->maxOutputChannels;
    outputparamaters.sampleFormat = paFloat32;
    outputparamaters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultHighOutputLatency;
    outputparamaters.hostApiSpecificStreamInfo = NULL;

    PaStream* stream = NULL;

    err = Pa_OpenStream(
        &stream,
        NULL,
        &outputparamaters,
        44100,
        framesperbuffer,
        paClipOff,
        audiocallback,
        NULL
    );
    if (err != paNoError) {
        fprintf(stdout, FG_BRED"Failed to open Stream\n" RESET);
        Pa_Terminate();
        return 1;
        }
    fprintf(stdout, FG_BCYAN, BOLD"=== Custom Duration Silence Player==="RESET"\n");
    fprintf(stdout, FG_BMAGENTA"Duration : %d\n", time_ms / 1000, RESET"\n");
    puts("► Playing sample.....");
    Pa_StartStream(stream);
    Pa_Sleep(time_ms + 30); // +30ms to correctly print last message by callback function
    puts("\n\t Done\n");
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
    return 0;


    }