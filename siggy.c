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
    0xCE, 0x03
};

unsigned char maybeTurnOn[] = {
    0xD5, 0x02
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
        sendTestPacket(maybeShutOff, 0, 2);
        sleep(1);
        sendTestPacket(maybeTurnOn, 0, 2);
        sleep(1);
    }
    
    rawhid_close(0);

    return 0;
}
