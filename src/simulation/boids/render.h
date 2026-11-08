#ifndef BOIDS_RENDER_H
#define BOIDS_RENDER_H

#include "simulation.h"
#include <SDL3/SDL.h>

void render_frame(SDL_Renderer *renderer, BoidsSimulation *sim);

#endif // #ifndef BOIDS_RENDER_H
