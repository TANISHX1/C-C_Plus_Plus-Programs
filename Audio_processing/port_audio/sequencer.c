/* This program produces a rhythmic synthesizer bassline using mathematical Fourier equations */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portaudio.h>

// Your custom terminal decorator
#include </home/blazex/Documents/git/C-C_Plus_Plus-Programs/Audio_processing/decor.h>

#define framesperbuffer 300
#include "sequencer_function.h" // Your local header

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

// REASON: We need an absolute master clock to calculate rhythm independently 
// of the phase/pitch of the audio wave itself.
volatile static unsigned long total_samples_played = 0; 

static int callback_function(const void* input, void* output, unsigned long framecount,
    const PaStreamCallbackTimeInfo* timeinfo, PaStreamCallbackFlags statusflag, void* userData) {

    // Timeline printing 
    if (call_back_count % 44100 == 0) {
        printf(" %d sec Elapsed \n", call_back_count / 44100);
    }
    call_back_count += framesperbuffer;

    function_data* data = (function_data*)userData;
    float* out = (float*)output;

    // REASON: Set the metronome. 44100 samples / 4 = 4 beats per second (16th notes at 60BPM).
    float samples_per_beat = 44100.0f / 4.0f;

    for (unsigned long i = 0; i < framecount; i++) {
        
        // 1. GENERATE RAW TONE
        float raw_synth_tone = squared_sample(data);

        // 2. CALCULATE AMPLITUDE ENVELOPE (RHYTHM)
        // REASON: fmodf creates a looping remainder based on our master clock. 
        // Dividing it down gives us a repeating saw-tooth value from 0.0 to 1.0 every beat.
        float beat_phase = fmodf((float)total_samples_played, samples_per_beat) / samples_per_beat;

        // REASON: expf(-5.0 * x) shapes that linear saw-tooth into a fast, natural 
        // logarithmic curve, simulating the physical "pluck" and fade of a synthesizer key.
        float volume_envelope = expf(-5.0f * beat_phase);

        // 3. APPLY MASTER MIX
        // REASON: Multiply the raw wave by the envelope to carve out the rhythm, 
        // then scale down by 0.15f so the square wave doesn't blow out the speakers.
        out[i] = raw_synth_tone * volume_envelope * 0.15f;

        // 4. ADVANCE TIME STATE
        data->current_phase += data->phase_increment;
        if (data->current_phase >= 2.0f * M_PI) {
            data->current_phase -= 2.0f * M_PI;
        }
        
        total_samples_played++;
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
    
    // Updated prompt text to encourage the correct bass frequencies for this effect
    printf("\tEnter a Bass Frequency (e.g., 55 or 65 Hz) :\t ");
    scanf("%f", &function->frequency);

    function->current_phase = 0.0f;
    function->phase_increment = (2.0f * M_PI * function->frequency) / 44100.0f;
    function->num_harmonic = 25; // 25 is plenty for a thick bass tone

    // PortAudio Initialization
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        fprintf(stdout, FG_BRED"PortAudio initilization Failed!"RESET"\n");
        return 1;
    }
    puts(BLINK"PortAudio initalized Successfully"RESET);

    // Selecting output device
    PaDeviceIndex device = Pa_GetDefaultOutputDevice();
    if (device == paNoDevice) {
        fprintf(stdout, FG_BRED"No output Device Available"RESET);
        return 1;
    }
    device_info(device);
    
    // Configure Device 
    PaStreamParameters outputpara = { 0 };

    // REASON: 1 channel requested. Linux audio subsystems like PipeWire/PulseAudio 
    // will intercept this mono stream and safely duplicate it to L/R stereo speakers automatically.
    outputpara.channelCount = 1; 
    outputpara.device = device;
    outputpara.hostApiSpecificStreamInfo = NULL;
    outputpara.sampleFormat = paFloat32;
    outputpara.suggestedLatency = Pa_GetDeviceInfo(device)->defaultHighOutputLatency;

    // Opening stream
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

    fprintf(stdout, FG_BCYAN BOLD"=== Retro Synth Rhythm Sequencer ==="RESET"\n");
    fprintf(stdout, FG_BMAGENTA"Duration : %d\n", time_ms / 1000, RESET"\n");
    puts("► Playing sequence.....");
    
    Pa_StartStream(stream);
    Pa_Sleep(time_ms + 30);
    puts("\n\t Done\n");
    
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    free(function);
    Pa_Terminate();
    
    return 0;
}