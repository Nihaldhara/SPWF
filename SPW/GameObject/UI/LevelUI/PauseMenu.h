#ifndef _GAME_MENU_H_
#define _GAME_MENU_H_

#include "../UIObject.h"

void Class_InitPauseMenu();

/// @brief Description de la classe "PauseMenu".
extern const void *const Class_PauseMenu;

typedef struct PauseMenuClass_s {
    const UIObjectClass base;
} PauseMenuClass;

typedef struct PauseMenu_s
{
    struct {
        UIObject base;
    } m_super;

} PauseMenu;

void PauseMenu_Constructor(void *self, void *scene);

#endif