#ifndef _INPUT_GROUP_H_
#define _INPUT_GROUP_H_

#include "../Settings.h"
#include "../Common.h"

void Class_InitInputGroup();

/// @brief Description de la classe "InputGroup".
extern const void *const Class_InputGroup;

typedef struct InputGroupClass_s
{
    const Class base;

    // Messages
    void (*OnPreEventProcess)(void *self);
    void (*OnEventProcess)(void *self, SDL_Event evt);
    void (*Reset)(void *self);
} InputGroupClass;

typedef struct InputGroup_s
{
    struct {
        Object base;
    } m_super;

    bool m_enabled;
} InputGroup;

void InputGroup_Constructor(void *self);

INLINE void InputGroup_OnPreEventProcess(void *self)
{
    InputGroup *inputGrp = Object_Cast(self, Class_InputGroup);
    if (inputGrp->m_enabled)
    {
        const InputGroupClass *clsDesc = Object_GetClass(self);
        (clsDesc->OnPreEventProcess)(self);
    }
}

INLINE void InputGroup_OnEventProcess(void *self, SDL_Event evt)
{
    InputGroup *inputGrp = Object_Cast(self, Class_InputGroup);
    if (inputGrp->m_enabled)
    {
        const InputGroupClass *clsDesc = Object_GetClass(self);
        (clsDesc->OnEventProcess)(self, evt);
    }
}

INLINE void InputGroup_Reset(void *self)
{
    InputGroup *inputGrp = Object_Cast(self, Class_InputGroup);
    const InputGroupClass *clsDesc = Object_GetClass(self);
    (clsDesc->Reset)(self);
}

INLINE void InputGroup_SetEnabled(void *self, bool enabled)
{
    InputGroup *inputGrp = Object_Cast(self, Class_InputGroup);
    if (inputGrp->m_enabled != enabled)
    {
        inputGrp->m_enabled = enabled;
        InputGroup_Reset(self);
    }
}

INLINE bool InputGroup_IsEnabled(void *self)
{
    InputGroup *inputGrp = Object_Cast(self, Class_InputGroup);
    return inputGrp->m_enabled;
}

#endif
