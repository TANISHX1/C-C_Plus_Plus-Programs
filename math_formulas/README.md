## The Translation Dictionary

When you see a math formula, you can map its parts directly to C programming concepts:

*   **The Accumulator** ($y = \dots$): A summation means you are adding a bunch of numbers together into a final total. You need a variable starting at `0` to hold this running total.
*   **The Summation Symbol** ($\Sigma$): This always translates to a `for` loop.
*   **The Bottom Limit** ($k=1$): This is the start of your loop (`int k = 1;`).
*   **The Top Limit** ($12$): This is the end condition of your loop (`k <= 12;`).
*   **The Math Inside**: This is the code executed inside the loop.
*   **The Constant Outside** ($6$): This is multiplied at the very end after the loop finishes.

### Step-by-step C code 

```c
#include <stdio.h>
#include <math.h> // Required for the sin() function

// Function to calculate y for a given x
double calculate_y(double x) {
    double y_sum = 0.0; // Step 1: Create an accumulator

    // Step 2, 3 & 4: The Sigma (Summation) becomes a for loop
    for (int k = 1; k <= 12; k++) {
        
        // Step 5: Translate the math inside the summation
        // NOTE: We use 1.0 and 2.0 to force decimal math!
        double term = (1.0 / (2.0 * k + 1.0)) * sin((2.0 * k + 1.0) * x);
        
        y_sum += term; // Add the current term to our running total
    }

    // Step 6: Multiply by the constant (6) on the outside of the Sigma
    double y = 6.0 * y_sum;
    
    return y;
}

int main() {
    double x_value = 1.5; // Example input for x
    double result = calculate_y(x_value);
    
    printf("For x = %f, y = %f\n", x_value, result);
    
    return 0;
}
```
> always Remember the calculation of int type variable eith float variable (like int division with float is float but assigning to the int variable type will implicity convert it to int )