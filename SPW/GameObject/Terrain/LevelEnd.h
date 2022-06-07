#ifndef _LEVEL_END_H_
#define _LEVEL_END_H_

#include "../GameBody.h"

void Class_InitLevelEnd();

/// @brief Description de la classe "LevelEnd".
extern const void *const Class_LevelEnd;

typedef struct LevelEndClass_s {
    const GameBodyClass base;
} LevelEndClass;


typedef struct LevelEnd_s
{
    struct {
        GameBody base;
    } m_super;

    bool m_isActive;
    RE_Animator *m_animator;
} LevelEnd;

void LevelEnd_Constructor(void *self, void *scene, PE_Vec2 position);

#endif
