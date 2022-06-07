#ifndef _TITLE_MENU_H_
#define _TITLE_MENU_H_

#include "../UIObject.h"

void Class_InitStartScreen();

/// @brief Description de la classe "StartScreen".
extern const void *const Class_StartScreen;

typedef struct StartScreenClass_s {
    const UIObjectClass base;
} StartScreenClass;

typedef struct StartScreen_s
{
    struct {
        UIObject base;
    } m_super;

} StartScreen;

void StartScreen_Constructor(void *self, void *scene);

#endif