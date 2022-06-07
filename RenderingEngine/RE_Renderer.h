
/// @file RE_Renderer.h
/// @defgroup RE_Renderer Fonctions relatives au renderer

#ifndef _RE_RENDERER_H_
#define _RE_RENDERER_H_

#include "RE_Settings.h"
#include "RE_Utils.h"

typedef enum RE_Anchor_e
{
    // Horizontal
    RE_ANCHOR_LEFT = 0x01,
    RE_ANCHOR_CENTER = 0x02,
    RE_ANCHOR_RIGHT = 0x03,
    // Vertical
    RE_ANCHOR_BOTTOM = 0x10,
    RE_ANCHOR_MIDDLE = 0x20,
    RE_ANCHOR_TOP = 0x30,
} RE_Anchor;



int RE_RenderCopy(
    SDL_Renderer *renderer, SDL_Texture *texture,
    const SDL_Rect *srcRect, const SDL_Rect *dstRect, int anchor
);

int RE_RenderCopyF(
    SDL_Renderer *renderer, SDL_Texture *texture,
    const SDL_Rect *srcRect, const SDL_FRect *dstRect, int anchor
);

int RE_RenderCopyExF(
    SDL_Renderer *renderer, SDL_Texture *texture,
    const SDL_Rect *srcRect, const SDL_FRect *dstRect, int anchor,
    const double angle, Vec2 center, int flip
);


int RE_RenderCopyBordersF(
    SDL_Renderer *renderer, SDL_Texture *texture,
    const SDL_Rect *srcRect, const SDL_FRect *dstRect, int anchor,
    int l, int r, int t, int b
);

#endif