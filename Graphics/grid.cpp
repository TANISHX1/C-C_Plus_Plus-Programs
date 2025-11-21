// Grid printing with animated drawing of squares and diagonals using putpixel
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <direct.h>
int animationspeed = 1;
void drawSquareWithDiagonals(int x1, int y1, int step) {
    int x2 = x1 + step;
    int y2 = y1 + step;
    
    for (int i = x1; i <= x2; i++) {
        putpixel(i, y1, WHITE);
        putpixel(i, y2, WHITE);
        delay(animationspeed);
    }
    for (int j = y1; j <= y2; j++) {
        putpixel(x1, j, WHITE);
        putpixel(x2, j, WHITE);
        delay(animationspeed);
    }
    
    // Draw diagonals inside the square using putpixel
    for (int i = 0; i <= step; i++) {
        putpixel(x1 + i, y1 + i, WHITE); // Main diagonal
        putpixel(x2 - i, y1 + i, WHITE); // Other diagonal
        delay(animationspeed);
    }
    
    // Draw center lines using putpixel
    for (int i = y1; i <= y2; i++) {
        putpixel((x1 + x2) / 2, i, WHITE); // Vertical center
        delay(animationspeed);
    }
    for (int i = x1; i <= x2; i++) {
        putpixel(i, (y1 + y2) / 2, WHITE); // Horizontal center
        delay(animationspeed);
    }
}

void drawPatternWithAnimation() {
    int step = 100; // Grid step size
    int width = 640;
    int height = 475;
    
    for (int x = 0; x <= width; x += step) {
        for (int y = 0; y <= height; y += step) {
            drawSquareWithDiagonals(x, y, step);
            delay(1); // Delay 
        }
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm,(char * ) ""); // Initialize graphics mode
    
    drawPatternWithAnimation(); // Animated drawing
    
    getch(); // Wait for a key press
    closegraph(); // Close the graphics window
    return 0;
}
