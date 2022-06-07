#include "ControlsInput.h"

// Object virtual methods
void ControlsInput_VM_Destructor(void *self);

// InputGroup virtual methods
void ControlsInput_VM_OnEventProcess(void *self, SDL_Event evt);
void ControlsInput_VM_OnPreEventProcess(void *self);
void ControlsInput_VM_Reset(void *self);

static ControlsInputClass _Class_ControlsInput = { 0 };
const void *const Class_ControlsInput = &_Class_ControlsInput;

void Class_InitControlsInput()
{
    if (!Class_IsInitialized(Class_ControlsInput))
    {
        Class_InitInputGroup();

        void *self = (void *)Class_ControlsInput;
        ClassCtorParams params = {
            .self = self,
            .super = Class_InputGroup,
            .name = "ControlsInput",
            .instanceSize = sizeof(ControlsInput),
            .classSize = sizeof(ControlsInputClass)
        };
        Class_Constructor(params, ControlsInput_VM_Destructor);
        ((InputGroupClass *)self)->OnEventProcess = ControlsInput_VM_OnEventProcess;
        ((InputGroupClass *)self)->OnPreEventProcess = ControlsInput_VM_OnPreEventProcess;
        ((InputGroupClass *)self)->Reset = ControlsInput_VM_Reset;
    }
}

void ControlsInput_Constructor(void *self)
{
    InputGroup_Constructor(self);
    Object_SetClass(self, Class_ControlsInput);

    ControlsInput *controls = Object_Cast(self, Class_ControlsInput);
    ControlsInput_VM_Reset(controls);
}

void ControlsInput_VM_Destructor(void *self)
{
    ControlsInput *controls = Object_Cast(self, Class_ControlsInput);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_ControlsInput);
}

void ControlsInput_VM_OnEventProcess(void *self, SDL_Event evt)
{
    ControlsInput *controls = Object_Cast(self, Class_ControlsInput);

    SDL_Scancode scanCode;
    switch (evt.type)
    {
    case SDL_KEYDOWN:
        scanCode = evt.key.keysym.scancode;

        if (evt.key.repeat)
            break;

        switch (scanCode)
        {
        case SDL_SCANCODE_RIGHT:
            // Deplacement à droite
            controls->hAxis = 1.f;
            break;

        case SDL_SCANCODE_LEFT:
            // Deplacement à gauche
            controls->hAxis = -1.f;
            break;

        case SDL_SCANCODE_DOWN:
            // Descente d'une plateforme
            controls->goDownDown = true;
            break;

        case SDL_SCANCODE_SPACE:
        case SDL_SCANCODE_UP:
            // Saut
            controls->jumpDown = true;
            controls->jumpPressed = true;
            break;

        default:
            break;
        }
        break;

    case SDL_KEYUP:
        scanCode = evt.key.keysym.scancode;

        switch (scanCode)
        {
        case SDL_SCANCODE_RIGHT:
            // Deplacement à droite
            if (controls->hAxis > 0.f)
                controls->hAxis = 0.f;
            break;

        case SDL_SCANCODE_LEFT:
            // Deplacement à gauche
            if (controls->hAxis < 0.f)
                controls->hAxis = 0.f;
            break;

        case SDL_SCANCODE_DOWN:
            // Descente d'une plateforme
            controls->goDownDown = false;
            break;

        case SDL_SCANCODE_SPACE:
        case SDL_SCANCODE_UP:
            // Saut
            controls->jumpDown = false;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}

void ControlsInput_VM_OnPreEventProcess(void *self)
{
    ControlsInput *controls = Object_Cast(self, Class_ControlsInput);

    controls->jumpPressed = false;
}

void ControlsInput_VM_Reset(void *self)
{
    ControlsInput *controls = Object_Cast(self, Class_ControlsInput);

    controls->goDownDown = false;
    controls->hAxis = 0.0f;
    controls->jumpDown = false;
    controls->jumpPressed = false;
}