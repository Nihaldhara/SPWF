#ifndef _CONTROLS_INPUT_H_
#define _CONTROLS_INPUT_H_

#include "../Settings.h"
#include "InputGroup.h"

void Class_InitControlsInput();

/// @brief Description de la classe "ControlsInput".
extern const void *const Class_ControlsInput;

typedef struct ControlsInputClass_s
{
    const InputGroupClass base;
} ControlsInputClass;

typedef struct ControlsInput_s
{
    struct {
        InputGroup base;
    } m_super;

    float hAxis;
    bool jumpDown;
    bool jumpPressed;
    bool goDownDown;
} ControlsInput;

void ControlsInput_Constructor(void *self);

#endif
