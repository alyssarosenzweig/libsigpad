// TODO: Alternative sizes

typedef struct {
    unsigned char bitmap[8];
} Glyph;

typedef struct {
    Glyph table[256];
} Font;

void initializeGlyphs();
