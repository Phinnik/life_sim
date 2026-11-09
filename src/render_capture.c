#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>

bool output_frame(SDL_Renderer *renderer)
{
    SDL_Surface *surface = SDL_RenderReadPixels(renderer, NULL);

    if (!surface)
    {
        fprintf(stderr, "SDL_RenderReadPixels error: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface *rgba = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);

    if (rgba->w != 1080 || rgba->h != 1920)
    {
        fprintf(stderr, "Invalid frame size: %dx%d\n", rgba->w, rgba->h);
        SDL_DestroySurface(rgba);
        return false;
    }

    SDL_DestroySurface(surface);
    if (!rgba)
    {
        fprintf(stderr, "SDL_ConvertSurface error: %s\n", SDL_GetError());
        return false;
    }

    for (int y = 0; y < rgba->h; y++)
    {
        unsigned char *row = (unsigned char *)rgba->pixels + y * rgba->pitch;
        fwrite(row, 1, rgba->w * 4, stdout);
    }
    fflush(stdout);
    SDL_DestroySurface(rgba);
    return true;
}