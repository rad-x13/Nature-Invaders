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

#include "draw.h"

void prepareScene(void)
{
	SDL_SetRenderDrawColor(app.renderer, 32, 32, 32, 255);
	SDL_RenderClear(app.renderer);
}

void presentScene(void)
{
	SDL_RenderPresent(app.renderer);
}

static SDL_Texture *getTexture(char *name)
{
	Texture *t;

        // Search for texture.
        // For each texture in the linked list, compare its name with parameter name
        // return texture is names match or NULL if not.
	for (t = app.textureHead.next; t != NULL; t = t->next)
	{
		if (strcmp(t->name, name) == 0)
		{
			return t->texture;
		}
	}

	return NULL;
}

static void addTextureToCache(char *name, SDL_Texture *sdlTexture)
{
	Texture *texture;

        // Add texture to cache
        // Allocate memory for the texture object, add it to the linked list,
        // and truncate the texture name if necessary
	texture = malloc(sizeof(Texture));
	memset(texture, 0, sizeof(Texture));
	app.textureTail->next = texture;
	app.textureTail = texture;

	STRNCPY(texture->name, name, MAX_NAME_LENGTH);
	texture->texture = sdlTexture;
}

SDL_Texture *loadTexture(char *filename)
{
	SDL_Texture *texture;

	texture = getTexture(filename);

        // Load texture
        // Load an image from filename and return a texture with SDL image library, then
        // addthe texture to the cache  
	if (texture == NULL)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
		texture = IMG_LoadTexture(app.renderer, filename);
		addTextureToCache(filename, texture);
	}

	return texture;
}

void blit(SDL_Texture *texture, int x, int y)
{
	SDL_Rect dest;

        // Draw the texture
        // Draw the given texture on the screen according to the given positions x and y
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
{
	SDL_Rect dest;

        // Draw a part of the texture
        // Draw a part of the given texture, define by its width and its height, 
        // on the screen according to the given positions x and y
	dest.x = x;
	dest.y = y;
	dest.w = src->w;
	dest.h = src->h;

	SDL_RenderCopy(app.renderer, texture, src, &dest);
}



