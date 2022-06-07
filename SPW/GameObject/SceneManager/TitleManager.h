#ifndef _TITLE_MANAGER_H_
#define _TITLE_MANAGER_H_

#include "../GameObject.h"

void Class_InitTitleManager();

/// @brief Description de la classe "TitleManager".
extern const void *const Class_TitleManager;

typedef struct TitleManagerClass_s {
    const GameObjectClass base;
} TitleManagerClass;

typedef struct TitleManager_s
{
    struct {
        GameObject base;
    } m_super;

} TitleManager;

void TitleManager_Constructor(void *self, void *scene);

#endif
