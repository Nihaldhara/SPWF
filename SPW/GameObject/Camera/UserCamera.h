#ifndef _USER_CAMERA_H_
#define _USER_CAMERA_H_

#include "Camera.h"

void Class_InitUserCamera();

/// @brief Description de la classe "UserCamera".
extern const void *const Class_UserCamera;

typedef struct UserCameraClass_s {
    const CameraClass base;
} UserCameraClass;

typedef struct UserCamera_s
{
    struct {
        Camera base;
    } m_super;

    bool m_track;
    Camera *m_trackedCam;
    PE_Vec2 m_trackedLower;
} UserCamera;

void UserCamera_Constructor(
    void *self, void *scene,
    int width, int height, void *trackedCam
);

#endif