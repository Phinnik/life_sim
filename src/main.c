#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "simulation/boids/simulation.h"
#include "simulation/boids/render.h"

// Rendering settings
#define WINDOW_HEIGHT 1920
#define WINDOW_WIDTH 1080
#define FPS 60
#define MS_PER_FRAME 1 * 1000/ FPS
#define UPS 300
#define MS_PER_UPDATE 1 * 1000 / UPS
#define WINDOW_NAME "boids"
#define TRAIL_LENGTH 200

// Simulation settings
#define NUM_BOIDS 500
#define HEIGHT WINDOW_HEIGHT
#define WIDTH WINDOW_WIDTH
#define MAX_SPEED 1.5f
#define MAX_FORCE 0.5f
#define PERCEPTION 50
#define SEPARATION_WEIGHT 1.3f
#define ALIGNMENT_WEIGHT 1.0f
#define COHESION_WEIGHT 1.0f

typedef struct appstate_t
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    uint32_t seed;
    BoidsSimulation *sim;
    uint64_t last_updated;
    uint64_t last_rendered;
} appstate_t;

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    appstate_t *st = (appstate_t *)appstate;
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    appstate_t *st = (appstate_t *)appstate;
    const uint32_t now = SDL_GetTicks();

    if (now - st->last_updated > MS_PER_UPDATE)
    {
        boids_step(st->sim);
        st->last_updated = now;
    }
    if (now - st->last_rendered > MS_PER_FRAME)
    {
        SDL_SetRenderDrawColor(st->renderer, 8, 12, 24, 255);
        SDL_RenderClear(st->renderer);
        render_frame(st->renderer, st->sim);
        SDL_RenderPresent(st->renderer);
        st->last_rendered = now;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    appstate_t *st = (appstate_t *)SDL_malloc(sizeof(appstate_t));
    *appstate = st;
    st->seed = 42;
    st->sim = boids_new(
        NUM_BOIDS,
        HEIGHT,
        WIDTH,
        MAX_SPEED,
        MAX_FORCE,
        PERCEPTION,
        SEPARATION_WEIGHT,
        ALIGNMENT_WEIGHT,
        COHESION_WEIGHT,
        TRAIL_LENGTH,
        &st->seed);

    if (!st->sim)
    {
        SDL_Log("Couldn't initialize Simulation");
        return SDL_APP_FAILURE;
    }

    SDL_SetAppMetadata(WINDOW_NAME, "1.0", "com.example.boids");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Life simulator", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE,
                                     &st->window, &st->renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(st->renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return SDL_APP_CONTINUE;
}