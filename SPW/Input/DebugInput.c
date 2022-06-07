#include "DebugInput.h"

// Object virtual methods
void DebugInput_VM_Destructor(void *self);

// InputGroup virtual methods
void DebugInput_VM_OnEventProcess(void *self, SDL_Event evt);
void DebugInput_VM_OnPreEventProcess(void *self);
void DebugInput_VM_Reset(void *self);

static DebugInputClass _Class_DebugInput = { 0 };
const void *const Class_DebugInput = &_Class_DebugInput;

void Class_InitDebugInput()
{
    if (!Class_IsInitialized(Class_DebugInput))
    {
        Class_InitInputGroup();

        void *self = (void *)Class_DebugInput;
        ClassCtorParams params = {
            .self = self,
            .super = Class_InputGroup,
            .name = "DebugInput",
            .instanceSize = sizeof(DebugInput),
            .classSize = sizeof(DebugInputClass)
        };
        Class_Constructor(params, DebugInput_VM_Destructor);
        ((InputGroupClass *)self)->OnEventProcess = DebugInput_VM_OnEventProcess;
        ((InputGroupClass *)self)->OnPreEventProcess = DebugInput_VM_OnPreEventProcess;
        ((InputGroupClass *)self)->Reset = DebugInput_VM_Reset;
    }
}

void DebugInput_Constructor(void *self)
{
    InputGroup_Constructor(self);
    Object_SetClass(self, Class_DebugInput);

    DebugInput *debug = Object_Cast(self, Class_DebugInput);
    DebugInput_VM_Reset(debug);
}

void DebugInput_VM_Destructor(void *self)
{
    DebugInput *debug = Object_Cast(self, Class_DebugInput);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_DebugInput);
}

void DebugInput_VM_OnEventProcess(void *self, SDL_Event evt)
{
    DebugInput *debug = Object_Cast(self, Class_DebugInput);

    SDL_Scancode scanCode;
    switch (evt.type)
    {
    case SDL_KEYDOWN:
        scanCode = evt.key.keysym.scancode;

        if (evt.key.repeat)
            break;

        switch (scanCode)
        {
        case SDL_SCANCODE_Q:
            debug->quitStepPressed = true;
            break;

        case SDL_SCANCODE_RCTRL:
            debug->infoPressed = true;
            break;

        case SDL_SCANCODE_TAB:
            // Nouveau pas
            debug->nextStepPressed = true;
            debug->nextStepDown = true;
            break;

        case SDL_SCANCODE_G:
            // Gizmos
            debug->gizmosPressed = true;
            break;

        case SDL_SCANCODE_P:
            // Body
            debug->bodyPressed = true;
            break;

        case SDL_SCANCODE_C:
            // Caméra
            debug->camPressed = true;
            break;

        case SDL_SCANCODE_T:
            // Camera traking
            debug->trackPressed = true;
            break;

        default:
            break;
        }
        break;

    case SDL_KEYUP:
        scanCode = evt.key.keysym.scancode;

        switch (scanCode)
        {
        case SDL_SCANCODE_TAB:
            // Nouveau pas
            debug->nextStepDown = false;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}

void DebugInput_VM_OnPreEventProcess(void *self)
{
    DebugInput *debug = Object_Cast(self, Class_DebugInput);

    debug->bodyPressed = false;
    debug->camPressed = false;
    debug->gizmosPressed = false;
    debug->infoPressed = false;
    debug->quitStepPressed = false;
    debug->nextStepPressed = false;
    debug->trackPressed = false;
}

void DebugInput_VM_Reset(void *self)
{
    DebugInput *debug = Object_Cast(self, Class_DebugInput);

    debug->bodyPressed = false;
    debug->camPressed = false;
    debug->gizmosPressed = false;
    debug->infoPressed = false;
    debug->quitStepPressed = false;
    debug->nextStepDown = false;
    debug->nextStepPressed = false;
    debug->quitStepPressed = false;
    debug->trackPressed = false;
}