#include <stdio.h>
#include <portaudio.h>

// setp 1: define callback function
static int audiocallback(const void* input, void* output,
    unsigned long framecount, const PaStreamCallbackTimeInfo* timeinfo,
    PaStreamCallbackFlags statusflag, void* userData)
    {
    float* out = (float*)output;
    for (unsigned long i = 0;i < framecount;i++) {
        out[i] = 0.0f; //zero = silence
        }
    return paContinue;
    }

int main() {
    printf("=== PortAudio Silence Test ===\n\n");

    // step 2 : initilize the portaudio
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        printf("[Error]: initilization failed!..\n");
        return 1;
        }
    puts("PortAudio Successfully Initilized\n");

    // step 3 : configure output
    PaStreamParameters outputparams = { 0 };
    outputparams.device = Pa_GetDefaultOutputDevice(); // selects the output devices bydefault used by OS 
    outputparams.channelCount = 2;  //total channels
    outputparams.sampleFormat = paFloat32;
    outputparams.suggestedLatency =
        Pa_GetDeviceInfo(outputparams.device)->defaultHighOutputLatency;
    outputparams.hostApiSpecificStreamInfo = NULL;
    printf("Device :%s\n\n", Pa_GetDeviceInfo(outputparams.device)->name);

    // step 4 : open stream 
    PaStream* stream = NULL;
    err = Pa_OpenStream(
        &stream,
        NULL,          // no input field / microphone
        &outputparams,  // Speaker output setting
        44100,        // 44100 samples per second
        256,         // no. of samples per callback
        paClipOff,
        audiocallback,  // call our function 
        NULL            // No custom data
    );

    if (err != paNoError) {
        printf("[Error] Open stream Failed!...\n");
        Pa_Terminate();
        return 1;
        }
    puts("Stream opended");
    // step 5: Start playback
    Pa_StartStream(stream);
    puts("Playing silnce for 2 sec...\n");
    // step 6 : wait for 2 sec
    Pa_Sleep(2000); // sleeep for 2 sec
    printf("Done");
    // step 7 : cleanup 
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
    return 0;
    }