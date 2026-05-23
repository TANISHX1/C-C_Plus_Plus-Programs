
#include <stdio.h>
#include <math.h>

double fourier(float phase) {
    float y = 0;
    for (int k = 1; k <= 12;k++) {
        y += (1.0 / (2 * k + 1.0)) * sinf((2 * k + 1) * phase);
        printf(" K: %d\tY: %f\n", k, y);
        }
    return 0.0;
    }

int main() {
    fourier(3.5);
    return 0;
    }