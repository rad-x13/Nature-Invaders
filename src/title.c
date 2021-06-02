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

#include "title.h"

static void logic(void);
static void draw(void);
static void drawLogo(void);

static SDL_Texture *titleTexture;
static int reveal = 0;
static int timeout;

void initTitle(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;
	
	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);
	
	titleTexture = loadTexture("gfx/title.png");
	
	timeout = FPS * 5;
}

static void logic(void)
{
	doBackground();

        // Apply a discovery graphic effect on the logo 
	if (reveal < SCREEN_HEIGHT)
	{
		reveal++;
	}

        // Display alternatively highscore and title screens
	if (--timeout <= 0)
	{
		initHighscores();
	}

        // When user press 'Fire' start the game
	if (app.keyboard[SDL_SCANCODE_LCTRL])
	{
		initStage();
	}
}

static void draw(void)
{
	drawBackground();
	
	drawLogo();
	
	if (timeout % 40 < 20)
	{
		drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, TEXT_CENTER, "PRESS FIRE TO PLAY!");
	}
}

static void drawLogo(void)
{
	SDL_Rect r;
	
	r.x = 0;
	r.y = 0;
	
	SDL_QueryTexture(titleTexture, NULL, NULL, &r.w, &r.h);
	
	r.h = MIN(reveal, r.h);
	
	blitRect(titleTexture, &r, (SCREEN_WIDTH / 2) - (r.w / 2), 100);
}
