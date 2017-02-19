#include "stext.h"

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library ft;
FT_Face face;

void initializeGlyphs() {
    if(FT_Init_FreeType(&ft)) printf("ft init error\n");
    if(FT_New_Face(ft, "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", 0, &face)) printf("ft glyph error\n");
}

void renderString(char* str, int x, int y, int size) {
    FT_Set_Pixel_Sizes(face, 0, size);
    
    int prev = 0;

    while(*str != '\0') {
        int glyph = FT_Get_Char_Index(face, *str++);

        if(FT_HAS_KERNING(face) && glyph) {
            FT_Vector kerning;
            FT_Get_Kerning(face, prev, glyph, FT_KERNING_DEFAULT, &kerning);
            x += kerning.x >> 6;
        }

        FT_Load_Glyph(face, glyph, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
        FT_Bitmap bmp = face->glyph->bitmap;

        bitmap(x, y, ((bmp.width + 15) >> 4 << 4) , bmp.rows, bmp.buffer);

        x += face->glyph->advance.x >> 6;
        y += face->glyph->advance.y >> 6;

        prev = glyph;
    }
}
