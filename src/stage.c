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

#include "stage.h"

static void addDebris(Entity *e);
static void addExplosions(int x, int y, int num);
static void assignEnemyPoints(Entity* e, int row);
static void assignEnemyTextrure(Entity* e, int row);
static int bulletHitEnemy(Entity *b);
static int bulletHitPlayer(Entity *b);
static void clipEnemies(void);
static void clipPlayer(void);
static void destroyEnemies(void);
static void doBullets(void);
static void doDebris(void);
static void doEnemies(void);
static void doExplosions(void);
static void doPlayer(void);
static void draw(void);
static void drawBullets(void);
static void drawDebris(void);
static void drawEnemies(void);
static void drawExplosions(void);
static void drawHud(void);
static void drawPlayer(void);
static void fireBullet(void);
static void fireEnemyBullet(Entity *e);
static void initEnemies(void);
static void initPlayer(void);
static void logic(void);
static void moveEnemies(void);
static void resetStage(void);
static void shootPlayer(void);

static Entity *player;
static SDL_Texture *bulletTexture;
static SDL_Texture *enemyBulletTexture;
static SDL_Texture *enemyLargeTexture;
static SDL_Texture *enemyMediumTexture;
static SDL_Texture *enemySmallTexture;
static SDL_Texture *explosionTexture;
static SDL_Texture *playerTexture;
static int enemyStepTimer;

int enemyCurrentStep;     // Current horizontal position of enemies 
int enemyDestroyed;       // Use to trigger the state when all enemy are destroyed : TRUE or FALSE 
int enemyDestroyedNumber; // Current enemy destroyed number
int enemyDirection;       // Define the horizontal enemy direction : RIGHT or LEFT
int enemyMoveDown;        // Define the down move of enemies : TRUE or FALSE
int enemyTotalNumber;     // Total enemy number
int stageResetTimer;      // Timer to reset the game 

// Initialize the game stage.
// by loading textures, initialize entities (player and enemies), and initialize gloabl variables.
void initStage(void)
{
        app.delegate.logic = logic;
	app.delegate.draw = draw;

	bulletTexture = loadTexture("gfx/bullet.png");
        enemyBulletTexture = loadTexture("gfx/enemyBullet.png");
	enemyLargeTexture = loadTexture("gfx/largeEnemy.png");
	enemyMediumTexture = loadTexture("gfx/mediumEnemy.png");
        enemySmallTexture = loadTexture("gfx/smallEnemy.png");
        playerTexture = loadTexture("gfx/player.png");
        explosionTexture = loadTexture("gfx/explosion.png");
	
	memset(app.keyboard, 0 , sizeof(int) * MAX_KEYBOARD_KEYS);

        resetStage();

	initPlayer();
	initEnemies();
        
	enemyStepTimer = FPS;
        
	enemyDirection = RIGHT;

	enemyMoveDown = FALSE;
        enemyDestroyed = FALSE;

        enemyCurrentStep = 0;
        enemyDestroyedNumber = 0;
        enemyTotalNumber = ENEMY_ROW * ENEMY_COL;

        stageResetTimer = FPS * 3;
}

// Reset the stage to initial state.
// by freeing entities, bullets, explosion, debris from the memory,
// resetting the stage object to zero, and initializing liked lists.
static void resetStage()
{
        Entity *e;
        Explosion *ex;
        Debris *d;
        
        int i, j;

        free(player);

        for (i = 0; i < ENEMY_ROW; i++)
	{
                for (j = 0; j < ENEMY_COL; j++)
                {
                        free(stage.enemies[i][j]);
                }
        }

        while (stage.bulletHead.next)
        {
                e = stage.bulletHead.next;
                stage.bulletHead.next = e->next;
                free(e);
        }

        while (stage.explosionHead.next)
        {
                ex = stage.explosionHead.next;
                stage.explosionHead.next = ex->next;
                free(ex);
        }

        while (stage.debrisHead.next)
	{
		d = stage.debrisHead.next;
		stage.debrisHead.next = d->next;
		free(d);
	}
        
        memset(&stage, 0, sizeof(Stage));
        stage.bulletTail = &stage.bulletHead;
        stage.explosionTail = &stage.explosionHead;
        stage.debrisTail = &stage.debrisHead;

        stage.score = 0;
}

