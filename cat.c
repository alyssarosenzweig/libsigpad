#include <stdio.h>
#include "stext.h"

void main() {
    if(init_sigpad()) return;
    initializeGlyphs();

    char buffer[40];
    
    int cursorX = 8, cursorY = 8;

    while(fgets(buffer, sizeof(buffer), stdin)) {
        if(cursorY > 240) {
            clear();
            cursorY = 8;
        }
        
        buffer[strlen(buffer) - 1] = '\0';
        renderString(buffer, cursorX, cursorY, 12);

        cursorY += 12;
    }
}
