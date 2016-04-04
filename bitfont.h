// TODO: Alternative sizes

typedef struct {
    Glyph table[256];
} Font;

typedef struct {
    unsigned char bitmap[8];
} Glyph;
