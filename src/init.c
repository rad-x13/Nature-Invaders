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

#include "init.h"

void initSDL(void)
{
	int rendererFlags, windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED;

	windowFlags = 0;

        // Initialize the SDL library with the video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDl: %s\n", SDL_GetError());
		exit(1);
	}

        // Initialize audio
        // Set up audio with SDL mixer, with the following settings
        // Use CD quality frequency, default format, stereo, and 1024 bytes per output sample. 
        // See: https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer_11.html 
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        {
                printf("Couldn't initialize SDL Mixer\n");
		exit(1);
        }

        Mix_AllocateChannels(MAX_SND_CHANNELS);

	app.window = SDL_CreateWindow("Nature Invader", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

	if(!app.window)
	{
 		printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
       	        exit(1);
	}
			
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

       	if(!app.renderer)
	{
 		printf("Failed to create renderer: %s\n", SDL_GetError());
       	        exit(1);
	}

	IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);

	SDL_ShowCursor(0);
}

void initGame(void)
{
        initBackground();

        initSounds();

        initFonts();

        initHighscoreTable();

        loadMusic("music/music.ogg");

        playMusic(1);
}

void cleanup(void)
{
	SDL_DestroyRenderer(app.renderer);

	SDL_DestroyWindow(app.window);

	SDL_Quit();
}

