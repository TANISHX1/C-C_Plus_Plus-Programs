#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    float current_phase;
    float phase_increment;
    int num_harmonic;     // Changed to int, as harmonics are discrete steps
    float frequency;
} function_data;

float squared_sample(function_data* state) {
    float y_sum = 0.0f;

    // REASON: A true Fourier series requires stacking all harmonics simultaneously 
    // for the exact same point in time (phase). The for-loop ensures the wave 
    // shape forms correctly on every single sample.
    for (int k = 1; k <= state->num_harmonic; k++) {
        
        // REASON: (2k - 1) gives us only ODD harmonics (1, 3, 5, 7...). 
        // Using -1 instead of +1 ensures our very first loop iteration captures 
        // the fundamental base frequency (the note the user actually typed).
        float harmonic = (2.0f * k) - 1.0f;
        
        // REASON: In a square wave, the amplitude of a harmonic is inversely 
        // proportional to its frequency (1 / harmonic).
        y_sum += (1.0f / harmonic) * sinf(harmonic * state->current_phase);
    }

    // REASON: The raw summation of an infinite square wave converges to Pi/4. 
    // Multiplying by 4/Pi scales the peak amplitude down to exactly 1.0.
    float normalized_sample = y_sum * (4.0f / (float)M_PI);

    // REASON: Hard clipping prevents DAC integer overflow caused by the Gibbs 
    // phenomenon (overshoot spikes at the sharp edges of finite Fourier series).
    if (normalized_sample > 1.0f)  return 1.0f;
    if (normalized_sample < -1.0f) return -1.0f;

    return normalized_sample;
}