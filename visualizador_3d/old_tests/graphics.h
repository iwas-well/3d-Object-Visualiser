#ifndef __GRAF_VISU__
#define __GRAF_VISU__

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <stdio.h>
#include "datatypes.h"
#include "perspect.h"

void render_polygon(SDL_Renderer* renderer, polygon* pol);

void render_object(mesh* object, cam* camera, SDL_Renderer* renderer);

#endif
