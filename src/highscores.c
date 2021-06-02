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

#include "highscores.h"

static void doNameInput(void);
static void draw(void);
static void drawHighscores(void);
static void drawNameInput(void);
static int highscoreComparator(const void *a, const void *b);
static void logic(void);

static Highscore *newHighscore;
static int cursorBlink;
static int timeout;

void initHighscoreTable(void)
{
	int i;
	
	memset(&highscores, 0, sizeof(Highscores));

        // Fill highscore table by default.
        // For each highscore, fill score and name with default values row number and "ANONYMOUS".
	for (i = 0 ; i < NUM_HIGHSCORES ; i++)
	{
		highscores.highscore[i].score = NUM_HIGHSCORES - i;
		STRNCPY(highscores.highscore[i].name, "ANONYMOUS", MAX_SCORE_NAME_LENGTH);
	}
	
	newHighscore = NULL;
	
	cursorBlink = 0;
}

void initHighscores(void)
{
        // Add highscore logic and draw functions to delegate pattern.
	app.delegate.logic = logic;
	app.delegate.draw = draw;
	
	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);
	
	timeout = FPS * 5;
}

static void logic(void)
{
	doBackground();

        // Apply highscore logic
        // Check if a new score must be add to the table, then handle the new name input.
        // Else display title screen and then highscore table screen
        // during 'timeout' duration each one, and
        // listen if the 'Fire' key is press by the user to start the game. 
	if (newHighscore != NULL)
	{
		doNameInput();
	}
	else
	{
                if (--timeout <= 0)
                {
                        initTitle();
                }
                
		if (app.keyboard[SDL_SCANCODE_LCTRL])
		{
			initStage();
		}
	}
        
	if (++cursorBlink >= FPS)
	{
		cursorBlink = 0;
	}
}

static void doNameInput(void)
{
	int i, n;
	char c;
	
	n = strlen(newHighscore->name);

        // Get the new name from the user.
        // For each letter enter by the user,
        // convert the letter to an uppercase alphabet,
        // check if the new name keep the wanted lenght with the null character '\0', and
        // check if the letter is a valid character,
        // then increase the new name lenght, and add the letter to the new nane.
	for (i = 0 ; i < strlen(app.inputText) ; i++)
	{
		c = toupper(app.inputText[i]);
                
		if (n < MAX_SCORE_NAME_LENGTH - 1 && c >= ' ' && c <= 'Z')
		{
			newHighscore->name[n++] = c;
		}
	}

        // Erase th last letter in the new name.
        // Check if the new name is not empty, and
        // check if the user press 'backspace' key,
        // then replace the last letter by a null character '/0', and
        // reset the 'backspace' key to zero.
	if (n > 0 && app.keyboard[SDL_SCANCODE_BACKSPACE])
	{
		newHighscore->name[--n] = '\0';
		
		app.keyboard[SDL_SCANCODE_BACKSPACE] = 0;
	}

        // Validate the new name.
        // Check if the user press 'return' key,
        // then reset the new highscore structure to display the highscore table, and
        // check if the new name is empty, then fill the new name with "ANONYMOUS".
        // TODO Handle names with only 'backspace'.
	if (app.keyboard[SDL_SCANCODE_RETURN])
	{
		if (strlen(newHighscore->name) == 0)
		{
			STRNCPY(newHighscore->name, "ANONYMOUS", MAX_SCORE_NAME_LENGTH);
		}
		
		newHighscore = NULL;
	}
}

static void draw(void)
{
	drawBackground();

        // Draw highscore table
        // Check if a new score must be add to the table, then display the new name input.
        // Else display the highscore table, and a blinking instruction text to start the game.
	if (newHighscore != NULL)
	{
		drawNameInput();
	}
	else
	{
		drawHighscores();
		
		if (timeout % 40 < 20)
		{
			drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, TEXT_CENTER, "PRESS FIRE TO PLAY!");
		}
	}
}

static void drawNameInput(void)
{
	SDL_Rect r;

        // Draw instructions text to user for enter his/her name.
	drawText(SCREEN_WIDTH / 2, 30, 255, 255, 255, TEXT_CENTER, "CONGRATULATIONS,");
        drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255, TEXT_CENTER, "YOU'VE GAINED A HIGHSCORE!");
        
	drawText(SCREEN_WIDTH / 2, 120, 255, 255, 255, TEXT_CENTER, "ENTER YOUR NAME BELOW:");
	
	drawText(SCREEN_WIDTH / 2, 250, 128, 255, 128, TEXT_CENTER, newHighscore->name);

        // Draw a green blinking cursor on the name field.
	if (cursorBlink < FPS / 2)
	{
		r.x = ((SCREEN_WIDTH / 2) + (strlen(newHighscore->name) * GLYPH_WIDTH) / 2) + 5;
		r.y = 250;
		r.w = GLYPH_WIDTH;
		r.h = GLYPH_HEIGHT;
		
		SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(app.renderer, &r);
	}
	
	drawText(SCREEN_WIDTH / 2, 625, 255, 255, 255, TEXT_CENTER, "PRESS RETURN WHEN FINISHED");
}

static void drawHighscores(void)
{
	int i, y, r, g, b;
	
	y = 150;
	
	drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255, TEXT_CENTER, "HIGHSCORES");

        // Display saved highscores
        // For each saved highscore, draw a text with the rank, the name, and the score.
        // Check if the highscore is recent, then draw the text in green.
	for (i = 0 ; i < NUM_HIGHSCORES ; i++)
	{
		r = 255;
		g = 255;
		b = 255;
		
		if (highscores.highscore[i].recent)
		{
                        r = 0;
			b = 0;
		}
		
		drawText(SCREEN_WIDTH / 2, y, r, g, b, TEXT_CENTER, "#%d. %-15s ...... %03d", (i + 1), highscores.highscore[i].name, highscores.highscore[i].score);
		
		y += 50;
	}
}

void addHighscore(int score)
{
	Highscore newHighscores[NUM_HIGHSCORES + 1];
	int i;
        
	memset(newHighscores, 0, sizeof(Highscore) * (NUM_HIGHSCORES + 1));

        // Add the new highscore to a highscore table.
        // Fill the highscore table with the current highscores, then
        // add the new highscore to the highscore table at the last spot of the array, and
        // sort the highscore table by increasing order of score.
	for (i = 0 ; i < NUM_HIGHSCORES ; i++)
	{
		newHighscores[i] = highscores.highscore[i];
		newHighscores[i].recent = 0;
	}
	
	newHighscores[NUM_HIGHSCORES].score = score;
	newHighscores[NUM_HIGHSCORES].recent = 1;
	
	qsort(newHighscores, NUM_HIGHSCORES + 1, sizeof(Highscore), highscoreComparator);

        // Get the final highscore table
        // Copy the new highscore table in the current highscore table, then
        // assign the new highscore variable if the recent highscore if in the first
        // eighth position of the array in order to display the name input screen.
	newHighscore = NULL;
        
	for (i = 0 ; i < NUM_HIGHSCORES ; i++)
	{
		highscores.highscore[i] = newHighscores[i];
		
		if (highscores.highscore[i].recent)
		{
			newHighscore = &highscores.highscore[i];
		}
	}
}

static int highscoreComparator(const void *a, const void *b)
{
        // Cast the void variables passed to the generic compar function.
        // See: https://www.cplusplus.com/reference/cstdlib/qsort/
	Highscore *h1 = ((Highscore*)a);
	Highscore *h2 = ((Highscore*)b);

	return h2->score - h1->score;
}
