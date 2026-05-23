// To learn How to write or convert a mathematical
// formula to code . check the math-function directory in c-c_Plus_Plus-Programs/math_functions


#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
typedef struct
    {
    float current_phase;
    float phase_increment;
    float num_harmonic;
    float frequency;
    int k; 
    }function_data;


float squared_sample(function_data* state) {
    float y_sum = 0.0f;
    if (state->k >= 1 && state->k < state->num_harmonic) {
        y_sum += (1.0 / (float)(2 * state->k - 1)) * sinf((2 * state->k - 1) * state->current_phase);
        state->k++;
        }

    else
        state->k = 1;

    // Reason: The raw summation of a sawtooth converges to Pi/2. 
    // Multiplying by 2/Pi scales the peak amplitude down to exactly 1.0.
    float normalized_sample = y_sum * (4.0f / (float)M_PI);

    // Applying the clipping 
    if (normalized_sample > 1.0f)  return 1.0f;
    if (normalized_sample < -1.0f) return -1.0f;

    return normalized_sample ;
    }