// Initialize player entity.
// Allocate memory space, assign position on the screen, query texture parameters, and
// initialize entity properties.
static void initPlayer()
{
	player = malloc(sizeof(Entity));
	memset(player, 0, sizeof(Entity));

	player->x = 100;
	player->y = 800;
	player->texture = playerTexture;
	SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

	player->health = 1;
	player->side = SIDE_PLAYER;
}

// Initialize enemies entity.
// For each enemies, allocate memory space, assign texture, querying texture parameters,
// assigne position on the screen, assign speed, and initializing entity properties.
static void initEnemies()
{
        int i, j;
	
	for (i = 0; i < ENEMY_ROW; i++)
	{
                for (j = 0; j < ENEMY_COL; j++)
                {        
                        Entity *e;

                        e = malloc(sizeof(Entity));
                        memset(e, 0, sizeof(Entity));
                        stage.enemies[i][j] = e;

                        assignEnemyTextrure(e, i);
                        
                        SDL_QueryTexture(e->texture, NULL, NULL, &e->w, &e->h);
                        
                        e->x = HORIZONTAL_POSITION + (e->w + (e->w / 8)) * j;
                        e->y = VERTICAL_POSITION + (e->h + (e->h / 8)) * i;

                        e->dx = e->w / 8;
                        e->dy = e->h / 8;
		
                        e->health = 1;
                        e->side = SIDE_ENEMY;
                        e->reload = FPS * (1 + (rand() % 10));

                        assignEnemyPoints(e, i);
                }
        }	
}

// Assign texture to enemy according to a row number.
// From the top
// row 1   - small enemy texture,
// row 2/3 - medium enemy texture, and
// row 4/5 - large enemy texture.
static void assignEnemyTextrure(Entity* e, int row)
{
        if (row < 1)
        {
                e->texture = enemySmallTexture;
        }
        else if (row < 3)
        {
                e->texture = enemyMediumTexture;
        }
        else
        {
                e->texture = enemyLargeTexture;
        }
}

// Assign points to enemy according to a row number.
// From the top
// row 1   - 30 points for small enemy,
// row 2/3 - 20 points for medium enemy, and
// row 4/5 - 10 points for large enemy.
static void assignEnemyPoints(Entity* e, int row)
{
        if (row == 1)
        {
                e->points = 30;
        }
        else if (row <= 3)
        {
                e->points = 20;
        }
        else
        {
                e->points = 10;
        }
}

// Apply stage logic.
static void logic(void)
{
        doBackground();
        
	doPlayer();

	doEnemies();

	doBullets();

        doExplosions();

        doDebris();

	clipEnemies();
	
	clipPlayer();
        
        // Reset the game stage.
        // When the player or all enemy are destroyed, wait for the reset time,
        // then add the highscore on the table and display the highscore table.
        if ((player == NULL || enemyDestroyed == TRUE) && --stageResetTimer <= 0)
        {
                addHighscore(stage.score);

                initHighscores();
        }
}

// Do player actions.
// Reload bullets, move entity on the right or on the left according to user inputs,
// fire bullet according to user input, destroyed player if its health is zero.
static void doPlayer(void)
{
	if (player != NULL)
	{                
                player->dx = 0;
                                
		if (player->reload > 0)
		{
			player->reload--;
		}

	      	if (app.keyboard[SDL_SCANCODE_LEFT])
		{
			player->dx = -PLAYER_SPEED;
		}

	      	if (app.keyboard[SDL_SCANCODE_RIGHT])
		{
			player->dx = PLAYER_SPEED;
		}

		if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload <= 0)
		{
                        playSound(SND_PLAYER_FIRE, CH_PLAYER);
                        
			fireBullet();
		}
                
                player->x += player->dx;
                player->y += player->dy;

                if (player->health == 0)
		{
                        player = NULL;
                }
	}
}

// Fire player bullet.
// Allocate memory space to the entity, add the entity to the bullet linked list,
// assign the player position to the entity, query texture parameters, and
// initialize entity properties.
static void fireBullet(void)
{
	Entity *bullet;

	bullet = malloc(sizeof(Entity));
	memset(bullet, 0, sizeof(Entity));
	stage.bulletTail->next = bullet;
	stage.bulletTail = bullet;

	bullet->x = player->x;
	bullet->y = player->y;
	bullet->dy = -PLAYER_BULLET_SPEED;

	bullet->texture = bulletTexture;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

	bullet->y += (player->h / 2) - (bullet->h / 2);

        bullet->health = 1;
	bullet->side = SIDE_PLAYER;
	player->reload = 20;
}

