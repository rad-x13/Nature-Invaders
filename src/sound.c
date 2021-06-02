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

#include "sound.h"

static void loadSounds(void);

static Mix_Chunk *sounds[SND_MAX];
static Mix_Music *music;

void initSounds(void)
{
	memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);
	
	music = NULL;
	
	loadSounds();
}

void loadMusic(char *filename)
{
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}

	music = Mix_LoadMUS(filename);
}

void playMusic(int loop)
{
	Mix_PlayMusic(music, (loop) ? -1 : 0);
}

void playSound(int id, int channel)
{
	Mix_PlayChannel(channel, sounds[id], 0);
}

static void loadSounds(void)
{
	sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("sound/425209__velkstar__water-drop.wav");
	sounds[SND_ALIEN_FIRE] = Mix_LoadWAV("sound/468852__christianand__26-pestaneo-ruidoso.wav");
	sounds[SND_PLAYER_DIE] = Mix_LoadWAV("sound/541887__d4xx__pop-up-sound.ogg");
	sounds[SND_ALIEN_DIE] = Mix_LoadWAV("sound/396270__eflexmusic__exploding-car-with-fire-mixed.ogg");
}
