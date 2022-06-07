/// @file PE_Shape.h
/// @defgroup PE_Shape Fonctions relatives à la forme d'un collider

#ifndef _PE_SHAPE_H_
#define _PE_SHAPE_H_

#include "PE_Settings.h"
#include "PE_Math.h"
#include "PE_PolygonShape.h"
#include "PE_CircleShape.h"
#include "PE_CapsuleShape.h"

/// @ingroup PE_Shape
/// @brief Enumération donnant les formes possibles pour un collider.
typedef enum PE_ShapeType_e
{
    PE_SHAPE_CIRCLE  = 0,
    PE_SHAPE_POLYGON = 1,
    PE_SHAPE_POLYGON_BOX = 2,
    PE_SHAPE_CAPSULE = 3
} PE_ShapeType;

/// @ingroup PE_Shape
/// @brief Structure représentant la forme d'un collider
typedef struct PE_Shape_s
{
    int m_type;
    float m_skin;
    union {
        PE_PolygonShape polygon;
        PE_CircleShape circle;
        PE_CapsuleShape capsule;
    } m_data;
} PE_Shape;

/// @ingroup PE_Shape
/// @brief Définit une forme comme étant une boîte alignée sur les axes.
/// @param[out] shape la forme à initialiser.
/// @param[in] xLower abcisse du coin inférieur gauche de la boîte.
/// @param[in] yLower ordonnée du coin inférieur gauche de la boîte.
/// @param[in] xUpper abcisse du coin supérieur droit de la boîte.
/// @param[in] yUpper ordonnée du coin supérieur droit de la boîte.
PE_INLINE void PE_Shape_SetAsBox(
    PE_Shape *shape, float xLower, float yLower, float xUpper, float yUpper)
{
    shape->m_type = PE_SHAPE_POLYGON_BOX;
    shape->m_skin = PE_SKIN_RADIUS;

    PE_PolygonShape_SetAsBox(&shape->m_data.polygon, xLower, yLower, xUpper, yUpper);
}

PE_INLINE void PE_Shape_SetAsPolygon(
    PE_Shape *shape, PE_Vec2 *vertices, int vertexCount)
{
    shape->m_type = PE_SHAPE_POLYGON;
    shape->m_skin = PE_SKIN_RADIUS;

    PE_PolygonShape_Set(&shape->m_data.polygon, vertices, vertexCount);
}

PE_INLINE void PE_Shape_SetAsCircle(
    PE_Shape *shape, PE_Vec2 center, float radius)
{
    shape->m_type = PE_SHAPE_CIRCLE;
    shape->m_skin = PE_SKIN_RADIUS;

    PE_CircleShape_Set(&shape->m_data.circle, center, radius);
}

PE_INLINE void PE_Shape_SetAsCapsule(
    PE_Shape *shape, PE_Vec2 vertexA, PE_Vec2 vertexB, float radius)
{
    shape->m_type = PE_SHAPE_CAPSULE;
    shape->m_skin = PE_SKIN_RADIUS;

    PE_CapsuleShape_Set(&shape->m_data.capsule, vertexA, vertexB, radius);
}

// Methodes internes

PE_INLINE float PE_Shape_GetSkin(PE_Shape *shape)
{
    return shape->m_skin;
}

void PE_Shape_GetAABB(PE_Shape *shape, PE_Vec2 transform, PE_AABB *aabb);

/// @brief Renvoie le type d'une forme.
/// Les valeurs possible sont données dans l'énumération PE_ShapeType_e
/// @param[in] shape la forme.
/// @return Le type de la forme.
PE_INLINE int PE_Shape_GetType(PE_Shape *shape)
{
    return shape->m_type;
}

#endif
