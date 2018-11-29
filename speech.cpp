#include "speech.h"

#include "globals.h"
#include "hardware.h"
#define OceanDark 0x064273
/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();

void draw_speech_bubble() {
    uLCD.background_color(OceanDark);
    uLCD.textbackground_color(OceanDark);
    uLCD.filled_rectangle(0, 75, 127, 100, OceanDark);    
}


void draw_speech_line(const char* line, int which){
    uLCD.locate(1, 10+which);      // Move cursor
    uLCD.color(WHITE);
    uLCD.printf(line);
}

void speech_bubble_wait(){
    GameInputs inputs; 
    int b1;
    while(1) {
        inputs = read_inputs(); 
        b1 = inputs.b1; 
        if(b1 == 0)
            break;
    }
}

void speech(const char* line1, const char* line2)
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    //erase_speech_bubble();
}

