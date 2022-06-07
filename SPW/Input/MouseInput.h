#ifndef _MOUSE_INPUT_H_
#define _MOUSE_INPUT_H_

#include "../Settings.h"
#include "InputGroup.h"

void Class_InitMouseInput();

/// @brief Description de la classe "MouseInput".
extern const void *const Class_MouseInput;

typedef struct MouseInputClass_s
{
    const InputGroupClass base;
} MouseInputClass;

typedef struct MouseInput_s
{
    struct {
        InputGroup base;
    } m_super;

    // Position de la souris
    PE_Vec2 viewPos;
    PE_Vec2 lastViewPos;

    // Clic gauche
    int  leftClickCount;
    bool leftDown;
    bool leftReleased;

    // Clic droit
    int  rightClickCount;
    bool rightDown;
    bool rightReleased;

    // Roulette
    bool middleClick;
    int  wheel;
} MouseInput;

void MouseInput_Constructor(void *self);

#endif
