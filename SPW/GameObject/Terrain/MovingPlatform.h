#ifndef _MOVING_PLATFORM_H_
#define _MOVING_PLATFORM_H_

#include "../GameBody.h"

void Class_InitMovingPlatform();

/// @brief Description de la classe "MovingPlatform".
extern const void *const Class_MovingPlatform;

typedef struct MovingPlatformClass_s {
    const GameBodyClass base;
} MovingPlatformClass;

typedef struct MovingPlatform_s
{
    struct {
        GameBody base;
    } m_super;

    PE_ColliderDef m_colliderDef;
    PE_Vec2 *m_points;
    int m_pointCount;
    int m_capacity;
    float m_speed;
    int m_index;
    int m_flags;
    bool m_forward;

    RE_AtlasPart *m_platformPart;
} MovingPlatform;

void MovingPlatform_Constructor(
    void *self, void *scene, PE_Vec2 position, PE_ColliderDef *colliderDef
);

void MovingPlatform_SetPointCount(void *self, int pointCount);

INLINE int MovingPlatform_GetPointCount(void *self)
{
    MovingPlatform *platform = Object_Cast(self, Class_MovingPlatform);
    return platform->m_pointCount;
}

INLINE PE_Vec2 *MovingPlatform_GetPoints(void *self)
{
    MovingPlatform *platform = Object_Cast(self, Class_MovingPlatform);
    return platform->m_points;
}

INLINE void MovingPlatform_SetSpeed(void *self, float speed)
{
    MovingPlatform *platform = Object_Cast(self, Class_MovingPlatform);
    platform->m_speed = speed;
}

INLINE float MovingPlatform_GetSpeed(void *self)
{
    MovingPlatform *platform = Object_Cast(self, Class_MovingPlatform);
    return platform->m_speed;
}

INLINE PE_Vec2 MovingPlatform_GetCollisionEnterImpulse(
    MovingPlatform *platform)
{
    PE_Body *body = GameBody_GetBody((GameBody *)platform);
    return PE_Vec2_Neg(PE_Body_GetVelocity(body));
}

INLINE PE_Vec2 MovingPlatform_GetCollisionExitImpulse(
    MovingPlatform *platform)
{
    PE_Body *body = GameBody_GetBody((GameBody *)platform);
    return PE_Body_GetVelocity(body);
}

#endif