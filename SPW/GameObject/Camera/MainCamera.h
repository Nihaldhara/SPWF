#ifndef _MAIN_CAMERA_H_
#define _MAIN_CAMERA_H_

#include "Camera.h"

void Class_InitMainCamera();

/// @brief Description de la classe "MainCamera".
extern const void *const Class_MainCamera;

typedef struct MainCameraClass_s {
    const CameraClass base;
} MainCameraClass;

typedef struct MainCamera_s
{
    struct {
        Camera base;
    } m_super;

    PE_Vec2 m_smoothedTarget;
    PE_Vec2 m_centerVelocity;

} MainCamera;

void MainCamera_Constructor(void *self, void *scene, int width, int height);


#endif
