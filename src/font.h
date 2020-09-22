// Handle bitmap font.
// Copyright (c) 2020 PotatoTech
// SPDX-License-Identifier: Apache-2.0

#ifndef FONT_H
#define FONT_H

#include "defs.h"

#define GLYPH_WIDTH 5
#define GLYPH_HEIGHT 8

SDL_Texture* load_bitmap_font(SDL_Renderer* renderer, const char* path);
SDL_Texture* load_default_font(SDL_Renderer* renderer);
void get_char_src_rect(SDL_Rect* rect, const char c);

#endif
