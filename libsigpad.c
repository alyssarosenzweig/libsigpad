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

int init_sigpad() {
    if(rawhid_open(10, 0x06A8, 0x0043, -1, -1) != 1) return -1;

    clear();
    setBacklight(false);
    sleep(1);
    setBacklight(true);

    return 0;
}

void clear() { 
    rectangle(0, 0, 320, 240, 2);
    usleep(1000000);
}

static inline void send_packet(uint8_t* packet, size_t length) {
    rawhid_send(0, packet, length, 4);
}

void bitmap(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint8_t* data) {
    uint8_t packet[11 + 8] = { 0xFF, 0x07, 0x02, 0, 0, 0, 0, 0, 8, 0, 8};
    uint8_t* block = packet + 11;

    int awidth = ((width + 7) >> 3) << 3;

    for(int x = 0; x < width; x += 8) {
        for(int y = 0; y < height; y += 8) {
            /* blit 8x8 block */

            memset(block, 0, sizeof(block));
            for(int row = 0; row < 8 && y + row < height; ++row) {
                block[row] = data[(((y + row) * awidth) >> 3) + (x >> 3)];
            }

            packet[3] = ((xpos + x) & 0xFF00) >> 8;
            packet[4] = ((xpos + x) & 0xFF);
            packet[5] = ((ypos + y) & 0xFF00) >> 8;
            packet[6] = ((ypos + y) & 0xFF);

            send_packet(packet, sizeof(packet));
        }
    }
}

void rectangle(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint8_t mode) {
    uint8_t packet[11] = {
        0xFF, 0x12, mode,

        (xpos   & 0xFF00) >> 8, xpos   & 0x00FF,
        (ypos   & 0xFF00) >> 8, ypos   & 0x00FF,

        (width  & 0xFF00) >> 8, width  & 0x00FF,
        (height & 0xFF00) >> 8, height & 0x00FF,
    };

    send_packet(packet, sizeof(packet));
}

void setBacklight(bool on) {
    uint8_t packet[] = { 0xFF, 0x02 | (!on) };
    send_packet(packet, sizeof(packet));
}
