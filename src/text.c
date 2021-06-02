/*
    Copyright (C) 2021 Vincent Radé
    Copyright (C) 2015-2018 Parallel Realities

    Nature Invaders is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Nature Invaders is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Nature Invaders. If not, see <https://www.gnu.org/licenses/>.

*/

#include "text.h"


static SDL_Texture *fontTexture;
static char drawTextBuffer[MAX_LINE_LENGTH];

// Initialize fonts.
// The font of the game is create from a bitmap.
void initFonts(void)
{
	fontTexture = loadTexture("gfx/font.png");
}

// Draw the text.
// The text is drew by select the right spot on the font bitmap
// according to the letter enter by the user.
// The text can also be align to the right, at the center or to the left by default.
void drawText(int x, int y, int r, int g, int b, int align, char *format, ...)
{
	int i, len, c;
        SDL_Rect rect;
        va_list args;

        memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

        // Use variable arguments function to manage text format
        va_start(args, format);
        vsprintf(drawTextBuffer, format, args);
        va_end(args);

        len = strlen(drawTextBuffer);

        switch (align)
        {
        case TEXT_RIGHT:
                x -= (len * GLYPH_WIDTH);
                break;
        case TEXT_CENTER:
                x -= (len * GLYPH_WIDTH) / 2;
                break;
        }

        // Define the width and the height of the rectangle part to use in the bitmap font 
        rect.w = GLYPH_WIDTH;
        rect.h = GLYPH_HEIGHT;
        rect.y = 0;

        SDL_SetTextureColorMod(fontTexture, r, g, b);

        // Define the place to put the rectangle on the bitmap font to get the right letter
        for (i = 0; i < len; i++)
        {
                c = drawTextBuffer[i];

                if (c >= ' ' && c <= 'Z')
                {
                        rect.x = (c - ' ') * GLYPH_WIDTH;

                        blitRect(fontTexture, &rect, x, y);

                        x += GLYPH_WIDTH;
                }
        }
}
