#ifndef _APPLICATION_INPUT_H_
#define _APPLICATION_INPUT_H_

#include "../Settings.h"
#include "InputGroup.h"

void Class_InitAppInput();

/// @brief Description de la classe "AppInput".
extern const void *const Class_AppInput;

typedef struct AppInputClass_s
{
    const InputGroupClass base;
} AppInputClass;

typedef struct AppInput_s
{
    struct {
        InputGroup base;
    } m_super;

    bool quitPressed;
    bool pausePressed;
} AppInput;

void AppInput_Constructor(void *self);

#endif
