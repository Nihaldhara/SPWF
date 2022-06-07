#ifndef _PE_POLYGON_SHAPE_H_
#define _PE_POLYGON_SHAPE_H_

#include "PE_Settings.h"
#include "PE_Math.h"

typedef struct PE_PolygonShape_s
{
    PE_Vec2 m_vertices[PE_MAX_POLYGON_VERTICES];
    PE_Vec2 m_normals[PE_MAX_POLYGON_VERTICES];
    int m_vertexCount;
    PE_AABB m_aabb;
} PE_PolygonShape;

int PE_PolygonShape_Set(PE_PolygonShape *polygon, PE_Vec2 *vertices, int vertexCount);

int PE_PolygonShape_SetAsBox(
    PE_PolygonShape *polygon,
    float xLower, float yLower, float xUpper, float yUpper);

PE_INLINE void PE_PolygonShape_GetAABB(
    PE_PolygonShape *polygon, PE_Vec2 transform, PE_AABB *aabb)
{
    *aabb = polygon->m_aabb;
    PE_AABB_Translate(aabb, transform);
}

#endif