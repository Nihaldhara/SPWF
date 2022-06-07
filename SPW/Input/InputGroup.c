#include "InputGroup.h"

// Object virtual methods
void InputGroup_VM_Destructor(void *self);

// InputGroup virtual methods
void InputGroup_VM_OnEventProcess(void *self, SDL_Event evt);
void InputGroup_VM_OnPreEventProcess(void *self);
void InputGroup_VM_Reset(void *self);

static InputGroupClass _Class_InputGroup = { 0 };
const void *const Class_InputGroup = &_Class_InputGroup;

void Class_InitInputGroup()
{
    if (!Class_IsInitialized(Class_InputGroup))
    {
        Class_InitObject();

        void *self = (void *)Class_InputGroup;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Object,
            .name = "InputGroup",
            .instanceSize = sizeof(InputGroup),
            .classSize = sizeof(InputGroupClass)
        };
        Class_Constructor(params, InputGroup_VM_Destructor);
        ((InputGroupClass *)self)->OnEventProcess = InputGroup_VM_OnEventProcess;
        ((InputGroupClass *)self)->OnPreEventProcess = InputGroup_VM_OnPreEventProcess;
        ((InputGroupClass *)self)->Reset = InputGroup_VM_Reset;
    }
}

void InputGroup_Constructor(void *self)
{
    Object_Constructor(self);
    Object_SetClass(self, Class_InputGroup);

    InputGroup *inputGrp = Object_Cast(self, Class_InputGroup);
    inputGrp->m_enabled = true;
}

void InputGroup_VM_Destructor(void *self)
{
    InputGroup *inputGrp = Object_Cast(self, Class_InputGroup);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_InputGroup);
}

void InputGroup_VM_OnEventProcess(void *self, SDL_Event evt)
{
}

void InputGroup_VM_OnPreEventProcess(void *self)
{
}

void InputGroup_VM_Reset(void *self)
{
}