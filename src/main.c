// Main game loop.
// Copyright (c) 2020 PotatoTech
// SPDX-License-Identifier: Apache-2.0

#include "logic.h"
#include "render.h"
#include "font.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAYBE_UNUSED(x) ((void)x)

int main(int argc, char* argv[]) {
    MAYBE_UNUSED(argc);
    MAYBE_UNUSED(argv);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);

    SDL_Window* window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }

#ifdef _WIN32
    char font_path[] = "res\\font.png";
#else
    char font_path[] = "res/font.png";
#endif

    char* base_path = SDL_GetBasePath();
    char* font_abs_path = malloc(strlen(base_path) + strlen(font_path) + 1);
    strcpy(font_abs_path, base_path);
    strcat(font_abs_path, font_path);

    SDL_Texture* font = load_bitmap_font(renderer, font_abs_path);
    if (!font) {
        fprintf(stderr, "Could not load %s\nLoading default font\n", font_path);
        font = load_default_font(renderer);
        if (!font) {
            fprintf(stderr, "Could not load default font\n");
            return 1;
        }
    }
    free(font_abs_path);
    SDL_free(base_path);

    SDL_SetWindowMinimumSize(window, MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT);
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    srand((unsigned)time(NULL));
    struct Game game;
    init_game(&game);

    bool is_pause_held = false;
    bool left_paddle_up = false;
    bool left_paddle_down = false;
    bool right_paddle_up = false;
    bool right_paddle_down = false;

    while (game.state != GAME_QUIT) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_KEYDOWN:
                switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_RETURN:
                    if (!is_pause_held) {
                        if (game.state != GAME_PAUSED) {
                            game.state_before_pause = game.state;
                            game.state = GAME_PAUSED;
                        }
                        else {
                            game.state = game.state_before_pause;
                        }
                    }
                    is_pause_held = true;
                    break;
                case SDL_SCANCODE_E: left_paddle_up = true; break;
                case SDL_SCANCODE_D: left_paddle_down = true; break;
                case SDL_SCANCODE_UP: right_paddle_up = true; break;
                case SDL_SCANCODE_DOWN: right_paddle_down = true; break;
                default: break;
                }
                break;
            case SDL_KEYUP:
                switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_RETURN: is_pause_held = false; break;
                case SDL_SCANCODE_E: left_paddle_up = false; break;
                case SDL_SCANCODE_D: left_paddle_down = false; break;
                case SDL_SCANCODE_UP: right_paddle_up = false; break;
                case SDL_SCANCODE_DOWN: right_paddle_down = false; break;
                default: break;
                }
                break;
            case SDL_QUIT: game.state = GAME_QUIT; break;
            default: break;
            }
        }

        game.left_paddle.velocity_y = 0;
        game.right_paddle.velocity_y = 0;
        if (left_paddle_up && !left_paddle_down) {
            game.left_paddle.velocity_y = -PADDLE_SPEED;
        }
        if (left_paddle_down && !left_paddle_up) {
            game.left_paddle.velocity_y = PADDLE_SPEED;
        }
        if (right_paddle_up && !right_paddle_down) {
            game.right_paddle.velocity_y = -PADDLE_SPEED;
        }
        if (right_paddle_down && !right_paddle_up) {
            game.right_paddle.velocity_y = PADDLE_SPEED;
        }

        // Visually differentiate playable and non-playable areas.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &(SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});

        update_game(&game);
        render_game(renderer, &game, font);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / FRAMERATE);
    }
    return 0;
}
