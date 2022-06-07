#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../GameObject.h"

void Class_InitCamera();

/// @brief Description de la classe "Camera".
extern const void *const Class_Camera;

typedef struct CameraClass_s {
    const GameObjectClass base;
} CameraClass;

typedef struct Camera_s
{
    struct {
        GameObject base;
    } m_super;

    /// @brief Rectangle représentant la vue de la caméra.
    /// Ces coordonnées sont dans le référentiel monde.
    PE_AABB m_worldView;

    PE_AABB m_worldBounds;

    /// @brief Largeur en pixels de la caméra.
    int m_width;

    /// @brief Hauteur en pixels de la caméra.
    int m_height;
} Camera;

void Camera_Constructor(void *self, void *scene, int width, int height);

/// @brief Renvoie la zone du monde vue par la caméra.
/// @param[in] camera la caméra.
/// @param[out] worldView le rectangle vu par la caméra exprimé dans le référentiel monde.

INLINE PE_AABB Camera_GetWorldView(void *self)
{
    Camera *camera = Object_Cast(self, Class_Camera);
    return camera->m_worldView;
}

INLINE void Camera_SetWorldView(void *self, PE_AABB worldView)
{
    Camera *camera = Object_Cast(self, Class_Camera);
    camera->m_worldView = worldView;
}

void Camera_TranslateWorldView(void *self, PE_Vec2 displacement);

INLINE PE_AABB Camera_GetWorldBounds(void *self)
{
    Camera *camera = Object_Cast(self, Class_Camera);
    return camera->m_worldBounds;
}

INLINE void Camera_SetWorldBounds(void *self, PE_AABB bounds)
{
    Camera *camera = Object_Cast(self, Class_Camera);
    camera->m_worldBounds = bounds;
}

/// @brief Renvoie la largeur en pixels de la caméra.
/// @param[in] camera la caméra.
/// @return La largeur en pixels de la caméra.
int Camera_GetWidth(void *self);

/// @brief Renvoie la hauteur en pixels de la caméra.
/// @param[in] camera la caméra.
/// @return La hauteur en pixels de la caméra.
int Camera_GetHeight(void *self);

float Camera_GetWorldToViewScale(void *self);

/// @brief Transforme des coordonnées exprimées dans le référentiel monde vers le référentiel caméra.
/// @param[in] camera la caméra.
/// @param[in] position la position d'un point dans le référentiel monde.
/// @param[out] x l'abscisse du point dans le référentiel caméra (en pixels).
/// @param[out] y l'ordonnée du point dans le référentiel caméra (en pixels).
void Camera_WorldToView(void *self, PE_Vec2 position, float *x, float *y);

/// @brief Transforme des coordonnées exprimées dans le référentiel caméra vers le référentiel monde.
/// @param[in] camera camera la caméra.
/// @param[in] x l'abscisse d'un point dans le référentiel caméra (en pixels).
/// @param[in] y l'ordonnée d'un point dans le référentiel caméra (en pixels).
/// @param[out] position la position du point dans le référentiel monde.
void Camera_ViewToWorld(void *self, float x, float y, PE_Vec2 *position);

#endif