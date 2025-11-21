/*
 * C++ Graphics Demo using conio.h and graphics.h
 * This program demonstrates various graphics primitives and animations
 * using the BGI graphics library.
 */

 #include <iostream>
 #include <conio.h>
 #include <graphics.h>
 #include <dos.h>
 #include <stdlib.h>
 #include <math.h>
 
 // Function prototypes
 void drawColorfulPattern();
 void bounceCircle();
 void drawStarfield();
 void drawShapes();
 
 int main() {
     // Initialize graphics mode
     int gd = DETECT, gm;
     initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
     
     // Check if graphics initialization was successful
     int errorcode = graphresult();
     if (errorcode != grOk) {
         std::cout << "Graphics error: " << grapherrormsg(errorcode) << std::endl;
         getch(); // Wait for keypress
         return 1;
     }
     
     // Clear screen and set background color
     setbkcolor(BLACK);
     cleardevice();
     
     // Display welcome message
     settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
     setcolor(YELLOW);
     outtextxy(getmaxx()/2 - 170, 20, "C++ GRAPHICS DEMONSTRATION");
     
     // Draw some basic shapes
     drawShapes();
     getch(); // Wait for keypress
     cleardevice();
     
     // Display animated color pattern
     drawColorfulPattern();
     getch(); // Wait for keypress
     cleardevice();
     
     // Display bouncing circle animation
     bounceCircle();
     getch(); // Wait for keypress
     cleardevice();
     
     // Display starfield animation
     drawStarfield();
     getch(); // Wait for keypress
     
     // Close graphics mode
     closegraph();
     return 0;
 }
 
 void drawShapes() {
     // Draw a rectangle
     setcolor(RED);
     rectangle(100, 100, 300, 200);
     outtextxy(130, 215, "Rectangle");
     
     // Draw a circle
     setcolor(GREEN);
     circle(400, 150, 50);
     outtextxy(380, 215, "Circle");
     
     // Draw an ellipse
     setcolor(BLUE);
     ellipse(150, 300, 0, 360, 70, 40);
     outtextxy(130, 350, "Ellipse");
     
     // Draw a polygon (hexagon)
     setcolor(CYAN);
     int poly[14] = {400, 300, 450, 275, 450, 325, 400, 350, 350, 325, 350, 275, 400, 300};
     drawpoly(7, poly);
     outtextxy(380, 350, "Polygon");
     
     // Draw a filled shape
     setcolor(MAGENTA);
     setfillstyle(SOLID_FILL, MAGENTA);
     fillellipse(getmaxx()/2, getmaxy()-100, 80, 40);
     outtextxy(getmaxx()/2 - 40, getmaxy()-50, "Filled Ellipse");
 }
 
 void drawColorfulPattern() {
     // Display title
     settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
     setcolor(WHITE);
     outtextxy(getmaxx()/2 - 150, 20, "COLORFUL PATTERN");
     
     // Draw a colorful pattern
     for (int i = 0; i < 15; i++) {
         setcolor(i % 15 + 1);
         setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
         
         // Draw concentric shapes
         rectangle(getmaxx()/2 - 10*i, getmaxy()/2 - 7*i, 
                   getmaxx()/2 + 10*i, getmaxy()/2 + 7*i);
                   
         circle(getmaxx()/2, getmaxy()/2, 5*i);
         
         // Add small delay for animation effect
         delay(100);
     }
 }
 
 void bounceCircle() {
     // Display title
     settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
     setcolor(WHITE);
     outtextxy(getmaxx()/2 - 130, 20, "BOUNCING CIRCLE");
     
     int x = 100, y = 100;
     int dx = 5, dy = 5;
     int radius = 30;
     
     // Animation loop
     for (int i = 0; i < 200; i++) {
         // Clear previous position (by redrawing in black)
         setcolor(BLACK);
         setfillstyle(SOLID_FILL, BLACK);
         fillellipse(x, y, radius, radius);
         
         // Update position
         x += dx;
         y += dy;
         
         // Check boundaries and reverse direction if needed
         if (x <= radius || x >= getmaxx() - radius)
             dx = -dx;
         if (y <= radius || y >= getmaxy() - radius)
             dy = -dy;
         
         // Draw circle at new position
         setcolor(YELLOW);
         setfillstyle(SOLID_FILL, RED);
         fillellipse(x, y, radius, radius);
         
         // Small delay for animation
         delay(20);
     }
 }
 
 void drawStarfield() {
     // Display title
     settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
     setcolor(WHITE);
     outtextxy(getmaxx()/2 - 100, 20, "STARFIELD");
     
     // Create stars with random positions
     int stars[100][2];
     for (int i = 0; i < 100; i++) {
         stars[i][0] = rand() % getmaxx();
         stars[i][1] = rand() % getmaxy();
     }
     
     // Animation loop
     for (int frame = 0; frame < 100; frame++) {
         // Draw each star
         for (int i = 0; i < 100; i++) {
             // Clear old star (by drawing in black)
             putpixel(stars[i][0], stars[i][1], BLACK);
             
             // Update position - move toward center
             if (stars[i][0] < getmaxx()/2)
                 stars[i][0]++;
             else if (stars[i][0] > getmaxx()/2)
                 stars[i][0]--;
                 
             if (stars[i][1] < getmaxy()/2)
                 stars[i][1]++;
             else if (stars[i][1] > getmaxy()/2)
                 stars[i][1]--;
                 
             // Randomly reset stars that reach center
             if (abs(stars[i][0] - getmaxx()/2) < 5 && 
                 abs(stars[i][1] - getmaxy()/2) < 5) {
                 stars[i][0] = rand() % getmaxx();
                 stars[i][1] = rand() % getmaxy();
             }
             
             // Draw new star with brightness based on distance to center
             int brightness = abs(stars[i][0] - getmaxx()/2) + 
                             abs(stars[i][1] - getmaxy()/2);
             brightness = brightness / 30;
             int color = WHITE - brightness;
             if (color < DARKGRAY) color = DARKGRAY;
             
             putpixel(stars[i][0], stars[i][1], color);
         }
         
         // Small delay for animation
         delay(30);
     }
 }