#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include "../Settings.h"
#include "../Common.h"
#include "InputGroup.h"
#include "ControlsInput.h"
#include "DebugInput.h"
#include "MouseInput.h"
#include "AppInput.h"

typedef enum InputTypes_e
{
    INPUT_APPLICATION,
    INPUT_CONTROLS,
    INPUT_MOUSE,
    INPUT_DEBUG,
    INPUT_TYPE_COUNT
} InputTypes;

typedef struct InputManager_s
{
    InputGroup *m_groups[INPUT_TYPE_COUNT];
} InputManager;

InputManager* InputManager_New();
void InputManager_Delete(InputManager *self);

void InputManager_ProcessEvents(InputManager *self);

INLINE AppInput *InputManager_GetApplication(InputManager *self)
{
    return Object_Cast(self->m_groups[INPUT_APPLICATION], Class_AppInput);
}

INLINE ControlsInput *InputManager_GetControls(InputManager *self)
{
    return Object_Cast(self->m_groups[INPUT_CONTROLS], Class_ControlsInput);
}

INLINE MouseInput *InputManager_GetMouse(InputManager *self)
{
    return Object_Cast(self->m_groups[INPUT_MOUSE], Class_MouseInput);
}

INLINE DebugInput *InputManager_GetDebug(InputManager *self)
{
    return Object_Cast(self->m_groups[INPUT_DEBUG], Class_DebugInput);
}

#endif
