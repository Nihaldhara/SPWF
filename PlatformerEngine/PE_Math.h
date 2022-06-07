/// @file PE_Math.h
/// @defgroup PE_Math Fonctions utilitaires

#ifndef _PE_MATH_H_
#define _PE_MATH_H_

#include "PE_Settings.h"

/// @ingroup PE_Math
/// @brief Structure représentant un vecteur 2D dont les composantes sont de type float.
typedef struct PE_Vec2_s {

    /// @brief Abscisse du vecteur.
    float x;

    /// @brief Ordonnée du vecteur.
    float y;
} PE_Vec2;

/// @ingroup PE_Math
/// @brief Initialise les coordonnées d'un vecteur.
/// @param[in,out] v le vecteur à initialiser.
/// @param[in] x l'abscisse du vecteur. 
/// @param[in] y l'ordonnée du vecteur.
PE_INLINE PE_Vec2 PE_Vec2_Set(float x, float y)
{
    PE_Vec2 v = { x, y };
    return v;
}

PE_INLINE PE_Vec2 PE_Vec2_Add(PE_Vec2 v1, PE_Vec2 v2)
{
    v1.x += v2.x;
    v1.y += v2.y;

    return v1;
}

PE_INLINE PE_Vec2 PE_Vec2_Scale(PE_Vec2 v, float alpha)
{
    v.x *= alpha;
    v.y *= alpha;

    return v;
}

PE_INLINE PE_Vec2 PE_Vec2_Sub(PE_Vec2 v1, PE_Vec2 v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;

    return v1;
}

PE_INLINE PE_Vec2 PE_Vec2_Mul(PE_Vec2 v1, PE_Vec2 v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;

    return v1;
}

PE_INLINE PE_Vec2 PE_Vec2_Min(PE_Vec2 v1, PE_Vec2 v2)
{
    v1.x = fminf(v1.x, v2.x);
    v1.y = fminf(v1.y, v2.y);

    return v1;
}

PE_INLINE PE_Vec2 PE_Vec2_Max(PE_Vec2 v1, PE_Vec2 v2)
{
    v1.x = fmaxf(v1.x, v2.x);
    v1.y = fmaxf(v1.y, v2.y);

    return v1;
}

PE_INLINE PE_Vec2 PE_Vec2_Abs(PE_Vec2 v)
{
    v.x = fabsf(v.x);
    v.y = fabsf(v.y);

    return v;
}

PE_INLINE PE_Vec2 PE_Vec2_Neg(PE_Vec2 v)
{
    v.x = -v.x;
    v.y = -v.y;

    return v;
}

PE_INLINE float PE_Vec2_Dot(PE_Vec2 v1, PE_Vec2 v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y);
}

PE_INLINE float PE_Vec2_LengthSqr(PE_Vec2 v)
{
    return PE_Vec2_Dot(v, v);
}

PE_INLINE float PE_Vec2_Length(PE_Vec2 v)
{
    return sqrtf(PE_Vec2_LengthSqr(v));
}

PE_INLINE PE_Vec2 PE_Vec2_Perp(PE_Vec2 v)
{
    return PE_Vec2_Set(-v.y, v.x);
}

PE_INLINE PE_Vec2 PE_Vec2_NegPerp(PE_Vec2 v)
{
    return PE_Vec2_Set(v.y, -v.x);
}

PE_INLINE PE_Vec2 PE_Vec2_PerpDir(PE_Vec2 v, PE_Vec2 direction)
{
    PE_Vec2 skew = PE_Vec2_Perp(v);
    if (PE_Vec2_Dot(skew, direction) >= 0.0f)
    {
        return skew;
    }
    else
    {
        return PE_Vec2_Neg(skew);
    }
}

PE_INLINE PE_Vec2 PE_Vec2_Normalize(PE_Vec2 v)
{
    float norm = PE_Vec2_Length(v);
    float invNorm = 1.0f / norm;

    v.x *= invNorm;
    v.y *= invNorm;

    return v;
}

PE_INLINE float PE_Vec2_Distance(PE_Vec2 v1, PE_Vec2 v2)
{
    return PE_Vec2_Length(PE_Vec2_Sub(v1, v2));
}

PE_INLINE float PE_Vec2_DistanceSqr(PE_Vec2 v1, PE_Vec2 v2)
{
    return PE_Vec2_LengthSqr(PE_Vec2_Sub(v1, v2));
}

