#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdint.h>
#include <stdbool.h>

#include "hid.h"

#define PING_PACKET_COUNT 16
#define TIMEOUT 4

// Call this before using any other functions.
int init_sigpad();

// Blits a monochrome bitmap.
// Must fall on 8x8 boundaries

void bitmap(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, void* data);

// Lower-level tool for drawing rectangles efficiently.
// Set mode to 2 to use it to fill a rectangle.

void rectangle(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint8_t mode);

// Clears display.
void clear();

// Controls backlight state. 
// The latter two methods are for convenience only.

void backlightControl(bool on);
void backlightOn();
void backlightOff();

// low-level command to force the display to pong
void display_pong();
