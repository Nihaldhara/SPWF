
/// @file RE_Utils.h
/// @defgroup RE_Utils Fonctions utilitaires

#ifndef _RE_UTILS_H_
#define _RE_UTILS_H_

#include "RE_Settings.h"

/// @ingroup RE_Utils
/// @brief Structure représenant un rectangle 2D alligné sur les axes X et Y.
typedef struct Rect_s
{
    /// @brief Abscisse du sommet en bas à gauche du rectangle.
    float x;

    /// @brief Ordonnée du sommet en bas à gauche du rectangle.
    float y;

    /// @brief Largeur du rectangle.
    float w;

    /// @brief Hauteur du rectangle.
    float h;
} Rect;

/// @ingroup RE_Utils
/// @brief Construit un rectangle.
/// @param[in] x abscisse du sommet en bas à gauche du rectangle.
/// @param[in] y ordonnée du sommet en bas à gauche du rectangle.
/// @param[in] w largeur du rectangle.
/// @param[in] h hauteur du rectangle.
/// @return Le rectangle spécifié.
Rect Rect_set(float x, float y, float w, float h);

/// @ingroup RE_Utils
/// @brief Structure représentant un vecteur 2D dont les composantes sont de type float.
typedef struct Vec2_s
{
    /// @brief Abscisse du vecteur.
    float x;

    /// @brief Ordonnée du vecteur.
    float y;
} Vec2;

/// @ingroup RE_Utils
/// @brief Construit un vecteur à partir de ses composantes.
/// @param[in] x la composante x.
/// @param[in] y la composante y.
/// @return Le vecteur ayant les composantes données.
RE_INLINE Vec2 Vec2_Set(float x, float y)
{
    Vec2 v = { .x = x, .y = y };
    return v;
}

RE_INLINE Vec2 Vec2_Add(Vec2 v1, Vec2 v2)
{
    v1.x += v2.x;
    v1.y += v2.y;

    return v1;
}

RE_INLINE Vec2 Vec2_Sub(Vec2 v1, Vec2 v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;

    return v1;
}

RE_INLINE Vec2 Vec2_Scale(Vec2 v, float alpha)
{
    v.x *= alpha;
    v.y *= alpha;

    return v;
}

RE_INLINE float RE_Float_Lerp(float a, float b, float t)
{
    t = fmaxf(0.0f, fminf(t, 1.0f));
    return (1.0f - t) * a + t * b;
}


RE_INLINE Vec2 Vec2_Lerp(Vec2 from, Vec2 to, float t)
{
    t = fmaxf(0.0f, fminf(t, 1.0f));
    float s = 1.0f - t;
    Vec2 res = {
        .x = s * from.x + t * to.x,
        .y = s * from.y + t * to.y
    };
    return res;
}

/// @ingroup RE_Utils
/// @brief Indique si un rectangle contient un point.
/// @param[in] rect le rectangle.
/// @param[in] point le point.
/// @return TRUE si le rectangle contient le point, FALSE sinon.
RE_INLINE int RE_Rect_contains(Rect *rect, Vec2 *point)
{
    return (point->x >= rect->x) && (point->x <= rect->x + rect->w)
        && (point->y >= rect->y) && (point->y <= rect->y + rect->h);
}

RE_INLINE bool RE_FRect_Contains(SDL_FRect *rect, SDL_FPoint *point)
{
    return (point->x >= rect->x) && (point->x <= rect->x + rect->w)
        && (point->y >= rect->y) && (point->y <= rect->y + rect->h);
}

#endif
