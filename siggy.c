#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "hid.h"

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
    0xF7, 0x1C, 0x0A, 0x7E, 0xBA, 0x12, 0x8F, 0x4C,
    0xC7, 0x13, 0xA0, 0x01, 0x40, 0xFF, 0xFF, 0x70
};

unsigned char maybeShutOff[] = {
    0x7D, 0x1D, 0x94, 0x45, 0x75, 0x38, 0xE8, 0xCE,
    0x03, 0x0E, 0xFA, 0x79, 0xF0, 0x09, 0x7E, 0x12, 
};

unsigned char maybeTurnOn[] = {
    0x0B, 0x31, 0x14, 0xFE, 0x55, 0x57, 0x9B, 0x66, 
    0xD5, 0x16, 0x15, 0x02, 0x5D, 0xE5, 0xEA, 0xF9
};

void sendRandomPacket() {
    unsigned char buffer[64];
    
    // randomly initialize start of buffer;
    // use all-on for rest
    for(int i = 0; i < sizeof(buffer); ++i) {
        buffer[i] = i > 32 ? 0xFF : rand() & 0xFF;
        printf("0x%02X, ", buffer[i]);
    }

    putchar('\n');

    //s send

    rawhid_send(0, &buffer, sizeof(buffer), 64);
}

void paint() {
    rawhid_send(0, maybePaint, sizeof(maybePaint), 64);
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

 /*   for(;;) {
        sendRandomPacket();
        sleep(1);
        paint();
        sleep(1);
    }
    
    paint();

    sendTestPacket(arr, 0, 16);*/

    for(;;) {
        sendTestPacket(maybeShutOff, 0, 16);
        sleep(1);
        sendTestPacket(maybeTurnOn, 0, 16);
        sleep(1);
    }
    
    rawhid_close(0);

    return 0;
}
