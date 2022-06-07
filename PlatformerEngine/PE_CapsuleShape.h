#ifndef _PE_CAPSULE_SHAPE_H_
#define _PE_CAPSULE_SHAPE_H_

#include "PE_Settings.h"
#include "PE_Math.h"

typedef struct PE_CapsuleShape_s
{
    PE_Vec2 m_vertices[2];
    PE_Vec2 m_normals[2];
    float m_radius;
    PE_AABB m_aabb;
} PE_CapsuleShape;

int PE_CapsuleShape_Set(PE_CapsuleShape *capsule, PE_Vec2 vertexA, PE_Vec2 vertexB, float radius);

PE_INLINE void PE_CapsuleShape_GetAABB(
    PE_CapsuleShape *capsule, PE_Vec2 transform, PE_AABB *aabb)
{
    *aabb = capsule->m_aabb;
    PE_AABB_Translate(aabb, transform);
}

#endif
