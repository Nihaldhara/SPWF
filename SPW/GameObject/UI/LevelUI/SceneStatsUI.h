#ifndef _DEBUG_PANEL_H_
#define _DEBUG_PANEL_H_

#include "../UIObject.h"
#include "../Text.h"
#include "../Image.h"

void Class_InitSceneStatsUI();

/// @brief Description de la classe "SceneStatsUI".
extern const void *const Class_SceneStatsUI;

typedef struct SceneStatsUIClass_s {
    const UIObjectClass base;
} SceneStatsUIClass;

typedef struct SceneStatsUI_s
{
    struct {
        UIObject base;
    } m_super;

    Text *m_objectCountText;
    Text *m_disabledCountText;
    Text *m_bodyCountText;
    Text *m_visibleCountText;
    Text *m_uiCountText;
} SceneStatsUI;

void SceneStatsUI_Constructor(void *self, void *scene);

#endif