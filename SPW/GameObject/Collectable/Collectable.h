#ifndef _COLLECTABLE_H_
#define _COLLECTABLE_H_

#include "../GameBody.h"

void Class_InitCollectable();

/// @brief Description de la classe "Collectable".
extern const void *const Class_Collectable;

typedef struct CollectableClass_s {
    const GameBodyClass base;

    void (*Collect)(void *self, void *dst);
} CollectableClass;

typedef struct Collectable_s
{
    struct {
        GameBody base;
    } m_super;

} Collectable;

void Collectable_Constructor(void *self, void *scene, PE_Vec2 startPos);

INLINE void Collectable_Collect(void *self, void *dst)
{
    assert(Object_IsA(self, Class_Collectable));
    const CollectableClass *clsDesc = Object_GetClass(self);
    (clsDesc->Collect)(self, dst);
}

#endif
