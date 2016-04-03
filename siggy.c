#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "hid.h"

#define TIMEOUT 4

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

// 320x240 resolution

/* response format document notes (hypothesis anyway)
 * not currently used, but might be useful for debugging
 *
 * first byte is always under 0x80 -- that distinguishes send/recv
 * next byte the command -- I've gotten 0x1C a couple of times
 * next few bytes I'm not sure about:
 * 0x1C 0x1D 0x26 0x04 0x1D 0x26 0x04 0x00 0x00 0x00...
 * these are *definitely* structured.. but how?
 */

// HYPOTHESIS: ANY FIRST BYTE VALUE > 0x80 WORKS
// Therefore, the first byte is a command byte.

unsigned char maybeShutOff[] = {
    0x81, 0x03
};

unsigned char maybeTurnOn[] = {
    0x81, 0x02
};

static inline void send_packet(char* buffer, size_t length) {
    rawhid_send(0, buffer, length, TIMEOUT);
    usleep(100);
    
    rawhid_recv(0, buffer, sizeof(buffer), TIMEOUT);
    hexdump(buffer, sizeof(buffer));
    rawhid_send(0, buffer, sizeof(buffer), TIMEOUT);
}

void sendBitmapRaw(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, void* data) {
    printf("%d,%d x %d,%d\n", xpos, ypos, width, height);

    unsigned char header[] = {
        0xF2, 0x07, // command
        0x02, // mode
        0x00, 0x00, 0x00, 0x00, // poses
        0x00, 0x00, 0x00, 0x00 // heights
    };

    // TODO: non-little endian machines
    header[3] = (xpos & 0xFF00) >> 8;
    header[4] = (xpos & 0x00FF);
    header[5] = (ypos & 0xFF00) >> 8;
    header[6] = (ypos & 0x00FF);
    header[7] = (width & 0xFF00) >> 8;
    header[8] = (width & 0x00FF);
    header[9] = (height & 0xFF00) >> 8;
    header[10] = (height & 0x00FF);

    int dataLength = (width * height) >> 3;

    unsigned char* buffer = malloc(sizeof(header) + dataLength);

    memcpy(buffer, header, sizeof(header));
    memcpy(buffer + sizeof(header), data, dataLength);

    send_packet(buffer, sizeof(header) + dataLength);
}

unsigned char maybeSetMode[] = {
    0xFF, 0x09, // (guess) command to set mode
    0x02 // mode
};

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

    send_packet(buffer, sizeof(buffer));
}

void hexdump(unsigned char* buffer, size_t length) {
    for(int i = 0; i < length; ++i) {
        printf("%X ", buffer[i]);
    }
    putchar('\n');
}

int main() {
    int r = rawhid_open(10, 0x06A8, 0x0043, -1, -1);

    if(r != 1) {
        printf("Ahh! signature pad thing not working, hm: %d\n", r);
        return -1;
    }

    char charA[] = { 0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x00, 0x00 };
    char buffer[8];
    
    for(int y = 0; y < 30; ++y) {
        for(int x = 0; x < 40; ++x) {
            sendBitmapRaw(x << 3, y << 3, 8, 8, charA);
        }
   }

    rawhid_close(0);

    return 0;
}
