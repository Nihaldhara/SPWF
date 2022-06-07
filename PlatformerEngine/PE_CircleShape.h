#ifndef _PE_CIRCLE_SHAPE_H_
#define _PE_CIRCLE_SHAPE_H_

#include "PE_Settings.h"
#include "PE_Math.h"

typedef struct PE_CircleShape_s
{
    PE_Vec2 m_center;
    float m_radius;
} PE_CircleShape;

int PE_CircleShape_Set(PE_CircleShape *circle, PE_Vec2 center, float radius);

PE_INLINE void PE_CircleShape_GetAABB(
    PE_CircleShape *circle, PE_Vec2 transform, PE_AABB *aabb)
{
    PE_Vec2 center = PE_Vec2_Add(circle->m_center, transform);
    float radius = circle->m_radius;

    aabb->lower = PE_Vec2_Set(center.x - radius, center.y - radius);
    aabb->upper = PE_Vec2_Set(center.x + radius, center.y + radius);
}

#endif
