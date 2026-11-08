#include <stddef.h>
#include "render.h"

#define BOID_SIZE 4

static void render_boid(SDL_Renderer *renderer, BoidsSimulation *sim, size_t idx)
{
    SDL_FRect rect = (SDL_FRect){
        .x = sim->boids[idx].pos.x - BOID_SIZE / 2,
        .y = sim->boids[idx].pos.y - BOID_SIZE / 2,
        .h = BOID_SIZE,
        .w = BOID_SIZE,
    };
    SDL_RenderFillRect(renderer, &rect);
}

void render_frame(SDL_Renderer *renderer, BoidsSimulation *sim)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (size_t i = 0; i < sim->num_boids; i++)
        render_boid(renderer, sim, i);
}
