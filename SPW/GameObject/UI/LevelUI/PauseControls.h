#ifndef _PAUSE_CONTROLS_H_
#define _PAUSE_CONTROLS_H_

#include "../UIObject.h"

void Class_InitPauseControls();

/// @brief Description de la classe "PauseControls".
extern const void *const Class_PauseControls;

typedef struct PauseControlsClass_s {
    const UIObjectClass base;
} PauseControlsClass;

typedef struct PauseControls_s
{
    struct {
        UIObject base;
    } m_super;

} PauseControls;

void PauseControls_Constructor(void *self, void *scene);

#endif