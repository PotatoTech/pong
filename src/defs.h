// Common definitions.
// Copyright (c) 2020 PotatoTech
// SPDX-License-Identifier: Apache-2.0

#ifndef DEFS_H
#define DEFS_H

#include "SDL.h"
#include "SDL_image.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define MIN_SCREEN_WIDTH 854
#define MIN_SCREEN_HEIGHT 480
#define FRAMERATE 60

#define PADDLE_SPEED 700.0

enum GameState {
    GAME_QUIT,
    GAME_RUNNING,
    GAME_WAITING,
    GAME_PAUSED,
    GAME_PLAYER_L_WON,
    GAME_PLAYER_R_WON
};

struct Ball {
    SDL_Rect rect;
    double velocity_x;
    double velocity_y;
};

struct Paddle {
    SDL_Rect rect;
    double velocity_y;
};

struct Game {
    enum GameState state;
    enum GameState state_before_pause;
    int wait_ticks;
    struct Ball ball;
    struct Paddle left_paddle;
    struct Paddle right_paddle;
    unsigned left_score;
    unsigned right_score;
};

#endif
