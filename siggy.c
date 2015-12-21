#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "hid.h"

void sendRandomPacket() {
    char buffer[64];
    
    // randomly initialize buffer
    for(int i = 0; i < sizeof(buffer); ++i) {
        buffer[i] = rand() & 0xFF;
    }

    //s send

    rawhid_send(0, &buffer, 64, 64);
}

int main() {
    int r = rawhid_open(10, 0x06A8, 0x0043, -1, -1);

    if(r != 1) {
        printf("Ahh! signature pad thing not working, hm: %d\n", r);
        return -1;
    }

    srand(time(NULL));

    // let's fuzz this thing
    // random packet?

    for(;;) {
        sendRandomPacket();
    }

    rawhid_close(0);

    return 0;
}
