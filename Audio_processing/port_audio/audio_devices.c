#include <stdio.h>
#include <portaudio.h>

int main() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        puts("[Error] port audio initialization failed!");
        return 1;
        }
    puts("Port audio initialized \n");
    int count = Pa_GetDeviceCount();
    printf("Total audio devices Count :%d\n\n", count);

    for (__uint8_t i = 0; i < count;i++) {
        puts("\033[95m ===============================================\033[0m");
        const PaDeviceInfo* device = Pa_GetDeviceInfo(i);
        if (device == NULL) {
            continue;
            }

        printf("\nDevice index [%d]\n"
            "   NAME  : %s\n"
            "   INPUT\n"
            "       Max input channels     : %d\n"
            "       Min latency [Default]  : %f\n"
            "       Max latency [Default]  : %f\n"
            "   OUTPUT\n"
            "       Max output channels    : %d\n"
            "       Min latency [Default]  : %f\n"
            "       Max latency [Default]  : %f\n"
            "   Sample_Rate [Default] : %f\n",
            i,
            device->name,
            device->maxInputChannels,
            device->defaultLowInputLatency,
            device->defaultHighInputLatency,
            device->maxOutputChannels,
            device->defaultLowOutputLatency,
            device->defaultHighOutputLatency,
            device->defaultSampleRate
        );

        }

    puts("\033[42m ===============================================\033[0m");
    PaDeviceIndex default_device = Pa_GetDefaultOutputDevice();
    if (default_device == paNoDevice) {
        puts("[Error] No output Device is available or an error occured");
        return 1;
        }
    printf("Defult device is connected with device index : %d\n", default_device);

    Pa_Terminate();
    return 0;
    }