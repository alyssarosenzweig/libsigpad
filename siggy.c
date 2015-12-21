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
    0xC7, 0x13, 0xA0, 0x01, 0x40, 0xFF, 0xFF, 0x70
};

unsigned char maybeShutOff[] = {
    0x59, 0xF8, 0x97, 0xE9, 0xD4, 0xCB, 0xD7, 0xB2,
    0x7D, 0x1D, 0x94, 0x45, 0x75, 0x38, 0xE8, 0xCE
    0x03, 0x0E, 0xFA, 0x79, 0xF0, 0x09, 0x7E, 0x12, 
    0x8A, 0xE4, 0x3C, 0x20, 0x80, 0x26, 0x3B, 0xD9,
    0x1E
};

void sendRandomPacket() {
    unsigned char buffer[64];
    
    // randomly initialize buffer
    for(int i = 0; i < sizeof(buffer); ++i) {
        buffer[i] = i > 32 ? 0xFF : rand() & 0xFF;
        printf("0x%02X, ", buffer[i]);
    }

    putchar('\n');

    //s send

    rawhid_send(0, &buffer, sizeof(buffer), 64);
}

void paint() {
    rawhid_send(0, &maybePaint, sizeof(maybePaint), 64);
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

    for(;;) {
        sendRandomPacket();
        //paint();
        sleep(1);
    }

//   sendTestPacket(arr, 0, 16);
//     sendTestPacket(maybeShutOff, 0, 33);
    
    rawhid_close(0);

    return 0;
}
