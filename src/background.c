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

#include "background.h"

static int backgroundY;
static SDL_Texture *background;

void initBackground(void)
{       
        background = loadTexture("gfx/background.png");
        backgroundY = 0;
}

void doBackground(void)
{
        // Update the background.
        // Move the background from top to bottom screen and repeat.
        if (--backgroundY < -SCREEN_HEIGHT)
        {
                backgroundY = 0;
        }
}

void drawBackground(void)
{
        SDL_Rect dest;
        int y;

        // Display the background.
        // Draw the background without dispruption on the screen.
        for (y = backgroundY; y < SCREEN_HEIGHT; y += SCREEN_HEIGHT)
        {
                dest.x = 0;
                dest.y = y;
                dest.w = SCREEN_WIDTH;
                dest.h = SCREEN_HEIGHT;

                SDL_RenderCopy(app.renderer, background, NULL, &dest);
        }
}



