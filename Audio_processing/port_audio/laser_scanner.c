/* this program Produces laser scanning SOund by using mathematical
equations   */
#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include </home/blazex/Documents/git/C-C_Plus_Plus-Programs/Audio_processing/decor.h>
#define framesperbuffer 300
#include "funtion.h"



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



// Global Variables
volatile static int time_ms;
volatile static int call_back_count = 0;

static int callback_function(const void* input, void* output, unsigned long framecount,
    const PaStreamCallbackTimeInfo* timeinfo, PaStreamCallbackFlags statusflag, void* userData) {

    // timeline printing 
    if (call_back_count % 44100 == 0) {
        printf(" %d sec Elapsed \n", call_back_count / 44100);
        }

    call_back_count += framesperbuffer;

    function_data* data = (function_data*)userData;
    float* out = (float*)output;

    for (unsigned long i = 0;i < framecount;i++) {
        out[i] = squared_sample(data);
        data->current_phase += data->phase_increment;

        if (data->current_phase >= 2.0f * M_PI) {
            data->current_phase -= 2.0f * M_PI;
            }
        }

    return paContinue;

    }

int main(int argv, char* argc[]) {
    function_data* function = (function_data*)malloc(sizeof(function_data));
    if (function == NULL) {
        printf(FG_BRED"[ Error ]"RESET" Failed to initialize function\n");
        return 1;
        }

    printf("\tEnter the runtime (in sec) :\t ");
    scanf("%d", &time_ms);
    time_ms *= 1000;
    printf("\tEnter the Frquency (in Hz , multiple of 440Hz) :\t ");
    scanf("%f", &function->frequency);

    function->current_phase = 0.0f;
    function->phase_increment = (2.0f * M_PI * function->frequency) / 44100.0f;
    function->num_harmonic = 100;

    // port audio initilization
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        fprintf(stdout, FG_BRED"PortAudio initilization Failed!"RESET"\n");
        return 1;
        }
    puts(BLINK"PortAudio initalized Successfully"RESET);

    // selecting output device
    PaDeviceIndex device = Pa_GetDefaultOutputDevice();
    if (device == paNoDevice) {
        fprintf(stdout, FG_BRED"No output Device Available"RESET);
        return 1;
        }
    device_info(device);
    // Configure Device 
    PaStreamParameters outputpara = { 0 };

    outputpara.channelCount = 1; // using only one ,because OS (like fedora which used pipewire)
    // automatically divides (or duplicates) the according to the audio hardware system (mono , stereo,5.1) 
    outputpara.device = device;
    outputpara.hostApiSpecificStreamInfo = NULL;
    outputpara.sampleFormat = paFloat32;
    outputpara.suggestedLatency = Pa_GetDeviceInfo(device)->defaultHighOutputLatency;

    // opening stream
    PaStream* stream = NULL;
    err = Pa_OpenStream(
        &stream,
        NULL,
        &outputpara,
        44100,
        framesperbuffer,
        paClipOff,
        callback_function,
        (void*)function
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
    Pa_Sleep(time_ms + 30);
    puts("\n\t Done\n");
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    free(function);
    Pa_Terminate();
    return 0;

    }