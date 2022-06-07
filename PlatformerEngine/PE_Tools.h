#ifndef _PE_TOOLS_H_
#define _PE_TOOLS_H_

#include "PE_Settings.h"
#include "PE_Math.h"

#define PE_RAD_TO_DEG 57.2957795130823f
#define PE_FLOAT_EPSILON 1e-5f;

float PE_Vec2_SignedAngleDeg(PE_Vec2 from, PE_Vec2 to);
float PE_Vec2_SignedAngleRad(PE_Vec2 from, PE_Vec2 to);
float PE_Vec2_AngleDeg(PE_Vec2 from, PE_Vec2 to);
float PE_Vec2_AngleRad(PE_Vec2 from, PE_Vec2 to);

PE_Vec2 PE_Vec2_Lerp(PE_Vec2 from, PE_Vec2 to, float t);


PE_Vec2 PE_Vec2_SmoothDamp(
    PE_Vec2 current, PE_Vec2 target, PE_Vec2 *currentVelocity,
    float smoothTime, float maxSpeed, float deltaTime);

PE_INLINE int Int_Max(int a, int b)
{
    return (a > b) ? a : b;
}

PE_INLINE int Int_Min(int a, int b)
{
    return (a < b) ? a : b;
}

PE_INLINE int Int_Clamp(int value, int a, int b)
{
    return Int_Max(a, Int_Min(value, b));
}

PE_INLINE float Float_Clamp(float value, float a, float b)
{
    return fmaxf(a, fminf(value, b));
}

PE_INLINE float Float_Clamp01(float value)
{
    return Float_Clamp(value, 0.0f, 1.0f);
}

PE_INLINE float Float_Lerp(float a, float b, float t)
{
    t = Float_Clamp01(t);
    return (1.0f - t) * a + t * b;
}

PE_INLINE float Float_LerpUnclamped(float a, float b, float t)
{
    return (1.0f - t) * a + t * b;
}

float Float_SmoothDamp(
    float current, float target, float *currentVelocity,
    float smoothTime, float maxSpeed, float deltaTime);

PE_INLINE bool Float_EpsEq(float value1, float value2)
{
    return fabsf(value1 - value2) < PE_FLOAT_EPSILON;
}

PE_INLINE float Float_Sign(float value)
{
    return value < 0.0f ? -1.0f : 1.0f;
}

bool PE_Line_Intersect(
    PE_Vec2 p1, PE_Vec2 vec1,
    PE_Vec2 p2, PE_Vec2 vec2, PE_Vec2 *intersection);

PE_INLINE PE_Vec2 PE_AABB_GetCenter(PE_AABB aabb)
{
    return PE_Vec2_Scale(PE_Vec2_Add(aabb.lower, aabb.upper), 0.5f);
}

PE_AABB PE_AABB_Scale(PE_AABB aabb, PE_Vec2 center, float scale);
#endif
