#ifndef _TITLE_LEVEL_SELECT_H_
#define _TITLE_LEVEL_SELECT_H_

#include "../UIObject.h"

void Class_InitLevelSelection();

/// @brief Description de la classe "LevelSelection".
extern const void *const Class_LevelSelection;

typedef struct LevelSelectionClass_s {
    const UIObjectClass base;
} LevelSelectionClass;

typedef struct LevelSelection_s
{
    struct {
        UIObject base;
    } m_super;

    int m_levelCount;
    int *m_levelIndices;
} LevelSelection;

void LevelSelection_Constructor(void *self, void *scene);

#endif