// Do bullet actions.
// For each bullet in the linked list, move it with the right speed,
// check if the player is hit, check if an enemy is hit, and
// check if the bullet go out the screen, then
// remove the bullet from the list.
static void doBullets(void)
{
	Entity *b, *prev;

	prev = &stage.bulletHead;
        
	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		b->x += b->dx;
		b->y += b->dy;

		if (bulletHitPlayer(b)
                    || bulletHitEnemy(b)
		    || b->x < -b->w || b->y < -b->h
		    || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT)
		{
			if (b == stage.bulletTail)
			{
				stage.bulletTail = prev;
			}

			prev->next = b->next;
			free(b);
			b = prev;
       		}

		prev = b;
	}
}

// Do actions when the bullet hits the player.
// Check if the player is still alive, and check if the bullet comes from an enemy, and
// check if there is collision between the player and the bullet, then
// update the health property of both player and bullet, add explosions, add debris, and
// play a sound.
static int bulletHitPlayer(Entity *b)
{
        if (player != NULL
            && b->side != SIDE_PLAYER
            && collision(b->x, b->y, b->w, b->h, player->x, player->y, player->w, player->h))
        {
                b->health = 0;
                player->health = 0;

                addExplosions(player->x, player->y, 32);

                addDebris(player);

                playSound(SND_PLAYER_DIE, CH_PLAYER);
                
                return 1;
        }
	return 0;
}

// Do actions when a bullet hits a enemy.
// Check if the bullet come from the player, then
// for each enemy in the matrix, chech if the enemy is alive, and
// check if there is a collision between the enemy and the bullet, then
// update the health property of both enemy and bullet, add explosions, add debris,
// play a sound, and increase the global score with the enemy's points property. 
static int bulletHitEnemy(Entity *b)
{
	Entity *e;
        int i, j;

        if (b->side != SIDE_ENEMY)
        {
                for (i = 0; i < ENEMY_ROW; i++)
                {
                        for (j = 0; j < ENEMY_COL; j++)
                        {
                                e = stage.enemies[i][j];
                                
                                if (e != NULL
                                    && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
                                {                                        
                                        b->health = 0;
                                        e->health = 0;

                                        addExplosions(e->x, e->y, 32);

                                        addDebris(e);

                                        playSound(SND_ALIEN_DIE, CH_ANY);
                                        
                                        stage.score += e->points;
                                        
                                        return 1;
                                }
                        } // Next j
                } // Next i
        }
	return 0;
}

// Do enemies actions.
static void doEnemies(void)
{
        shootPlayer();

        moveEnemies();

        destroyEnemies();
}

// Shoot player.
// for each enemy on the matrix, check if the enemy is alive,
// decrease the enemy's weapon reloading and chech if the weapon reloading is finish, then
// fire a bullet, and play a sound.
static void shootPlayer(void)
{
        int i, j;
        
        for (j = 0; j < ENEMY_COL; j++)
        {        
                for (i = ENEMY_ROW - 1; i >= 0; i--)
                {
                        if (stage.enemies[i][j] != NULL)
                        {
                                if (--stage.enemies[i][j]->reload <= 0)
                                {
                                        playSound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);
                                        
                                        fireEnemyBullet(stage.enemies[i][j]);
                                }
                                break;
                        }
                        
                } // Next i
        } // Next j
}


// Move enemies together on the screen.
// Every step time, move enemies from left to right, then move them down, and then
// move them from right to left, next repeat these actions.
static void moveEnemies(void)
{
        Entity *e;
        int i, j;
        
	if (--enemyStepTimer <= 0)
	{
                for (i = 0; i < ENEMY_ROW; i++)
                {        
                        for (j = 0; j < ENEMY_COL; j++)
                        {
                                e = stage.enemies[i][j];
                                
                                if (e != NULL)
                                {                                
                                        if (enemyMoveDown)
                                        {
                                                e->y += e->dy;
                                        }
                                        else
                                        {
                                                e->x += enemyDirection * e->dx;
                                        }
                                }
                        } // Next j
                } // Next i
                
		if(enemyMoveDown == TRUE)
		{
			enemyMoveDown = FALSE;
		}
	       	else
		{
			enemyCurrentStep++;
		}

                enemyStepTimer = FPS;
	}
}

