// Handle bitmap font.
// Copyright (c) 2020 PotatoTech
// SPDX-License-Identifier: Apache-2.0

#include "font.h"
#include "default_font_data.h"

#define GLYPHS_PER_ROW 16

static SDL_Texture* load_texture_file(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* font_surface = IMG_Load(path);
    if (!font_surface) return NULL;

    SDL_Texture* font = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_FreeSurface(font_surface);
    return font;
}

SDL_Texture* load_bitmap_font(SDL_Renderer* renderer, const char* path) {
    SDL_Texture* font = load_texture_file(renderer, path);
    if (!font) return NULL;

    SDL_Rect file_dimensions;
    SDL_QueryTexture(font, NULL, NULL, &file_dimensions.w, &file_dimensions.h);
    if (file_dimensions.w < GLYPH_WIDTH * GLYPHS_PER_ROW || file_dimensions.h < GLYPH_HEIGHT) {
        free(font);
        return NULL;
    }
    return font;
}

SDL_Texture* load_default_font(SDL_Renderer* renderer) {
    SDL_RWops* font_data = SDL_RWFromConstMem(default_font_data, default_font_data_length);
    if (!font_data) return NULL;

    SDL_Surface* font_surface = IMG_Load_RW(font_data, 1);
    if (!font_surface) return NULL;

    SDL_Texture* font = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_FreeSurface(font_surface);
    return font;
}

void get_char_src_rect(SDL_Rect* rect, const char c) {
    rect->y = 0;
    rect->w = GLYPH_WIDTH;
    rect->h = GLYPH_HEIGHT;

    if (c >= '0' && c <= '9') {
        rect->x = (c - '0') * GLYPH_WIDTH;
    }
    else if (c >= 'A' && c <= 'F') {
        rect->x = 10 * GLYPH_WIDTH + (c - 'A') * GLYPH_WIDTH ;
    }
    else {
        rect->x = 0;
        rect->w = 0;
        rect->h = 0;
    }
}
