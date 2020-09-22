// Game logic.
// Copyright (c) 2020 PotatoTech
// SPDX-License-Identifier: Apache-2.0

#include "logic.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define WINNING_SCORE 11
#define BALL_SIZE 20
#define BALL_SPEED 900.0
#define BALL_INIT_OFFSET_X (SCREEN_WIDTH / 4)
#define BALL_INIT_OFFSET_Y (SCREEN_HEIGHT / 6)
#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 100
#define PADDLE_OFFSET_X 100
#define PADDLE_MIN_Y_BOUNDARY ((int)(BALL_SIZE * 1.2))
#define PADDLE_MAX_Y_BOUNDARY (SCREEN_HEIGHT - PADDLE_MIN_Y_BOUNDARY)

enum Player {
    PLAYER_L,
    PLAYER_R
};

static void reset_ball(struct Game* game, enum Player player_scored) {
    // Reset y position and velocity.
    bool is_directed_up = rand() % 2;
    if (is_directed_up) {
        game->ball.rect.y = BALL_INIT_OFFSET_Y;
        game->ball.velocity_y = .6 * -BALL_SPEED;
    }
    else {
        game->ball.rect.y = SCREEN_HEIGHT - BALL_INIT_OFFSET_Y - BALL_SIZE;
        game->ball.velocity_y = .6 * BALL_SPEED;
    }

    // Reset x position and velocity.
    switch (player_scored) {
    default:
    case PLAYER_L:
        game->ball.rect.x = BALL_INIT_OFFSET_X;
        game->ball.velocity_x = BALL_SPEED;
        break;
    case PLAYER_R:
        game->ball.rect.x = SCREEN_WIDTH - BALL_INIT_OFFSET_X - BALL_SIZE;
        game->ball.velocity_x = -BALL_SPEED;
        break;
    }
}

void init_game(struct Game* game) {
    game->state = GAME_WAITING;
    game->wait_ticks = (int)(1.5 * FRAMERATE);
    game->ball.rect.w = BALL_SIZE;
    game->ball.rect.h = BALL_SIZE;
    reset_ball(game, PLAYER_L);

    game->left_paddle.rect.x = PADDLE_OFFSET_X;
    game->left_paddle.rect.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    game->left_paddle.rect.w = PADDLE_WIDTH;
    game->left_paddle.rect.h = PADDLE_HEIGHT;
    game->left_paddle.velocity_y = 0;

    game->right_paddle.rect.x = SCREEN_WIDTH - PADDLE_OFFSET_X - PADDLE_WIDTH;
    game->right_paddle.rect.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    game->right_paddle.rect.w = PADDLE_WIDTH;
    game->right_paddle.rect.h = PADDLE_HEIGHT;
    game->right_paddle.velocity_y = 0;

    game->left_score = 0;
    game->right_score = 0;
}

static void move_paddles(struct Game* game) {
    game->left_paddle.rect.y += (int)(game->left_paddle.velocity_y / FRAMERATE);
    game->right_paddle.rect.y += (int)(game->right_paddle.velocity_y / FRAMERATE);

    // Restrict the paddles so they cannot touch the upper and lower walls.
    if (game->left_paddle.rect.y < PADDLE_MIN_Y_BOUNDARY) {
        game->left_paddle.rect.y = PADDLE_MIN_Y_BOUNDARY;
    }
    else if (game->left_paddle.rect.y > PADDLE_MAX_Y_BOUNDARY - PADDLE_HEIGHT) {
        game->left_paddle.rect.y = PADDLE_MAX_Y_BOUNDARY - PADDLE_HEIGHT;
    }
    if (game->right_paddle.rect.y < PADDLE_MIN_Y_BOUNDARY) {
        game->right_paddle.rect.y = PADDLE_MIN_Y_BOUNDARY;
    }
    else if (game->right_paddle.rect.y > PADDLE_MAX_Y_BOUNDARY - PADDLE_HEIGHT) {
        game->right_paddle.rect.y = PADDLE_MAX_Y_BOUNDARY - PADDLE_HEIGHT;
    }
}

static void score_point(struct Game* game, enum Player player_scored) {
    switch (player_scored) {
    case PLAYER_L: ++game->left_score; break;
    case PLAYER_R: ++game->right_score; break;
    }

    // Check win condition.
    if (game->left_score >= WINNING_SCORE) {
        game->state = GAME_PLAYER_L_WON;
        game->wait_ticks = 5 * FRAMERATE;
    }
    else if (game->right_score >= WINNING_SCORE) {
        game->state = GAME_PLAYER_R_WON;
        game->wait_ticks = 5 * FRAMERATE;
    }
    else {
        // Wait before sending the next ball.
        game->state = GAME_WAITING;
        game->wait_ticks = (int)(1.5 * FRAMERATE);
        reset_ball(game, player_scored);
    }
}

static void hit_ball(struct Game* game, enum Player player) {
    int ball_center_y = game->ball.rect.y + BALL_SIZE / 2;
    int paddle_center_y;

    switch (player) {
    default:
    case PLAYER_L:
        paddle_center_y = game->left_paddle.rect.y + PADDLE_HEIGHT / 2;
        game->ball.velocity_x = fabs(game->ball.velocity_x);
        break;
    case PLAYER_R:
        paddle_center_y = game->right_paddle.rect.y + PADDLE_HEIGHT / 2;
        game->ball.velocity_x = -fabs(game->ball.velocity_x);
        break;
    }

    game->ball.velocity_y = ((double)(ball_center_y - paddle_center_y) /
        PADDLE_HEIGHT) * BALL_SPEED * 1.5;
}

static void move_ball(struct Game* game) {
    game->ball.rect.x += (int)(game->ball.velocity_x / FRAMERATE);
    game->ball.rect.y += (int)(game->ball.velocity_y / FRAMERATE);

    // Point scored detection
    if (game->ball.rect.x < 0) {
        score_point(game, PLAYER_R);
    }
    else if (game->ball.rect.x > SCREEN_WIDTH - BALL_SIZE) {
        score_point(game, PLAYER_L);
    }

    // Collision detection for walls
    if (game->ball.rect.y < 0) {
        game->ball.rect.y = 0;
        game->ball.velocity_y = -game->ball.velocity_y;
    }
    else if (game->ball.rect.y > SCREEN_HEIGHT - BALL_SIZE) {
        game->ball.rect.y = SCREEN_HEIGHT - BALL_SIZE;
        game->ball.velocity_y = -game->ball.velocity_y;
    }

    // Collision detection for paddles
    if (SDL_HasIntersection(&game->ball.rect, &game->left_paddle.rect)) {
        hit_ball(game, PLAYER_L);
    }
    else if (SDL_HasIntersection(&game->ball.rect, &game->right_paddle.rect)) {
        hit_ball(game, PLAYER_R);
    }
}

void update_game(struct Game* game) {
    if (game->state == GAME_RUNNING || game->state == GAME_WAITING) {
        move_paddles(game);
    }
    if (game->state == GAME_RUNNING) move_ball(game);

    if (game->wait_ticks > 0) --game->wait_ticks;
    if (game->wait_ticks == 0) {
        if (game->state == GAME_WAITING) {
            game->state = GAME_RUNNING;
        }
        else if (game->state == GAME_PLAYER_L_WON || game->state == GAME_PLAYER_R_WON) {
            init_game(game);
        }
    }
}
