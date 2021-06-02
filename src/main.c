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

#include "main.h"

static void capFrameRate(long *then, float *remainder);

int main(int args, char *argv[])
{
	long then;
	float remainder;
        
	memset(&app, 0, sizeof(App));
	
	app.textureTail = &app.textureHead;
        
	initSDL();

        // Call 'cleanup' function when the program terminates
	atexit(cleanup);

	initGame();
  
	initTitle();
	
	then = SDL_GetTicks();

	remainder = 0;

        // Main loop that stop when the user quit the game
	while(1)
	{
		prepareScene();

                doInput(); 

		app.delegate.logic();

		app.delegate.draw();
                
		presentScene();
		
		capFrameRate(&then, & remainder);
	}
  
	return 0;
}

// Keep the frame rate to 60Hz - 16.667ms
static void capFrameRate(long *then, float *remainder)
{
	long wait, frameTime;

	wait = 16 + *remainder;

	*remainder -= (int)*remainder;

	frameTime = SDL_GetTicks() - *then;

	wait -= frameTime;

	if(wait < 1) {
		wait = 1;
	}

	SDL_Delay(wait);

	*remainder += 0.667;

	*then = SDL_GetTicks();
}
