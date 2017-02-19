#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include <stdbool.h>

#include "hid.h"

// Call this before using any other functions.
int init_sigpad();

// Blits a monochrome bitmap.
void bitmap(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint8_t* data);

// Lower-level tool for drawing rectangles efficiently (fill mode = 2)
void rectangle(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint8_t mode);

// Clears display.
void clear();

// Controls backlight state. 
void setBacklight(bool on);
