#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "hid.h"

typedef unsigned short uint16_t;

// 320x240 resolution

unsigned char arr[] = {
    0xF7, 0x1C, 0x0A, 0x7E, 0xBA, 0x12, 0x8F, 0x4C,
    
    0xC7, // also likely control character
    
    0x13, // some sort of control character
          // hacking the last bit causes some change
    
    0xA0, // start offset, maybe? 
    
    0x01, 0x40, // width (wshort)
    
    0xFF, 0xFF, // height, maybe?
    
    0x70
};

unsigned char arr2[] = {
    0x14
};

unsigned char maybePaint[] = {
    0xFF, 0x12, // command to paint screen
    
    0x01, // Mode? Base-4. Upper 6 ignored
    
    0x00, 0x00, // x-position: big endian
    0x00, 0x00, // y-position: big endian

    0x01, 0x40, // width: big endian
    0x00, 0xF0 // height: big endian
};

// HYPOTHESIS: ANY FIRST BYTE VALUE > 0x80 WORKS
// Therefore, the first byte is a command byte.

unsigned char maybeShutOff[] = {
    0x81, 0x03
};

unsigned char maybeTurnOn[] = {
    0x81, 0x02
};

unsigned char maybeBitmap[] = {
    0xFF, 0x07, 0x56, 0x6D, 0x43, 0x2D, 0xC1, 0xB4,
    0x3A, 0xBC, 0x51, 0x02, 0x26, 0x1E, 0x7E, 0x3E,
    0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC
};

void sendRandomPacket() {
    unsigned char buffer[64];
    
    // randomly initialize start of buffer;
    // use magic number
    buffer[0] = 0xFF;
    for(int i = 1; i < sizeof(buffer); ++i) {
        buffer[i] = i > 16 ? 0xCC : rand() & 0xFF;
        printf("0x%02X, ", buffer[i]);
    }

    putchar('\n');

    //s send

    rawhid_send(0, &buffer, sizeof(buffer), 64);
}

void paint(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height) {
    unsigned char buffer[11] = {
        0xFF, 0x12, // command
        0x01, // mode
        0x00, 0x00, 0x00, 0x00, // poses
        0x00, 0x00, 0x00, 0x00 // heights
    };

    // TODO: non-little endian machines
    buffer[3] = xpos & 0xFF00 >> 8;
    buffer[4] = xpos & 0x00FF;
    buffer[5] = ypos & 0xFF00 >> 8;
    buffer[6] = ypos & 0x00FF;
    buffer[7] = width & 0xFF00 >> 8;
    buffer[8] = width & 0x00FF;
    buffer[9] = height & 0xFF00 >> 8;
    buffer[10] = height & 0x00FF;

    rawhid_send(0, buffer, sizeof(buffer), 64);
}

void sendTestPacket(unsigned char* buffer, int offset, int length) {
    rawhid_send(0, buffer + offset, length, 64);
}

int main() {
    int r = rawhid_open(10, 0x06A8, 0x0043, -1, -1);

    if(r != 1) {
        printf("Ahh! signature pad thing not working, hm: %d\n", r);
        return -1;
    }

    srand(time(NULL));

    rawhid_send(0, maybeBitmap, sizeof(maybeBitmap), 64);

    rawhid_close(0);

    return 0;
}
