#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "GameObject.h"

void Class_InitBackground();

/// @brief Description de la classe "Background".
extern const void *const Class_Background;

typedef struct BackgroundClass_s {
    const GameObjectClass base;
} BackgroundClass;

typedef struct Background_s
{
    struct {
        GameObject base;
    } m_super;

    PE_Vec2 m_worldDim;
    int m_layerCount;
    SDL_Texture **m_layers;
    float *m_xShiftFactors;
    float *m_yShiftFactors;
} Background;

void Background_Constructor(
    void *self, void *scene, int renderLayer, int layerCount, PE_Vec2 worldDim
);

INLINE void Background_SetLayer(void *self, SDL_Texture *layer, int index)
{
    Background *background = Object_Cast(self, Class_Background);
    assert(0 <= index && index < background->m_layerCount);
    background->m_layers[index] = layer;
}

INLINE void Background_SetXShiftFactors(void *self, float factor, int index)
{
    Background *background = Object_Cast(self, Class_Background);
    assert(0 <= index && index < background->m_layerCount);
    background->m_xShiftFactors[index] = factor;
}

INLINE void Background_SetYShiftFactors(void *self, float factor, int index)
{
    Background *background = Object_Cast(self, Class_Background);
    assert(0 <= index && index < background->m_layerCount);
    background->m_yShiftFactors[index] = factor;
}

#endif