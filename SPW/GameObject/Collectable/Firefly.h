#ifndef _FIREFLY_H_
#define _FIREFLY_H_

#include "Collectable.h"

void Class_InitFirefly();

/// @brief Description de la classe "Firefly".
extern const void *const Class_Firefly;

typedef struct FireflyClass_s {
    const CollectableClass base;
} FireflyClass;

typedef struct Firefly_s
{
    struct {
        Collectable base;
    } m_super;

    RE_Animator *m_animator;

} Firefly;

void Firefly_Constructor(void *self, void *scene, PE_Vec2 position);

#endif
