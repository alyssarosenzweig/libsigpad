extern Glyph c1, c2, c3, c4, c5, c6, c7, c8, c9, cColon, cUnknown;

void scale(unsigned char* g, int scalar) {
    unsigned char* out = malloc(8 * scalar);
    memset(out, 0, 8 * scalar);

    int index = 0;
    int bIndex = 7;

    for(int y = 0; y < 8; ++y) {
        for(int j = 0; j < scalar; ++j) {
            for(int x = 0; x < 8; ++x) {
                for(int i = 0; i < scalar; ++y) {
                    out[index] |= (g[y] & (1 << x) != 0) << bIndex;
                    bIndex--;
                    
                    if(bIndex < 0) {
                        bIndex = 7;
                        index++;
                    }
                }
            }
        }
    }
}

void renderGlyph(char c, int x, int y, int size) {
    int scalar = size >> 3; // only multiples of 8 work ATM

    Glyph g = cUnknown;

    switch(c) {
        case '1': g = c1; break;
        case '2': g = c2; break;
        case '3': g = c3; break;
        case '4': g = c4; break;
        case '5': g = c5; break;
        case '6': g = c6; break;
        case '7': g = c7; break;
        case '8': g = c8; break;
        case '9': g = c9; break;
        case ':': g = cColon; break;
    }

    unsigned char* bitmap = scale(g.bitmap, scalar);

    bitmap(x, y, size, size, bitmap);

    free(bitmap);
}
