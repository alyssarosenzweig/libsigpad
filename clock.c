#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "stext.h"

void drawTime() {
    char str[32];
    time_t t = time(NULL);
    struct tm* tmp = localtime(&t);

    strftime(str, 6, "%R", tmp);
    renderString(str, 0, 0, 64);

    strftime(str, 4, ":%S", tmp);
    renderString(str, 224, 64, 32);

    strftime(str, 13, "%x", tmp);
    renderString(str, 320 - (24 * strlen(str)), 216, 24);
}

int main(int argc, char** argv) {
    if(init_sigpad()) return -1;

    initializeGlyphs();
    clear();
    backlightOn();

    for(;;) {
        drawTime();
    }

    rawhid_close(0);
    return 0;
}
