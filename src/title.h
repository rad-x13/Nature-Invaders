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

#include "common.h"

extern void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);
extern void doBackground(void);
extern void drawBackground(void);
extern void drawText(int x, int y, int r, int g, int b, int align, char *format, ...);
extern void initHighscores(void);
extern void initStage(void);
extern SDL_Texture *loadTexture(char *filename);

extern App app;
