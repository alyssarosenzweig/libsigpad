#include "stext.h"

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library ft;
FT_Face face;

void initializeGlyphs() {
    if(FT_Init_FreeType(&ft)) printf("ft init error\n");
    if(FT_New_Face(ft, "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf", 0, &face)) printf("ft glyph error\n");
}

void renderString(char* str, int x, int y, int size) {
    FT_Set_Pixel_Sizes(face, 0, 52);
    while(*str != '\0') {
        printf("(%d, %d)\n", x, y);
        FT_Load_Char(face, *str++, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
        printf("%d|%d|\n", face->glyph->bitmap.width, face->glyph->bitmap.rows);

        FT_Bitmap bmp = face->glyph->bitmap;
        bitmap(x, y, bmp.width, bmp.rows, bmp.buffer);

        x += face->glyph->advance.x >> 6;
        y += face->glyph->advance.y >> 6;
    }
}
