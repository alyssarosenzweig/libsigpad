#include "libsigpad.h"
#include <pthread.h>

// draws a checkerboard

void* pong(void*);

int main() {
    //char charA[] = { 0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x00, 0x00 };
    if(init_sigpad()) return -1;

    char buffer[9600];
    memset(buffer, 0x99, 9600);

    /*for(int y = 0; y < 240; y += 16) {
        for(int x = 0; x < 320; x += 32) {
            bitmap(x, y, 32, 16, buffer);
            display_pong();
        }
    }*/

    bitmap(0, 0, 320, 240, buffer);
    rawhid_close(0);

    return 0;
}
