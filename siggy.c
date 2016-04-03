#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "hid.h"

#define MAX_SIZE 0x20

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

// 320x240 resolution

// HYPOTHESIS: ANY FIRST BYTE VALUE > 0x80 WORKS
// Therefore, the first byte is a command byte.

unsigned char maybeShutOff[] = {
    0x81, 0x03
};

unsigned char maybeTurnOn[] = {
    0x81, 0x02
};

// this function does not take more than 32 bytes
// length checking must be done in caller
void sendBitmapRaw(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, void* data) {
    printf("%d,%d x %d,%d\n", xpos, ypos, width, height);

    unsigned char buffer[32] = {
        0xFF, 0x07, // command
        0x02, // mode
        0x00, 0x00, 0x00, 0x00, // poses
        0x00, 0x00, 0x00, 0x00, // heights
        0x00, 0x00, 0x00, 0x00, // bitmap content
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00
    };

    // TODO: non-little endian machines
    buffer[3] = (xpos & 0xFF00) >> 8;
    buffer[4] = (xpos & 0x00FF);
    buffer[5] = (ypos & 0xFF00) >> 8;
    buffer[6] = (ypos & 0x00FF);
    buffer[7] = (width & 0xFF00) >> 8;
    buffer[8] = (width & 0x00FF);
    buffer[9] = (height & 0xFF00) >> 8;
    buffer[10] = (height & 0x00FF);

    memcpy(buffer + 11, data, width * height >> 3);
}

// in charge of segmentation..

void sendBitmap(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, void* data) {
    for(int y = 0; y < height; y += 16) {
        int h = height - y > 16 ? 16 : height - y;

        for(int x = 0; x < width; x += 8) {
            sendBitmapRaw(
                    xpos + x, ypos + y,
                    8, h,
                    data + (y * width >> 3) + (x >> 3)
                );

            printf("Blit\n");
            sleep(1);
        }
    }
}

unsigned char maybeSetMode[] = {
    0xFF, 0x09, // (guess) command to set mode
    0x02 // mode
};

void sendRandomPacket() {
    unsigned char buffer[64];
    
    // randomly initialize start of buffer;
    // use magic number
    buffer[0] = 0xFF;
    for(int i = 1; i < sizeof(buffer); ++i) {
        buffer[i] = i > 11 ? 0xCC : rand() & 0xFF;
        printf("0x%02X, ", buffer[i]);
    }

    putchar('\n');

    //s send

    rawhid_send(0, &buffer, sizeof(buffer), 64);
}

void paint(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint8_t mode) {
    unsigned char buffer[11] = {
        0xFF, 0x12, // command
        mode, // mode
        0x00, 0x00, 0x00, 0x00, // poses
        0x00, 0x00, 0x00, 0x00 // heights
    };

    // TODO: non-little endian machines
    buffer[3] = (xpos & 0xFF00) >> 8;
    buffer[4] = (xpos & 0x00FF);
    buffer[5] = (ypos & 0xFF00) >> 8;
    buffer[6] = (ypos & 0x00FF);
    buffer[7] = (width & 0xFF00) >> 8;
    buffer[8] = (width & 0x00FF);
    buffer[9] = (height & 0xFF00) >> 8;
    buffer[10] = (height & 0x00FF);

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

    //rawhid_send(0, maybeTurnOn, 2, 16);

    //char charA[] = { 0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x00, 0x00 };

    /*for(int x = 0; x < 40; ++x) {
        for(int y = 0; y < 30; ++y) {
            sendBitmap(x * 8, y * 8, 8, 8, charA);
            usleep(100000);
        }
    }*/

    paint(129, 129, 128, 128, 1);
    

    rawhid_close(0);

    return 0;
}
