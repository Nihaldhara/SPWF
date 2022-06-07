#ifndef _TITLE_CAMERA_H_
#define _TITLE_CAMERA_H_

#include "Camera.h"

void Class_InitTitleCamera();

/// @brief Description de la classe "TitleCamera".
extern const void *const Class_TitleCamera;

typedef struct TitleCameraClass_s {
    const CameraClass base;
} TitleCameraClass;

typedef struct TitleCamera_s
{
    struct {
        Camera base;
    } m_super;

} TitleCamera;

void TitleCamera_Constructor(void *self, void *scene, int width, int height);

#endif
