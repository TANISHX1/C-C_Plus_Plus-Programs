#include <stdio.h>
#include <math.h>
#include <portaudio.h>
#include <stdlib.h>
#include </home/blazex/Documents/git/C-C_Plus_Plus-Programs/Audio_processing/decor.h>
#define framersperbuffer 300

// ____________________________PseduoCode___________________________________
// 1. Create BeeperData struct
//    - phase = 0.0
//    - frequency = 500
//    - sample_rate = 44100

// 2. In main():
//    - Initialize PortAudio
//    - Configure output (like Practice 2)
//    - Pass BeeperData to callback via userData
//    - Open stream with callback
//    - Start stream
//    - Sleep for 3000ms
//    - Cleanup

// 3. In callback:
//    - Cast userData to BeeperData*
//    - For each frame:
//      * output[i] = sin(2π × phase)
//      * phase += frequency / sample_rate
//      * if phase >= 1.0: phase -= 1.0
//    - Update callback_count
//    - Show progress every 44100 samples
//    - Return paContinue
// _________________________________________________________________________


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

// phase data_struct
typedef struct
    {
    float phase;
    float frequency;
    float sample_rate;
    }beeperdata;

// Global variables
volatile static int time_ms;
volatile static int callback_count = 0;

static int beeper_callback(const void* input, void* output,
    unsigned long framecount, const PaStreamCallbackTimeInfo* timeinfo,
    PaStreamCallbackFlags statusflag, void* userData)
    {
    beeperdata* data = (beeperdata*)userData;
    if (callback_count % 44100 == 0) {
        printf("%d Second Elapsed (%d/%d)\n",
            callback_count / 44100,
            callback_count / 44100,
            time_ms / 1000);
        }
    callback_count += framersperbuffer;
    float* out = (float*)output;
    for (unsigned long i = 0;i < framecount;i++) {
        out[i] = sinf(2.0f * 3.14159265f * data->phase);
        data->phase += data->frequency / data->sample_rate;
        //to keeep in range [0,1)
        if (data->phase >= 1.0f) {
            data->phase -= 1.0f;
            }
        }
    return paContinue;
    }

int main(int argv, char* argc[]) {
    if (argv < 3 || argv>3) {
        printf(FG_BRED" Usage : silence_v2.c <silence (in sec)> <Beeper Frequency> from [1,44100) \n less frequency long beeper | more frequency short beeper..."RESET"\n");
        return 1;
        }
    
    beeperdata* beeper = malloc(sizeof(beeperdata));
    if (argv == 3) {
        time_ms = atoi(argc[1]) * 1000;
        beeper->frequency = atoi(argc[2]);
        beeper->phase = 0;
        beeper->sample_rate = 44100;
        }
    
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
    // configure Device
    PaStreamParameters outputpara = { 0 };
    outputpara.device = device;
    outputpara.channelCount = 2;
    outputpara.hostApiSpecificStreamInfo = NULL;
    outputpara.sampleFormat = paFloat32;
    outputpara.suggestedLatency = Pa_GetDeviceInfo(device)->defaultHighOutputLatency;

    PaStream* stream = NULL;

    err = Pa_OpenStream(
        &stream,
        NULL,
        &outputpara,
        44100,
        framersperbuffer,
        paClipOff,
        beeper_callback,
        (void*)beeper
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
    free(beeper);
    Pa_Terminate();
    return 0;

    }

