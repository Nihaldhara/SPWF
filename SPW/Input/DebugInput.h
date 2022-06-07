#ifndef _DEBUG_INPUT_H_
#define _DEBUG_INPUT_H_

#include "../Settings.h"
#include "InputGroup.h"

void Class_InitDebugInput();

/// @brief Description de la classe "DebugInput".
extern const void *const Class_DebugInput;

typedef struct DebugInputClass_s
{
    const InputGroupClass base;
} DebugInputClass;

typedef struct DebugInput_s
{
    struct {
        InputGroup base;
    } m_super;

    // Mode pas-à-pas
    bool nextStepDown;
    bool nextStepPressed;
    bool quitStepPressed;

    // Informations
    bool infoPressed;
    bool gizmosPressed;
    bool bodyPressed;

    // Changement de caméra
    bool trackPressed;
    bool camPressed;
} DebugInput;

void DebugInput_Constructor(void *self);

#endif
