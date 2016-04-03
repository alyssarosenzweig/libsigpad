#include "libsigpad.h"

// draws a checkerboard

int main() {
    //char charA[] = { 0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x00, 0x00 };
    init_sigpad();

    char buffer[9600];
    memset(buffer, 0x99, 9600);

    bitmap(0, 0, 320, 240, buffer);

    rawhid_close(0);

    return 0;
}
