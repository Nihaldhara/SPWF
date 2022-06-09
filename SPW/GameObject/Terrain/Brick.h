#ifndef _BRICK_H_
#define _BRICK_H_

#include "../GameBody.h"

void Class_InitBrick();

/// @brief Description de la classe "Brick".
extern const void *const Class_Brick;

typedef struct BrickClass_s {
    const GameBodyClass base;
} BrickClass;


typedef struct Brick_s
{
    struct {
        GameBody base;
    } m_super;

    RE_Animator *m_animator;
} Brick;

void Brick_Constructor(void *self, void *scene, PE_Vec2 startPos);

#endif
