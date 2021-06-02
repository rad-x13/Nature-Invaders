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

typedef struct App App;
typedef struct Debris Debris;
typedef struct Delegate Delegate;
typedef struct Entity Entity;
typedef struct Explosion Explosion;
typedef struct Highscore Highscore;
typedef struct Highscores Highscores;
typedef struct Stage Stage;
typedef struct Texture Texture;

// Logic and Draw methods are called in the main game loop and
// connect to alternatively to the following views: title, highscores or stage. 
struct Delegate {
	void (*logic)(void);
	void (*draw)(void);
};

struct Texture {
	char name[MAX_NAME_LENGTH];
	SDL_Texture *texture;
	Texture *next;
};

struct App {
	SDL_Renderer *renderer;
	SDL_Window *window;
	Delegate delegate;
	int keyboard[MAX_KEYBOARD_KEYS];
	Texture textureHead, *textureTail;
        char inputText[MAX_LINE_LENGTH];
};

// Entity defines the player, an enemy or bullets.
struct Entity {
	float x;       // Horizontal position on the screen
	float y;       // Vertical position on the screen
	int w;         // Width of the texture
	int h;         // Height of the texture
	float dx;      // Horizontal speed
	float dy;      // Vertical speed
	int health;    // Health of the entity, when it is 0 the enetity is removed
	int reload;    // Weapon reloading
	int side;      // PLAYER_SIDE or ENEMY_SIDE
        int points;    // Enemy points add to score when there are destroy, not used for player
	SDL_Texture *texture;
        Entity *next; // Next element of the linked list 
};

struct Explosion {
        float x;         // Horizontal position on the screen
        float y;         // Vertical position on the screen
        float dx;        // Horizontal speed
        float dy;        // Vertical speed
        int r, g, b, a;  // Color red, green, blue, alpha
        Explosion *next; // Next element of the linked list 
};

struct Debris {
        float x;        // Horizontal position on the screen
        float y;        // Vertical position on the screen
        float dx;       // Horizontal speed
        float dy;       // Vertical speed
        SDL_Rect rect;  // Part of the entity related to the debris
        SDL_Texture *texture;
        int life;       // Life level of the debris, use to remove the debris after a while
        Debris *next;   // Next element of the linked list 
};

struct Stage {
	Entity bulletHead, *bulletTail;          // Bullet linked list
        Explosion explosionHead, *explosionTail; // Explosion linked list
        Debris debrisHead, *debrisTail;          // Debris linked list
        Entity* enemies[ENEMY_ROW][ENEMY_COL];   // Enemies matrix
        int score;                               // Current game score
};

struct Highscore {
        char name[MAX_SCORE_NAME_LENGTH]; // Name of the user who realised the highscore
	int recent;                       // Define if the highscore is new to add it to the table
	int score;                        // Highscore's score
};

struct Highscores {
	Highscore highscore[NUM_HIGHSCORES]; // Highscore array
};