// Destroy enemies, according to their health level.
// For each enemy in the matrix, check if the enemy is still alive and
// check if its health is finished, then destroy the enemy and
// increase the number of enemy destroyed in order to be able to detect
// when all enemies are destroyed.
// When all enemies are destroyed signal it to reset the game.
static void destroyEnemies(void)
{
        Entity *e;
        int i, j;
        
        for (i = 0; i < ENEMY_ROW; i++)
        {        
                for (j = 0; j < ENEMY_COL; j++)
                {
                        e = stage.enemies[i][j];
                        
                        if (e != NULL && e->health == 0)
                        {
                                stage.enemies[i][j] = NULL;
                                enemyDestroyedNumber++;
                        }
                } // Next j
        } // Next i

        if (enemyDestroyedNumber == enemyTotalNumber)
        {
                enemyDestroyed = TRUE;
        }       
}

// Fire enemy bullet.
// Allocate memory space to the entity, add the entity to the bullet linked list,
// assign the enemy position to the entity, query texture parameters,
// initialize entity properties, and reload enemy's weapon.
static void fireEnemyBullet(Entity *e)
{
        Entity *bullet;

        bullet = malloc(sizeof(Entity));
        memset(bullet, 0, sizeof(Entity));
        stage.bulletTail->next = bullet;
        stage.bulletTail = bullet;

        bullet->x = e->x;
        bullet->y = e->y;
        
        bullet->texture = enemyBulletTexture;
        SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

        bullet->x += (e->w / 2) - (bullet->w / 2);
        bullet->y += (e->h / 2) - (bullet->h / 2);
        
        bullet->dy = ENEMY_BULLET_SPEED;

        bullet->side = e->side;
        bullet->health = 1;
        
        e->reload = (rand() % FPS * 10);
}

// Clip enemy movements.
// Check if enemies did the max number of step, then
// udapte movement variables to move down and change their direction.
static void clipEnemies(void)
{ 
        if (enemyCurrentStep ==  MAX_ENEMY_STEP)
        {
                enemyCurrentStep = 0;
                enemyMoveDown = TRUE;
                enemyDirection = (enemyDirection == RIGHT) ? LEFT : RIGHT; // Swap enemy direction
        }
}

// Clip player movements.
// Keep player inside the screen.
static void clipPlayer(void)
{
	if (player != NULL)
	{
		if (player->x < HORIZONTAL_POSITION)
		{
			player->x = HORIZONTAL_POSITION;
		}
		
		if (player->x > SCREEN_WIDTH - HORIZONTAL_POSITION - player->w)
		{
			player->x = SCREEN_WIDTH - HORIZONTAL_POSITION - player->w;
		}
	}
}

// Do explosions actions.
// For each explosion, move it and destroy it after a while.
static void doExplosions(void)
{
        Explosion *ex, *prev;

        prev = &stage.explosionHead;

        for (ex = stage.explosionHead.next; ex != NULL; ex = ex->next)
        {
                ex->x += ex->dx;
                ex->y += ex->dy;

                // Decrease opacity and delete explosion.
                // Decrease the explosion's opacity and delete it when it is not visible. 
                if (--ex->a <= 0)
                {
                        if (ex == stage.explosionTail)
                        {
                                stage.explosionTail = prev;
                        }

                        prev->next = ex->next;
                        free(ex);
                        ex = prev;
                }

                prev = ex;
        }
}

// Do debris actions.
// For each debris, move it and destroy it after a while.
static void doDebris(void)
{
	Debris *d, *prev;
	
	prev = &stage.debrisHead;

        // Move and delete debris after a while.
        // Move debris with an extra mouvment, decrease their life property
        // and delete them when their life is gone.  
	for (d = stage.debrisHead.next ; d != NULL ; d = d->next)
	{
		d->x += d->dx;
		d->y += d->dy;
		
		d->dy += 0.5;
		
		if (--d->life <= 0)
		{
			if (d == stage.debrisTail)
			{
				stage.debrisTail = prev;
			}
			
			prev->next = d->next;
			free(d);
			d = prev;
		}
		
		prev = d;
	}
}

