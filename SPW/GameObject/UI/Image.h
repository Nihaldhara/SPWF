#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "UIObject.h"

void Class_InitImage();

/// @brief Description de la classe "Image".
extern const void *const Class_Image;

typedef struct ImageClass_s {
    const UIObjectClass base;
} ImageClass;

typedef struct Image_s
{
    struct {
        UIObject base;
    } m_super;

    SDL_Texture *m_texture;
    SDL_Rect *m_srcRect;

    bool m_hasBorders;
    UIBorders m_borders;
} Image;

void Image_Constructor(void *self, void *scene, RE_AtlasPart *part, int index);

void Image_SetBorders(void *self, UIBorders *borders);

INLINE void Image_GetNaturalSize(void *self, int *w, int *h)
{
    Image *image = Object_Cast(self, Class_Image);
    *w = image->m_srcRect->w;
    *h = image->m_srcRect->h;
}

INLINE float Image_GetNaturalRatio(void *self)
{
    Image *image = Object_Cast(self, Class_Image);
    return (float)image->m_srcRect->w / (float)image->m_srcRect->h;
}

#endif
