// Rendering.
// Copyright (c) 2020 PotatoTech
// SPDX-License-Identifier: Apache-2.0

#include "render.h"
#include "font.h"

#define SCORE_FONT_WIDTH (10 * GLYPH_WIDTH)
#define SCORE_FONT_HEIGHT (10 * GLYPH_HEIGHT)
#define SCORE_OFFSET_Y 30

static void show_score(SDL_Renderer* renderer, struct Game* game, SDL_Texture* font) {
    static const SDL_Rect tens_dest_rect_left = {
        .x = SCREEN_WIDTH / 4 - SCORE_FONT_WIDTH,
        .y = SCORE_OFFSET_Y,
        .w = SCORE_FONT_HEIGHT / 2,
        .h = SCORE_FONT_HEIGHT
    };
    static const SDL_Rect ones_dest_rect_left = {
        .x = SCREEN_WIDTH / 4,
        .y = SCORE_OFFSET_Y,
        .w = SCORE_FONT_HEIGHT / 2,
        .h = SCORE_FONT_HEIGHT
    };
    static const SDL_Rect tens_dest_rect_right = {
        .x = SCREEN_WIDTH - SCREEN_WIDTH / 4 - SCORE_FONT_WIDTH,
        .y = SCORE_OFFSET_Y,
        .w = SCORE_FONT_HEIGHT / 2,
        .h = SCORE_FONT_HEIGHT
    };
    static const SDL_Rect ones_dest_rect_right = {
        .x = SCREEN_WIDTH - SCREEN_WIDTH / 4,
        .y = SCORE_OFFSET_Y,
        .w = SCORE_FONT_HEIGHT / 2,
        .h = SCORE_FONT_HEIGHT
    };

    char ones_digit_left = (char)('0' + game->left_score % 10);
    char ones_digit_right = (char)('0' + game->right_score % 10);
    SDL_Rect ones_src_rect_left;
    SDL_Rect ones_src_rect_right;
    get_char_src_rect(&ones_src_rect_left, ones_digit_left);
    get_char_src_rect(&ones_src_rect_right, ones_digit_right);
    SDL_RenderCopy(renderer, font, &ones_src_rect_left, &ones_dest_rect_left);
    SDL_RenderCopy(renderer, font, &ones_src_rect_right, &ones_dest_rect_right);

    if (game->left_score >= 10) {
        char tens_digit_left = (char)('0' + game->left_score / 10 % 10);
        SDL_Rect tens_src_rect_left;
        get_char_src_rect(&tens_src_rect_left, tens_digit_left);
        SDL_RenderCopy(renderer, font, &tens_src_rect_left, &tens_dest_rect_left);
    }
    if (game->right_score >= 10) {
        char tens_digit_right = (char)('0' + game->right_score / 10 % 10);
        SDL_Rect tens_src_rect_right;
        get_char_src_rect(&tens_src_rect_right, tens_digit_right);
        SDL_RenderCopy(renderer, font, &tens_src_rect_right, &tens_dest_rect_right);
    }
}

void render_game(SDL_Renderer* renderer, struct Game* game, SDL_Texture* font) {
    show_score(renderer, game, font);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    if (
        game->state == GAME_RUNNING ||
        (game->state == GAME_PAUSED && game->state_before_pause == GAME_RUNNING)
    ) {
        SDL_RenderFillRect(renderer, &game->ball.rect);
    }
    if (game->state != GAME_PLAYER_L_WON && game->state != GAME_PLAYER_R_WON) {
        SDL_RenderFillRect(renderer, &game->left_paddle.rect);
        SDL_RenderFillRect(renderer, &game->right_paddle.rect);
    }
}
