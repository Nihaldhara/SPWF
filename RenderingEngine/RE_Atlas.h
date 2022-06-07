#ifndef _RE_ATLAS_H_
#define _RE_ATLAS_H_

#include "RE_Settings.h"
#include "RE_Renderer.h"

typedef struct RE_Atlas_s RE_Atlas;

typedef struct RE_AtlasPart_s
{
    RE_Atlas *m_atlas;
    char *m_name;
    int *m_frames;
    int m_frameCount;
} RE_AtlasPart;

typedef struct RE_Atlas_s
{
    SDL_Renderer *m_renderer;
    SDL_Texture *m_texture;

    RE_AtlasPart *m_parts;
    int m_partCount;

    SDL_Rect *m_rects;
    int m_rectCount;
} RE_Atlas;

RE_Atlas *RE_Atlas_New(SDL_Renderer *renderer, const char *path);
void RE_Atlas_Delete(RE_Atlas *atlas);

RE_INLINE SDL_Texture *RE_Atlas_GetTexture(const RE_Atlas *atlas)
{
    assert(atlas);
    return atlas->m_texture;
}

RE_AtlasPart *RE_Atlas_GetPart(const RE_Atlas *atlas, const char *name);

RE_INLINE int RE_AtlasPart_GetFrameCount(const RE_AtlasPart *part)
{
    assert(part);
    return part->m_frameCount;
}

RE_INLINE SDL_Texture *RE_AtlasPart_GetTexture(const RE_AtlasPart *part)
{
    assert(part);
    return RE_Atlas_GetTexture(part->m_atlas);
}

RE_INLINE SDL_Rect *RE_AtlasPart_GetSrcRect(const RE_AtlasPart *part, int i)
{
    assert(part && part->m_atlas && (i >= 0));
    RE_Atlas *atlas = part->m_atlas;
    int frameCount = part->m_frameCount;
    int rectIndex = part->m_frames[i % frameCount];
    return &(atlas->m_rects[rectIndex]);
}

RE_INLINE int RE_AtlasPart_RenderCopyF(
    RE_AtlasPart *part, int idx,
    SDL_Renderer *renderer, const SDL_FRect *dstRect, int anchor)
{
    assert(part && part->m_atlas && renderer);
    SDL_Texture *texture = RE_Atlas_GetTexture(part->m_atlas);
    SDL_Rect *srcRect = RE_AtlasPart_GetSrcRect(part, idx);
    return RE_RenderCopyF(renderer, texture, srcRect, dstRect, anchor);
}

RE_INLINE int RE_AtlasPart_RenderCopy(
    RE_AtlasPart *part, int idx,
    SDL_Renderer *renderer, const SDL_Rect *dstRect, int anchor)
{
    assert(part && part->m_atlas && renderer);
    SDL_Texture *texture = RE_Atlas_GetTexture(part->m_atlas);
    SDL_Rect *srcRect = RE_AtlasPart_GetSrcRect(part, idx);
    return RE_RenderCopy(renderer, texture, srcRect, dstRect, anchor);
}

RE_INLINE int RE_AtlasPart_RenderCopyExF(
    RE_AtlasPart *part, int idx,
    SDL_Renderer *renderer, const SDL_FRect *dstRect, int anchor,
    const double angle, Vec2 center, int flip)
{
    assert(part && part->m_atlas && renderer);
    SDL_Texture *texture = RE_Atlas_GetTexture(part->m_atlas);
    SDL_Rect *srcRect = RE_AtlasPart_GetSrcRect(part, idx);
    return RE_RenderCopyExF(renderer, texture, srcRect, dstRect, anchor, angle, center, flip);
}

#endif