// Add an explosion.
// An explosion is composed of 'num' number of explosion elements.
// For each explosion element, allocate memory, add the element to the linked list,
// assign its position and its speed with a random variation, and assign its color.
static void addExplosions(int x, int y, int num)
{
        Explosion *ex;
        int i;

        for (i = 0; i < num; i++)
        {
                ex = malloc(sizeof(Explosion));
                memset(ex, 0, sizeof(Explosion));
                stage.explosionTail->next = ex;
                stage.explosionTail = ex;

                ex->x = x + (rand() % 32) - (rand() % 32);
                ex->y = y + (rand() % 32) - (rand() % 32);
                
                ex->dx = (rand() % 10) - (rand() % 10);
                ex->dy = (rand() % 10) - (rand() % 10);
                ex->dx /= 10;
                ex->dy /= 10;

                switch (rand() % 4)
                {
                case 0:
                        ex->r = 255;
                        break;
                case 1:
                        ex->r = 255;
                        ex->g = 128;
                        break;
                case 2:
                        ex->r = 255;
                        ex->g = 255;
                        break;
                default:
                        ex->r = 255;
                        ex->g = 255;
                        ex->b = 255;
                        break;
                }

                ex->a = rand() % FPS * 3;
        }
}

// Add debris.
// Debris are created by cutting a destroyed entity in four parts.
// For each debris' part, allocate memory, add it to the linked list,
// assign its position according to the entity, assign its speed with a random variation,
// assign its properties, and assign a part of the entity to the debris.
static void addDebris(Entity *e)
{
	Debris *d;
	int x, y, w, h;
	
	w = e->w / 2;
	h = e->h / 2;
	
	for (y = 0 ; y <= h ; y += h)
	{
		for (x = 0 ; x <= w ; x += w)
		{
			d = malloc(sizeof(Debris));
			memset(d, 0, sizeof(Debris));
			stage.debrisTail->next = d;
			stage.debrisTail = d;
			
			d->x = e->x + e->w / 2;
			d->y = e->y + e->h / 2;
                        
			d->dx = (rand() % 5) - (rand() % 5);
			d->dy = -(5 + (rand() % 12));

                        d->texture = e->texture;
                        
			d->life = FPS * 2;
			
			d->rect.x = x;
			d->rect.y = y;
			d->rect.w = w;
			d->rect.h = h;
		}
	}
}

static void draw(void)
{
        drawBackground();
        
	drawPlayer();

        drawEnemies();

        drawDebris();

        drawExplosions();

	drawBullets();

        drawHud();
}

static void drawPlayer(void)
{
	if (player != NULL)
        {
                blit(player->texture, player->x, player->y);
        }
}

static void drawEnemies(void)
{
	Entity *e;
        int i, j;
        
        for (i = 0; i < ENEMY_ROW; i++)
        {        
                for (j = 0; j < ENEMY_COL; j++)
                {
                        e = stage.enemies[i][j];
                        
                        if (e != NULL)
                        {
                                blit(e->texture, e->x, e->y);
                        }       
                }
        }
}

static void drawBullets(void)
{
	Entity *b;

	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		blit(b->texture, b->x, b->y);
	}
}

static void drawDebris(void)
{
	Debris *d;
	
	for (d = stage.debrisHead.next ; d != NULL ; d = d->next)
	{
		blitRect(d->texture, &d->rect, d->x, d->y);
	}
}

static void drawExplosions(void)
{
        Explosion *ex;

        // Manage blend color renderer for explosions
        SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
        SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

        for (ex = stage.explosionHead.next; ex != NULL; ex = ex->next)
        {
                SDL_SetTextureColorMod(explosionTexture, ex->r, ex->g, ex->b);
                SDL_SetTextureAlphaMod(explosionTexture, ex->a);

                blit(explosionTexture, ex->x, ex->y);
        }

        SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}

static void drawHud(void)
{
        drawText(100, 10, 255, 255, 255, TEXT_CENTER, "SCORE<1>");
        drawText(100, 40, 255, 255, 255, TEXT_CENTER, "%04d", stage.score);

        // Change highscore's color frow white to green when the user beats it.
	if (stage.score < highscores.highscore[0].score)
	{
		drawText(SCREEN_WIDTH / 2, 10, 255, 255, 255, TEXT_CENTER, "HI-SCORE");
                drawText(SCREEN_WIDTH / 2, 40, 255, 255, 255, TEXT_CENTER, "%04d", highscores.highscore[0].score);
                
	}
	else
	{
		drawText(SCREEN_WIDTH / 2, 10, 0, 255, 0, TEXT_CENTER, "HI-SCORE");
                drawText(SCREEN_WIDTH / 2, 40, 0, 255, 0, TEXT_CENTER, "%04d", highscores.highscore[0].score);
                
	}	
}
