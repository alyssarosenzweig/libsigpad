#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "hid.h"

unsigned char arr[] = {
    0xF7, 0x1C, 0x0A, 0x7E, 0xBA, 0x12, 0x8F, 0x4C,
    0xC7, 0x13, 0x85, 0x84, 0x48, 0xBF, 0xCA, 0x70
};

void sendRandomPacket() {
    unsigned char buffer[32];
    
    // randomly initialize buffer
    for(int i = 0; i < sizeof(buffer); ++i) {
        buffer[i] = rand() & 0xFF;
        printf("0x%02X, ", buffer[i]);
    }

    putchar('\n');

    //s send

    rawhid_send(0, &buffer, sizeof(buffer), 64);
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

    sendTestPacket(arr, 0, 16);
    
    rawhid_close(0);

    return 0;
}
