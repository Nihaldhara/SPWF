#ifndef _INFO_PANEL_H_
#define _INFO_PANEL_H_

#include "../UIObject.h"
#include "../Text.h"
#include "../Image.h"
#include "../../Player.h"

void Class_InitLevelHeader();

/// @brief Description de la classe "LevelHeader".
extern const void *const Class_LevelHeader;

typedef struct LevelHeaderClass_s {
    const UIObjectClass base;
} LevelHeaderClass;

typedef struct LevelHeader_s
{
    struct {
        UIObject base;
    } m_super;

    Text *m_fireflyCountText;

} LevelHeader;

void LevelHeader_Constructor(void *self, void *scene);

#endif