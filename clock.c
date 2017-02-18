#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "stext.h"

void drawTime() {
    char str[32];
    time_t t = time(NULL);
    struct tm* tmp = localtime(&t);

    strftime(str, 31, "%X", tmp);
    renderString(str, 16, 16, 64);

    strftime(str, 31, "%x", tmp);
    renderString(str, 16, 256 - 48, 32);
}

int main(int argc, char** argv) {
    if(init_sigpad()) return -1;

    initializeGlyphs();

    for(;;) {
        drawTime();
    }

    rawhid_close(0);
    return 0;
}
