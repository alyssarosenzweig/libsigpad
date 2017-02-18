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

// TODO: support big-endian machines

int init_sigpad() {
    if(rawhid_open(10, 0x06A8, 0x0043, -1, -1) != 1) return -1;

    clear();
    return 0;
}

static inline void send_packet(unsigned char* packet, size_t length) {
    rawhid_send(0, packet, length, TIMEOUT);
}

void bitmapBlock(uint16_t xpos, uint16_t ypos, void* data) {
    unsigned char packet[11 + 8];
    packet[0] = 0xF2; // command bytes
    packet[1] = 0x07;
    packet[2] = 0x02; // mode
    packet[3] = (xpos & 0xFF00) >> 8;
    packet[4] = (xpos & 0x00FF);
    packet[5] = (ypos & 0xFF00) >> 8;
    packet[6] = (ypos & 0x00FF);
    packet[7] = 0;
    packet[8] = 8;
    packet[9] = 0;
    packet[10] = 8;

    memcpy(packet + 11, data, 8);
    send_packet(packet, sizeof(packet));
}

void bitmap(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint8_t* data) {
    unsigned char block[8];

    int awidth = ((width + 7) >> 3) << 3;

    for(int x = 0; x < width; x += 8) {
        for(int y = 0; y < height; y += 8) {
            /* blit 8x8 block */

            memset(block, 0, sizeof(block));
            for(int row = 0; row < 8 && y + row < height; ++row) {
                block[row] = data[(((y + row) * awidth) >> 3) + (x >> 3)];
            }

            bitmapBlock(xpos + x, ypos + y, block);
        }
    }
}

void rectangle(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint8_t mode) {
    unsigned char packet[11] = { 0xFF, 0x12, mode };

    packet[3] = (xpos & 0xFF00) >> 8;
    packet[4] = (xpos & 0x00FF);
    packet[5] = (ypos & 0xFF00) >> 8;
    packet[6] = (ypos & 0x00FF);
    packet[7] = (width & 0xFF00) >> 8;
    packet[8] = (width & 0x00FF);
    packet[9] = (height & 0xFF00) >> 8;
    packet[10] = (height & 0x00FF);

    send_packet(packet, sizeof(packet));
}

void clear() { 
    rectangle(0, 0, 320, 240, 2);
    usleep(1000000);
}

void backlightControl(bool on) {
    unsigned char packet[] = { 0x81, 0x02 | (!on) };
    send_packet(packet, 2);
}

void backlightOn()  { backlightControl(1); }
void backlightOff() { backlightControl(0); }
