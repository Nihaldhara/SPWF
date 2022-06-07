
#ifndef _PE_SHAPE_COLLISION_H_
#define _PE_SHAPE_COLLISION_H_

#include "PE_Settings.h"
#include "PE_Shape.h"
#include "PE_Math.h"

/// @brief Structure stockant un contact entre deux formes.
typedef struct PE_Manifold_s
{
    PE_Vec2 normal;
    float depth;
    bool overlap;
} PE_Manifold;

PE_INLINE void PE_Manifold_Reset(PE_Manifold *manifold)
{
    manifold->overlap = false;
}
PE_INLINE void PE_Manifold_Reverse(PE_Manifold *manifold)
{
    manifold->normal = PE_Vec2_Neg(manifold->normal);
}

void PE_Shape_Collide(
    PE_Shape *shapeA, PE_Vec2 transformA,
    PE_Shape *shapeB, PE_Vec2 transformB, PE_Manifold *manifold);

bool PE_Shape_Overlap(
    PE_Shape *shapeA, PE_Vec2 transformA,
    PE_Shape *shapeB, PE_Vec2 transformB);

#endif
