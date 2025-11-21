/*
 * Animated Galaxy Simulation
 * Press any key to exit
 */

 #include <iostream>
 #include <conio.h>
 #include <graphics.h>
 #include <dos.h>
 #include <stdlib.h>
 #include <math.h>
 #include <time.h>
 
 // Structure to represent a star
 struct Star {
     float x, y;      // Position
     float angle;     // Angle from center
     float distance;  // Distance from center
     float speed;     // Rotation speed
     int brightness;  // Brightness (0-15)
     int color;       // Color
 };
 
 // Function prototypes
 void initStars(Star stars[], int numStars);
 void animateGalaxy(Star stars[], int numStars);
 bool checkKeyPress();
 void drawStar(int x, int y, int color, int size);
 void drawNebulaEffect();
 
 // Constants
 const int MAX_STARS = 1000;
 const int BRIGHT_STARS = 50;
 const int SCREEN_WIDTH = 640;
 const int SCREEN_HEIGHT = 480;
 
 int main() {
     // Initialize random number generator
     srand(time(NULL));
     
     // Initialize graphics mode
     int gd = DETECT, gm;
     initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
     
     // Check if graphics initialization was successful
     int errorcode = graphresult();
     if (errorcode != grOk) {
         std::cout << "Graphics error: " << grapherrormsg(errorcode) << std::endl;
         getch();
         return 1;
     }
     
     // Array to hold stars
     Star stars[MAX_STARS];
     
     // Initialize stars
     initStars(stars, MAX_STARS);
     
     // Run the animation
     animateGalaxy(stars, MAX_STARS);
     
     // Close graphics mode
     closegraph();
     return 0;
 }
 
 void initStars(Star stars[], int numStars) {
     // Galaxy center
     int centerX = SCREEN_WIDTH / 2;
     int centerY = SCREEN_HEIGHT / 2;
     
     // Initialize stars
     for (int i = 0; i < numStars; i++) {
         // Set random angle
         stars[i].angle = (float)(rand() % 360) * M_PI / 180.0f;
         
         // Set distance from center (more stars in spiral arms)
         float distanceFactor = (float)(rand() % 100) / 100.0f;
         
         // Create spiral galaxy shape: more stars in spiral arms
         if (i < numStars * 0.7) {
             // 70% stars follow spiral pattern
             stars[i].distance = 50.0f + 180.0f * distanceFactor;
             
             // Adjust angle to create spiral arms
             int arm = rand() % 5;  // 5 spiral arms
             float armAngle = (float)arm * (2.0f * M_PI / 5.0f);
             float spiral = 0.5f * stars[i].distance / 180.0f;
             stars[i].angle = armAngle + spiral + (float)(rand() % 40 - 20) * M_PI / 180.0f;
         } else {
             // 30% stars distributed randomly
             stars[i].distance = 10.0f + 220.0f * distanceFactor;
         }
         
         // Calculate position
         stars[i].x = centerX + cos(stars[i].angle) * stars[i].distance;
         stars[i].y = centerY + sin(stars[i].angle) * stars[i].distance;
         
         // Set rotation speed (stars closer to center move faster)
         stars[i].speed = 0.001f + 0.003f * (1.0f - distanceFactor);
         
         // Set brightness and color
         if (i < BRIGHT_STARS) {
             // A few bright stars
             stars[i].brightness = 8 + rand() % 8;
             
             // Bright stars have different colors
             int colorPick = rand() % 5;
             if (colorPick == 0) stars[i].color = WHITE;       // White star
             else if (colorPick == 1) stars[i].color = CYAN;   // Blue star
             else if (colorPick == 2) stars[i].color = LIGHTRED; // Red giant
             else if (colorPick == 3) stars[i].color = YELLOW; // Yellow star
             else stars[i].color = LIGHTCYAN;                  // Blue-white star
         } else {
             // Most stars are dimmer
             stars[i].brightness = 1 + rand() % 6;
             
             // Most stars are white/blue-ish
             int colorPick = rand() % 10;
             if (colorPick < 6) stars[i].color = WHITE;        // Most stars are white
             else if (colorPick < 8) stars[i].color = LIGHTCYAN; // Some blue-white
             else if (colorPick < 9) stars[i].color = LIGHTMAGENTA; // Few magenta
             else stars[i].color = LIGHTRED;                   // Few red
         }
     }
 }
 
 void animateGalaxy(Star stars[], int numStars) {
     int centerX = SCREEN_WIDTH / 2;
     int centerY = SCREEN_HEIGHT / 2;
     
     // Set background to black
     setbkcolor(BLACK);
     cleardevice();
     
     // Add title
     settextstyle(SMALL_FONT, HORIZ_DIR, 5);
     setcolor(WHITE);
     outtextxy(10, 10, "GALAXY SIMULATION - PRESS ANY KEY TO EXIT");
     
     // Draw some nebula effects first
     drawNebulaEffect();
     
     bool running = true;
     int frame = 0;
     
     // Animation loop
     while (running) {
         // Check for key press to exit
         if (checkKeyPress()) {
             running = false;
             break;
         }
         
         // Every 10 frames, clear device and redraw nebula to prevent screen clutter
         if (frame % 10 == 0) {
             cleardevice();
             
             // Add title again
             setcolor(WHITE);
             outtextxy(10, 10, "GALAXY SIMULATION - PRESS ANY KEY TO EXIT");
             
             // Redraw nebula effect
             drawNebulaEffect();
         }
         
         // Update and draw stars
         for (int i = 0; i < numStars; i++) {
             // Update star position by rotating around center
             stars[i].angle += stars[i].speed;
             if (stars[i].angle > 2 * M_PI) {
                 stars[i].angle -= 2 * M_PI;
             }
             
             // Calculate new position
             float newX = centerX + cos(stars[i].angle) * stars[i].distance;
             float newY = centerY + sin(stars[i].angle) * stars[i].distance;
             
             // Draw star at new position
             drawStar((int)newX, (int)newY, stars[i].color, stars[i].brightness);
             
             // Update position
             stars[i].x = newX;
             stars[i].y = newY;
         }
         
         // Short delay to control animation speed
         delay(30);
         frame++;
     }
 }
 
 bool checkKeyPress() {
     // Check if key is pressed
     if (kbhit()) {
         getch(); // Consume the key
         return true;
     }
     return false;
 }
 
 void drawStar(int x, int y, int color, int brightness) {
     // Don't draw stars outside the screen
     if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
         return;
     
     setcolor(color);
     
     // Different sizes based on brightness
     if (brightness > 10) {
         // Draw a bigger star (5 pixels)
         putpixel(x, y, color);
         putpixel(x+1, y, color);
         putpixel(x-1, y, color);
         putpixel(x, y+1, color);
         putpixel(x, y-1, color);
     }
     else if (brightness > 7) {
         // Draw a medium star (cross)
         putpixel(x, y, color);
         putpixel(x+1, y, color);
         putpixel(x-1, y, color);
         putpixel(x, y+1, color);
         putpixel(x, y-1, color);
     }
     else if (brightness > 4) {
         // Draw a small star (plus sign)
         putpixel(x, y, color);
         putpixel(x+1, y, color);
         putpixel(x, y+1, color);
     }
     else {
         // Draw a single pixel for dim stars
         putpixel(x, y, color);
     }
 }
 
 void drawNebulaEffect() {
     // Add nebula-like colorful clouds in the background
     // We'll use ellipses with low opacity to simulate dust clouds
     
     // Center of galaxy
     int centerX = SCREEN_WIDTH / 2;
     int centerY = SCREEN_HEIGHT / 2;
     
     // Draw purple-magenta nebula cloud
     setcolor(MAGENTA);
     setfillstyle(SOLID_FILL, MAGENTA);
     // Use XOR pattern to create semi-transparent effect
     setwritemode(XOR_PUT);
     for (int i = 0; i < 8; i++) {
         int x = centerX + rand() % 100 - 50;
         int y = centerY + rand() % 100 - 50;
         int width = 50 + rand() % 150;
         int height = 40 + rand() % 100;
         fillellipse(x, y, width, height);
     }
     
     // Draw blue nebula cloud
     setcolor(BLUE);
     setfillstyle(SOLID_FILL, BLUE);
     for (int i = 0; i < 5; i++) {
         int x = centerX + rand() % 120 - 60;
         int y = centerY + rand() % 120 - 60;
         int width = 60 + rand() % 130;
         int height = 30 + rand() % 90;
         fillellipse(x, y, width, height);
     }
     
     // Draw cyan nebula cloud
     setcolor(CYAN);
     setfillstyle(SOLID_FILL, CYAN);
     for (int i = 0; i < 4; i++) {
         int x = centerX + rand() % 150 - 75;
         int y = centerY + rand() % 150 - 75;
         int width = 40 + rand() % 100;
         int height = 30 + rand() % 80;
         fillellipse(x, y, width, height);
     }
     
     // Reset write mode
     setwritemode(COPY_PUT);
 }