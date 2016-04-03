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

unsigned int pingTimer = PING_PACKET_COUNT;
uint8_t pingBuffer[16];

// TODO: support big-endian machines

static inline void send_packet(unsigned char* buffer, size_t length) {
    rawhid_send(0, buffer, length, TIMEOUT);

    if(pingTimer-- == 0) {
        rawhid_recv(0, pingBuffer, sizeof(pingBuffer), TIMEOUT);
        rawhid_send(0, pingBuffer, sizeof(pingBuffer), TIMEOUT);
        pingTimer = PING_PACKET_COUNT;
    }
}

void bitmap(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, void* data) {
    int dataLength = (width * height) >> 3;

    unsigned char* buffer = malloc(11 + dataLength);
    buffer[0] = 0xF2; // command bytes
    buffer[1] = 0x07;
    buffer[2] = 0x02; // mode
    buffer[3] = (xpos & 0xFF00) >> 8;
    buffer[4] = (xpos & 0x00FF);
    buffer[5] = (ypos & 0xFF00) >> 8;
    buffer[6] = (ypos & 0x00FF);
    buffer[7] = (width & 0xFF00) >> 8;
    buffer[8] = (width & 0x00FF);
    buffer[9] = (height & 0xFF00) >> 8;
    buffer[10] = (height & 0x00FF);

    memcpy(buffer + 11, data, dataLength);
    send_packet(buffer, dataLength + 11);

    free(buffer);
}

void rectangle(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint8_t mode) {
    unsigned char buffer[11] = { 0xFF, 0x12, mode };

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

void clear() { 
    rectangle(0, 0, 320, 240, 2);
    usleep(1000000);
}

void backlightControl(bool on) {
    unsigned char buffer[] = { 0x81, 0x02 | (!on) };
    send_packet(buffer, 2);
}

void backlightOn() { backlightControl(1); }
void backlightOff() { backlightControl(0); }

int main() {
    int status = rawhid_open(10, 0x06A8, 0x0043, -1, -1); 

    if(status != 1) {
        printf("Opening error: %d\n", status);
        return -1;
    }

    clear();

    // test program -- fill screen with A's
    char charA[] = { 0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x00, 0x00 };

    for(int y = 0; y < 30; ++y) {
        for(int x = 0; x < 40; ++x) {
            bitmap(x << 3, y << 3, 8, 8, charA);
        }
   }

    rawhid_close(0);

    return 0;
}
