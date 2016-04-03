#include "libsigpad.h"

int main() {
    int status = rawhid_open(10, 0x06A8, 0x0043, -1, -1); 

    if(status != 1) {
        printf("Opening error: %d\n", status);
        return -1;
    }

    clear();

    // test program -- fill screen with A's
    char charA[] = { 0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x00, 0x00 };

    for(int y = 0; y < 30; ++y) {
        for(int x = 0; x < 40; ++x) {
            bitmap(x << 3, y << 3, 8, 8, charA);
        }
   }

    rawhid_close(0);

    return 0;
}
