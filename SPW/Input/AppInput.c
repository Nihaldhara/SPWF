#include "AppInput.h"

// Object virtual methods
void AppInput_VM_Destructor(void *self);

// InputGroup virtual methods
void AppInput_VM_OnEventProcess(void *self, SDL_Event evt);
void AppInput_VM_OnPreEventProcess(void *self);
void AppInput_VM_Reset(void *self);

static AppInputClass _Class_AppInput = { 0 };
const void *const Class_AppInput = &_Class_AppInput;

void Class_InitAppInput()
{
    if (!Class_IsInitialized(Class_AppInput))
    {
        Class_InitInputGroup();

        void *self = (void *)Class_AppInput;
        ClassCtorParams params = {
            .self = self,
            .super = Class_InputGroup,
            .name = "AppInput",
            .instanceSize = sizeof(AppInput),
            .classSize = sizeof(AppInputClass)
        };
        Class_Constructor(params, AppInput_VM_Destructor);
        ((InputGroupClass *)self)->OnEventProcess = AppInput_VM_OnEventProcess;
        ((InputGroupClass *)self)->OnPreEventProcess = AppInput_VM_OnPreEventProcess;
        ((InputGroupClass *)self)->Reset = AppInput_VM_Reset;
    }
}

void AppInput_Constructor(void *self)
{
    InputGroup_Constructor(self);
    Object_SetClass(self, Class_AppInput);

    AppInput *appInput = Object_Cast(self, Class_AppInput);
    AppInput_VM_Reset(appInput);
}

void AppInput_VM_Destructor(void *self)
{
    AppInput *appInput = Object_Cast(self, Class_AppInput);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_AppInput);
}

void AppInput_VM_OnEventProcess(void *self, SDL_Event evt)
{
    AppInput *appInput = Object_Cast(self, Class_AppInput);

    SDL_Scancode scanCode;
    switch (evt.type)
    {
    case SDL_QUIT:
        appInput->quitPressed = true;
        break;

    case SDL_KEYDOWN:
        scanCode = evt.key.keysym.scancode;

        if (evt.key.repeat)
            break;

        switch (scanCode)
        {
        case SDL_SCANCODE_ESCAPE:
            // Touche pour quiter le jeu
            appInput->quitPressed = true;
            break;

        case SDL_SCANCODE_RETURN:
            appInput->pausePressed = true;
            break;

        default:
            break;
        }
        break;
    default:
        break;
    }
}

void AppInput_VM_OnPreEventProcess(void *self)
{
    AppInput *appInput = Object_Cast(self, Class_AppInput);

    appInput->quitPressed = false;
    appInput->pausePressed = false;
}

void AppInput_VM_Reset(void *self)
{
    AppInput *appInput = Object_Cast(self, Class_AppInput);

    appInput->quitPressed = false;
    appInput->pausePressed = false;
}