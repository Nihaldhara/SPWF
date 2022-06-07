#include "MouseInput.h"

// Object virtual methods
void MouseInput_VM_Destructor(void *self);

// InputGroup virtual methods
void MouseInput_VM_OnEventProcess(void *self, SDL_Event evt);
void MouseInput_VM_OnPreEventProcess(void *self);
void MouseInput_VM_Reset(void *self);

static MouseInputClass _Class_MouseInput = { 0 };
const void *const Class_MouseInput = &_Class_MouseInput;

void Class_InitMouseInput()
{
    if (!Class_IsInitialized(Class_MouseInput))
    {
        Class_InitInputGroup();

        void *self = (void *)Class_MouseInput;
        ClassCtorParams params = {
            .self = self,
            .super = Class_InputGroup,
            .name = "MouseInput",
            .instanceSize = sizeof(MouseInput),
            .classSize = sizeof(MouseInputClass)
        };
        Class_Constructor(params, MouseInput_VM_Destructor);
        ((InputGroupClass *)self)->OnEventProcess = MouseInput_VM_OnEventProcess;
        ((InputGroupClass *)self)->OnPreEventProcess = MouseInput_VM_OnPreEventProcess;
        ((InputGroupClass *)self)->Reset = MouseInput_VM_Reset;
    }
}

void MouseInput_Constructor(void *self)
{
    InputGroup_Constructor(self);
    Object_SetClass(self, Class_MouseInput);

    MouseInput *mouse = Object_Cast(self, Class_MouseInput);
    MouseInput_VM_Reset(mouse);
}

void MouseInput_VM_Destructor(void *self)
{
    MouseInput *mouse = Object_Cast(self, Class_MouseInput);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_MouseInput);
}

void MouseInput_VM_OnEventProcess(void *self, SDL_Event evt)
{
    MouseInput *mouse = Object_Cast(self, Class_MouseInput);

    Uint8 mouseButton;
    switch (evt.type)
    {
    case SDL_MOUSEMOTION:
        mouse->viewPos = PE_Vec2_Set((float)evt.motion.x, (float)evt.motion.y);
        break;

    case SDL_MOUSEBUTTONDOWN:
        mouseButton = evt.button.button;

        switch (mouseButton)
        {
        case SDL_BUTTON_MIDDLE:
            mouse->middleClick = true;
            break;
        case SDL_BUTTON_LEFT:
            mouse->leftClickCount = evt.button.clicks;
            mouse->leftDown = true;
            break;
        case SDL_BUTTON_RIGHT:
            mouse->rightClickCount = evt.button.clicks;
            mouse->rightDown = true;
            break;
        default:
            break;
        }
        break;

    case SDL_MOUSEBUTTONUP:
        mouseButton = evt.button.button;

        switch (mouseButton)
        {
        case SDL_BUTTON_LEFT:
            mouse->leftDown = false;
            mouse->leftReleased = true;
            break;
        case SDL_BUTTON_RIGHT:
            mouse->rightDown = false;
            mouse->rightReleased = true;
            break;
        default:
            break;
        }
        break;

    case SDL_MOUSEWHEEL:
        mouse->wheel += evt.wheel.y;
        break;

    default:
        break;
    }
}

void MouseInput_VM_OnPreEventProcess(void *self)
{
    MouseInput *mouse = Object_Cast(self, Class_MouseInput);

    mouse->leftClickCount = 0;
    mouse->leftReleased = false;
    mouse->rightClickCount = 0;
    mouse->rightReleased = false;
    mouse->middleClick = false;
    mouse->wheel = 0;
    mouse->lastViewPos = mouse->viewPos;
}

void MouseInput_VM_Reset(void *self)
{
    MouseInput *mouse = Object_Cast(self, Class_MouseInput);

    mouse->lastViewPos = PE_Vec2_Zero;
    mouse->viewPos = PE_Vec2_Zero;

    mouse->leftClickCount = 0;
    mouse->leftDown = false;
    mouse->leftReleased = false;

    mouse->rightClickCount = 0;
    mouse->rightDown = false;
    mouse->rightReleased = false;

    mouse->middleClick = false;
    mouse->wheel = 0;
}