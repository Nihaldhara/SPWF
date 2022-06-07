#include "InputManager.h"

InputManager *InputManager_New()
{
    InputManager *manager = (InputManager *)calloc(1, sizeof(InputManager));
    AssertNew(manager);

    // AppInput
    AppInput *app = (AppInput *)calloc(1, sizeof(AppInput));
    AssertNew(app);
    AppInput_Constructor(app);
    manager->m_groups[INPUT_APPLICATION] = Object_Cast(app, Class_InputGroup);

    // ControlsInput
    ControlsInput *controls = (ControlsInput *)calloc(1, sizeof(ControlsInput));
    AssertNew(controls);
    ControlsInput_Constructor(controls);
    manager->m_groups[INPUT_CONTROLS] = Object_Cast(controls, Class_InputGroup);

    // MouseInput
    MouseInput *mouse = (MouseInput *)calloc(1, sizeof(MouseInput));
    AssertNew(mouse);
    MouseInput_Constructor(mouse);
    manager->m_groups[INPUT_MOUSE] = Object_Cast(mouse, Class_InputGroup);

    // DebugInput
    DebugInput *debug = (DebugInput *)calloc(1, sizeof(DebugInput));
    AssertNew(debug);
    DebugInput_Constructor(debug);
    manager->m_groups[INPUT_DEBUG] = Object_Cast(debug, Class_InputGroup);

    return manager;
}

void InputManager_Delete(InputManager *self)
{
    if (!self)
        return;

    for (int i = 0; i < INPUT_TYPE_COUNT; ++i)
    {
        if (self->m_groups[i])
        {
            Object_Delete(self->m_groups[i]);
            self->m_groups[i] = NULL;
        }
    }

    free(self);
}

void InputManager_ProcessEvents(InputManager *self)
{
    for (int i = 0; i < INPUT_TYPE_COUNT; ++i)
    {
        InputGroup_OnPreEventProcess(self->m_groups[i]);
    }

    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        for (int i = 0; i < INPUT_TYPE_COUNT; ++i)
        {
            InputGroup_OnEventProcess(self->m_groups[i], evt);
        }
    }
}
