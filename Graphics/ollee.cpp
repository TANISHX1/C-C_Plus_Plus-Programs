/**
 * Simple Stick Figure Animation - One figure patting another's head
 * Uses graphics.h library (Borland Graphics Interface)
 */

 #include <graphics.h>
 #include <stdio.h>
 #include <conio.h>
 #include <math.h>
 #include <dos.h>  // For built-in delay function
 #include <time.h>  // For clock function
 
 // Function prototypes
 void drawStickFigures(int handPos);
 
 int main() {
     int gd = DETECT, gm;
     int handPos = 0; // Position of the hand (0-100)
     int handDirection = 1; // 1: moving toward sad figure, -1: moving away
     
     // Initialize graphics mode
     initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
     
     // Check for errors
     int errorcode = graphresult();
     if (errorcode != grOk) {
         printf("Graphics error: %s\n", grapherrormsg(errorcode));
         printf("Press any key to exit...");
         getch();
         return 1;
     }
     
     // Main animation loop
     while (!kbhit()) {
         // Clear the screen
         cleardevice();
         
         // Update hand position
         handPos += handDirection * 2;
         if (handPos > 100) {
             handDirection = -1;
         } else if (handPos < 0) {
             handDirection = 1;
         }
         
         // Draw the stick figures with updated hand position
         drawStickFigures(handPos);
         
         // Display and delay
         delay(30);  // Using built-in BGI delay function
     }
     
     // Close graphics
     closegraph();
     return 0;
 }
 
 void drawStickFigures(int handPos) {
     // Colors
     int figureColor = WHITE;
     
     // Center of screen
     int centerX = getmaxx() / 2;
     int centerY = getmaxy() / 2;
     
     // Dimensions
     int headRadius = 50;
     int spacing = 120;
     
     // Positions for the two figures
     int sadX = centerX - spacing/2;  // Left figure (sad)
     int sadY = centerY;
     
     int happyX = centerX + spacing/2;  // Right figure (smirking)
     int happyY = centerY;
     
     // Draw sad figure (left)
     setcolor(figureColor);
     
     // Head
     circle(sadX, sadY, headRadius);
     
     // Body (simple line)
     line(sadX, sadY + headRadius, sadX, sadY + headRadius + 50);
     
     // Arms
     line(sadX, sadY + headRadius + 20, sadX - 30, sadY + headRadius + 40);  // Left arm
     line(sadX, sadY + headRadius + 20, sadX + 30, sadY + headRadius + 40);  // Right arm
     
     // Sad face features - drawn as in reference image
     // Sad eyebrows
     line(sadX - 20, sadY - 15, sadX - 10, sadY - 20);  // Left eyebrow
     line(sadX + 10, sadY - 20, sadX + 20, sadY - 15);  // Right eyebrow
     
     // Sad eyes (dots)
     fillellipse(sadX - 15, sadY - 5, 2, 2);  // Left eye
     fillellipse(sadX + 15, sadY - 5, 2, 2);  // Right eye
     
     // Sad mouth (wavy line)
     int mouthY = sadY + 15;
     int mouthWidth = 25;
     
     // Draw wavy mouth using small line segments
     moveto(sadX - mouthWidth/2, mouthY);
     lineto(sadX - mouthWidth/4, mouthY + 5);
     lineto(sadX, mouthY);
     lineto(sadX + mouthWidth/4, mouthY + 5);
     lineto(sadX + mouthWidth/2, mouthY);
     
     // Draw happy figure (right)
     // Head
     circle(happyX, happyY, headRadius);
     
     // Body
     line(happyX, happyY + headRadius, happyX, happyY + headRadius + 50);
     
     // Right arm
     line(happyX, happyY + headRadius + 20, happyX + 30, happyY + headRadius + 40);
     
     // Left arm with animation - moves to pat the sad figure
     // Calculate position based on handPos (0-100)
     float patProgress = handPos / 100.0;
     int armStartX = happyX;
     int armStartY = happyY + headRadius + 20;
     
     // Target is the sad figure's head
     int targetX = sadX;
     int targetY = sadY - headRadius/2;  // Slightly above the center of head
     
     // Calculate current position
     int handX = armStartX - (armStartX - targetX) * patProgress;
     int handY = armStartY - (armStartY - targetY) * patProgress;
     
     // Draw animated left arm
     line(armStartX, armStartY, happyX - 20, happyY + headRadius + 10);  // Upper part of arm
     line(happyX - 20, happyY + headRadius + 10, handX, handY);          // Lower part of arm to hand
     
     // Draw hand (small circle)
     fillellipse(handX, handY, 5, 5);
     
     // Happy face features - as in reference image
     // Straight line eyes
     line(happyX - 20, happyY - 10, happyX - 5, happyY - 10);  // Left eye
     line(happyX + 5, happyY - 10, happyX + 20, happyY - 10);  // Right eye
     
     // Smirking mouth (simple curved up line)
     arc(happyX, happyY + 15, 180, 360, 15);
 }