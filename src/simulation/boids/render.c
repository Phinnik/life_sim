#include <stddef.h>
#include <math.h>
#include "render.h"

#define BOID_LENGTH 4
#define BOID_WIDTH 4

static void render_head(SDL_Renderer *renderer, const Boid *boid)
{
    vec2 velocity_norm = vec2_normalize(boid->velocity);
    vec2 velocity_anti = vec2_scale(vec2_negate(velocity_norm), BOID_LENGTH * 0.6f);
    vec2 velocity_perp = vec2_scale(vec2_perpendicular(velocity_norm), BOID_WIDTH);

    vec2 points[3] = {
        vec2_add(boid->pos, vec2_scale(velocity_norm, BOID_LENGTH)),
        vec2_add(boid->pos, vec2_add(velocity_anti, velocity_perp)),
        vec2_add(boid->pos, vec2_add(velocity_anti, vec2_negate(velocity_perp))),
    };

    SDL_Vertex vertices[3];
    SDL_FColor color = {100.0f, 210.0f, 255.0f, 255.0f};
    for (int i = 0; i < 3; i++)
    {
        vertices[i].position = (SDL_FPoint){.x = points[i].x, .y = points[i].y};
        vertices[i].color = color;
    }

    const int indices[] = {0, 1, 2};
    SDL_RenderGeometry(renderer, NULL, vertices, 3, indices, 3);
}

static void render_trail(SDL_Renderer *renderer, const Boid *boid, int trail_length, float max_trail_length)
{
    for (size_t i = 1; i < trail_length; i++)
    {
        float d = vec2_distance_squared(boid->trail[i - 1], boid->trail[i]);
        if (d <= max_trail_length * 2)
        {
            SDL_RenderLine(
                renderer,
                boid->trail[i - 1].x,
                boid->trail[i - 1].y,
                boid->trail[i].x,
                boid->trail[i].y);
        }
    }
    float d = vec2_distance_squared(boid->trail[trail_length - 1], boid->pos);
    if (d <= max_trail_length * 2)
    {
        SDL_RenderLine(
            renderer,
            boid->trail[trail_length - 1].x,
            boid->trail[trail_length - 1].y,
            boid->pos.x,
            boid->pos.y);
    }
}

static void render_boid(SDL_Renderer *renderer, const Boid *boid, int trail_length, float max_trail_length)
{
    render_trail(renderer, boid, trail_length, max_trail_length);
    render_head(renderer, boid);
}

void render_frame(SDL_Renderer *renderer, BoidsSimulation *sim)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (size_t i = 0; i < sim->num_boids; i++)
        render_boid(renderer, &sim->boids[i], sim->trail_length, sim->max_speed);
}
