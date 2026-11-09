#include <stddef.h>
#include <math.h>
#include "render.h"

#define BOID_LENGTH 4
#define BOID_WIDTH 4
#define TRAIL_WIDTH 1

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
    SDL_FColor color = {1.0f, 1.0f, 1.0f, 1.0f};
    for (int i = 0; i < 3; i++)
    {
        vertices[i].position = (SDL_FPoint){.x = points[i].x, .y = points[i].y};
        vertices[i].color = color;
    }

    const int indices[] = {0, 1, 2};
    SDL_RenderGeometry(renderer, NULL, vertices, 3, indices, 3);
}

static void add_trail_segment(
    vec2 from, vec2 to, float width, SDL_Vertex vertices[], int indices[], int *n_vertices, int *curr_idx, float max_trail_length_squared)
{
    if (vec2_distance_squared(to, from) > max_trail_length_squared)
        return;

    int base = *n_vertices;

    vec2 dir = vec2_normalize(vec2_sub(to, from));
    vec2 norm = vec2_scale(vec2_perpendicular(dir), width * 0.5f);

    vec2 points[4] = {
        vec2_add(from, norm),
        vec2_sub(from, norm),
        vec2_add(to, norm),
        vec2_sub(to, norm),
    };
    const SDL_FColor color = (SDL_FColor){255, 255, 255, 255};

    for (int i = 0; i < 4; i++)
    {
        vertices[*n_vertices + i] = (SDL_Vertex){
            .position = {.x = points[i].x, .y = points[i].y}, .color = color};
    }
    *n_vertices += 4;

    indices[*curr_idx + 0] = base;
    indices[*curr_idx + 1] = base + 1;
    indices[*curr_idx + 2] = base + 2;

    indices[*curr_idx + 3] = base + 2;
    indices[*curr_idx + 4] = base + 1;
    indices[*curr_idx + 5] = base + 3;

    *curr_idx += 6;
}

static void render_trail(SDL_Renderer *renderer, const Boid *boid, int trail_length, float max_trail_length_squared)
{
    SDL_Vertex vertices[trail_length * 4];
    int indices[trail_length * 6];
    int n_vertices = 0;
    int curr_idx = 0;

    for (size_t i = 1; i < trail_length; i++)
        add_trail_segment(boid->trail[i - 1], boid->trail[i], TRAIL_WIDTH, vertices, indices, &n_vertices, &curr_idx, max_trail_length_squared);
    add_trail_segment(boid->trail[trail_length - 1], boid->pos, TRAIL_WIDTH, vertices, indices, &n_vertices, &curr_idx, max_trail_length_squared);

    SDL_RenderGeometry(renderer, NULL, vertices, n_vertices, indices, curr_idx);
}

static void render_boid(SDL_Renderer *renderer, const Boid *boid, int trail_length, float max_trail_length)
{
    render_trail(renderer, boid, trail_length, max_trail_length * max_trail_length);
    render_head(renderer, boid);
}

void render_frame(SDL_Renderer *renderer, BoidsSimulation *sim)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (size_t i = 0; i < sim->num_boids; i++)
        render_boid(renderer, &sim->boids[i], sim->trail_length, sim->max_speed);
}
