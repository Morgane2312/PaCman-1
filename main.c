#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600
#define PACMAN_SIZE 20
#define PACMAN_SPEED 5

typedef struct
{
    int x;
    int y;
} Position;

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Could not initialize SDL2: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Pac-Man Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Position pacman = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    bool running = true;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    pacman.y -= PACMAN_SPEED;
                    break;
                case SDLK_DOWN:
                    pacman.y += PACMAN_SPEED;
                    break;
                case SDLK_LEFT:
                    pacman.x -= PACMAN_SPEED;
                    break;
                case SDLK_RIGHT:
                    pacman.x += PACMAN_SPEED;
                    break;
                }
            }
        }

        // Teleport Pac-Man if it goes off the screen and test workflow
        if (pacman.x < 0)
        {
            pacman.x = WINDOW_WIDTH - PACMAN_SIZE;
        }
        else if (pacman.x > WINDOW_WIDTH - PACMAN_SIZE)
        {
            pacman.x = 0;
        }
        if (pacman.y < 0)
        {
            pacman.y = WINDOW_HEIGHT - PACMAN_SIZE;
        }
        else if (pacman.y > WINDOW_HEIGHT - PACMAN_SIZE)
        {
            pacman.y = 0;
        }

        SDL_SetRenderDrawColor(renderer, 194, 243, 192, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 187, 145, 124, 255);
        SDL_Rect pacmanRect = {pacman.x, pacman.y, PACMAN_SIZE, PACMAN_SIZE};
        SDL_RenderFillRect(renderer, &pacmanRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}