PE_INLINE float PE_Vec2_Det(PE_Vec2 v1, PE_Vec2 v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

extern const PE_Vec2 PE_Vec2_Up;
extern const PE_Vec2 PE_Vec2_Down;
extern const PE_Vec2 PE_Vec2_Left;
extern const PE_Vec2 PE_Vec2_Right;
extern const PE_Vec2 PE_Vec2_Zero;
extern const PE_Vec2 PE_Vec2_One;

typedef struct PE_Mat2_s
{
    PE_Vec2 cols[2];
} PE_Mat2;

PE_INLINE PE_Vec2 PE_Mat2_MulMV(PE_Mat2 mat, PE_Vec2 v)
{
    PE_Vec2 res = { 0 };
    res.x = mat.cols[0].x * v.x + mat.cols[1].x * v.y;
    res.y = mat.cols[0].y * v.x + mat.cols[1].y * v.y;
    return res;
}

PE_INLINE PE_Vec2 PE_Mat2_MulMVT(PE_Mat2 mat, PE_Vec2 v)
{
    PE_Vec2 res = { 0 };
    res.x = mat.cols[0].x * v.x + mat.cols[0].y * v.y;
    res.y = mat.cols[1].x * v.x + mat.cols[1].y * v.y;
    return res;
}

/// @ingroup PE_Math
/// @brief Structure représentant une boîte englobante (rectangle) dont les côtés sont alignés sur les axes.
/// AABB signifie "Aligned Axes Bounding Box".
typedef struct PE_AABB_s {

    /// @brief Coordonnées du coin inférieur gauche de la boîte.
    PE_Vec2 lower;

    /// @brief Coordonnées du coin supérieur droit de la boîte.
    PE_Vec2 upper;
} PE_AABB;

/// @ingroup PE_Math
/// @brief Initialise une AABB.
/// @param[in,out] aabb la boîte a initialiser.
/// @param[in] xLower abcisse du coin inférieur gauche de la boîte.
/// @param[in] yLower ordonnée du coin inférieur gauche de la boîte.
/// @param[in] xUpper abcisse du coin supérieur droit de la boîte.
/// @param[in] yUpper ordonnée du coin supérieur droit de la boîte.
PE_INLINE PE_AABB PE_AABB_Set(float xLower, float yLower, float xUpper, float yUpper)
{
    PE_AABB aabb;
    aabb.lower = PE_Vec2_Set(xLower, yLower);
    aabb.upper = PE_Vec2_Set(xUpper, yUpper);
    return aabb;
}

bool PE_AABB_Overlap(PE_AABB *aabb1, PE_AABB *aabb2);

PE_INLINE PE_AABB PE_AABB_Union(PE_AABB aabb1, PE_AABB aabb2)
{
    PE_AABB res;
    res.lower = PE_Vec2_Min(aabb1.lower, aabb2.lower);
    res.upper = PE_Vec2_Max(aabb1.upper, aabb2.upper);
    return res;
}

PE_INLINE void PE_AABB_Translate(PE_AABB *aabb, PE_Vec2 transform)
{
    aabb->lower = PE_Vec2_Add(aabb->lower, transform);
    aabb->upper = PE_Vec2_Add(aabb->upper, transform);
}

float PE_AABB_Distance(PE_AABB *aabb1, PE_AABB *aabb2);

PE_INLINE float PE_AABB_GetPerimeter(PE_AABB *aabb)
{
    float dx = aabb->upper.x - aabb->lower.x;
    float dy = aabb->upper.y - aabb->lower.y;
    return 2.0f * (dx + dy);
}

PE_INLINE bool PE_AABB_Contains(PE_AABB *aabb1, PE_AABB *aabb2)
{
    bool result = true;
    result = result && (aabb1->lower.x <= aabb2->lower.x);
    result = result && (aabb1->lower.y <= aabb2->lower.y);
    result = result && (aabb1->upper.x >= aabb2->upper.x);
    result = result && (aabb1->upper.y >= aabb2->upper.y);
    return result;
}

PE_INLINE bool PE_AABB_ContainsPoint(PE_AABB aabb, PE_Vec2 point)
{
    bool d0 = point.x < aabb.lower.x;
    bool d1 = point.y < aabb.lower.y;
    bool d2 = point.x > aabb.upper.x;
    bool d3 = point.y > aabb.upper.y;
    return !(d0 || d1 | d2 | d3);
}

/// @brief Structure décrivant le mouvement d'un point pendant la mise à jour du monde.
typedef struct PE_Sweep_s
{
    /// @brief Position au ratio alpha0
    PE_Vec2 position0;

    /// @brief Position finale.
    PE_Vec2 position1;

    /// @brief Ratio entre la position avant et après la mise à jour du monde.
    float alpha0;
} PE_Sweep;

PE_INLINE PE_Vec2 PE_Sweep_GetTransform(PE_Sweep *sweep, float beta)
{
    return PE_Vec2_Add(
        PE_Vec2_Scale(sweep->position0, 1.0f - beta),
        PE_Vec2_Scale(sweep->position1, beta)
    );
}

PE_INLINE void PE_Sweep_Advance(PE_Sweep *sweep, float alpha)
{
    float beta = (alpha - sweep->alpha0) / (1.0f - sweep->alpha0);
    sweep->position0 = PE_Sweep_GetTransform(sweep, beta);
    sweep->alpha0 = alpha;
}

// avance les sweeps au même alpha0
// Renvoie le alpha0 commun
PE_INLINE float PE_Sweep_Synchronize(PE_Sweep *sweepA, PE_Sweep *sweepB)
{
    float alpha0 = sweepA->alpha0;
    if (sweepA->alpha0 < sweepB->alpha0)
    {
        alpha0 = sweepB->alpha0;
        PE_Sweep_Advance(sweepA, alpha0);
    }
    else if (sweepB->alpha0 < sweepA->alpha0)
    {
        alpha0 = sweepA->alpha0;
        PE_Sweep_Advance(sweepB, alpha0);
    }
    return alpha0;
}

typedef union PE_Binary32_u
{
    float x;
    uint32 binary;
} PE_Binary32;

PE_INLINE bool PE_Math_SameSign(float a, float b)
{
    PE_Binary32 binaryA; binaryA.x = a;
    PE_Binary32 binaryB; binaryB.x = b;
    return !((binaryA.binary ^ binaryB.binary) & ((uint32)1 << 31));
}

#endif
