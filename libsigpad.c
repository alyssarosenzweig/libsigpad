/**
 * libigpad.c
 * Graphics driver for certain signature pads.
 *
 * Copyright (C) 2016 Alyssa Rosenzweig
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "libsigpad.h"

unsigned int pingTimer = PING_PACKET_COUNT;
uint8_t pingBuffer[16];

uint8_t framebuffer[320 * 240 / 8];

// TODO: support big-endian machines

int init_sigpad() {
    int status = rawhid_open(10, 0x06A8, 0x0043, -1, -1); 

    if(status != 1) {
        printf("Opening error: %d\n", status);
        return -1;
    }

    memset(framebuffer, 0, sizeof(framebuffer));

    clear();
    return 0;
}

static inline void send_packet(unsigned char* buffer, size_t length) {
    rawhid_send(0, buffer, length, TIMEOUT);
}

void bitmapRaw(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, void* data) {
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

void bitmap(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint8_t* data) {
    unsigned char scrap[8];

    for(int x = 0; x < width; x += 8) {
        for(int y = 0; y < width; y += 8) {
            // copy, row by row, 8 pixels of data

            for(int i = 0; i < 8; ++i) {
                // i is the row
                // y is the beginning
                // so (y+i) is the current y coord
                int tY = y + i;

                // x is the current x coordinate
                // the subblock is implicity
                int tX = x;

                // the address, as usual, is y*width + x
                int addr = tY * (width >> 3) + (tX >> 3);

                // except, here that address is 8x too big
                scrap[i] = data[addr];
            }

            bitmapRaw(
                    xpos + x, ypos + y,
                    8, 8,
                    scrap
                );
        }
    }
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
