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

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))
#define STRNCPY(dest, src, n) strncpy(dest, src, n); dest[n - 1] = '\0'

#define SCREEN_WIDTH   720
#define SCREEN_HEIGHT  960

#define MAX_SCORE_NAME_LENGTH  16
#define MAX_NAME_LENGTH        32
#define MAX_LINE_LENGTH        1024

#define FPS 60

#define PLAYER_SPEED        4
#define PLAYER_BULLET_SPEED 5
#define ENEMY_BULLET_SPEED  5

#define MAX_KEYBOARD_KEYS  350

#define ENEMY_ROW 5
#define ENEMY_COL 11

#define MAX_ENEMY_STEP 5

#define HORIZONTAL_POSITION  50
#define VERTICAL_POSITION    200

#define SIDE_PLAYER 0
#define SIDE_ENEMY  1

#define MAX_SND_CHANNELS 8

#define NUM_HIGHSCORES 8

#define GLYPH_HEIGHT 28
#define GLYPH_WIDTH  18

#define RIGHT 1
#define LEFT  -1

#define TRUE  1
#define FALSE 0

enum
{
	CH_ANY = -1,
	CH_PLAYER,
	CH_ALIEN_FIRE,
	CH_POINTS
};

enum
{
	SND_PLAYER_FIRE,
	SND_ALIEN_FIRE,
	SND_PLAYER_DIE,
	SND_ALIEN_DIE,
	SND_POINTS,
	SND_MAX
};
        
enum
{
	TEXT_LEFT,
	TEXT_CENTER,
	TEXT_RIGHT
};
