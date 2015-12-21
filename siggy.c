#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "hid.h"

int main() {
    int r = rawhid_open(10, 0x06A8, 0x0043, -1, -1);

    if(r != 1) {
        printf("Ahh! signature pad thing not working, hm: %d\n", r);
        return -1;
    }

    rawhid_close(0);

    return 0;
}
