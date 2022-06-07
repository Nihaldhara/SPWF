#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include "Scene.h"
#include "../GameObject/SceneManager/TitleManager.h"
#include "../GameObject/UI/TitleUI/TitleCanvas.h"

void Class_InitTitleScene();

/// @brief Description de la classe "TitleScene".
extern const void *const Class_TitleScene;

typedef struct TitleSceneClass_s
{
    const SceneClass base;
} TitleSceneClass;

typedef struct TitleScene_s
{
    struct {
        Scene base;
    } m_super;

    TitleCanvas *m_canvas;
    TitleManager *m_manager;

    int m_selectedLevel;
} TitleScene;

void TitleScene_Constructor(void *self, SDL_Renderer *renderer);

INLINE TitleCanvas *TitleScene_GetCanvas(void *self)
{
    TitleScene *scene = Object_Cast(self, Class_TitleScene);
    return scene->m_canvas;
}

INLINE TitleManager *TitleScene_GetManager(void *self)
{
    TitleScene *scene = Object_Cast(self, Class_TitleScene);
    return scene->m_manager;
}

INLINE void TitleScene_SetSelectedLevel(void *self, int levelIdx)
{
    TitleScene *scene = Object_Cast(self, Class_TitleScene);
    scene->m_selectedLevel = levelIdx;
}

INLINE int TitleScene_GetSelectedLevel(void *self)
{
    TitleScene *scene = Object_Cast(self, Class_TitleScene);
    return scene->m_selectedLevel;
}

#endif
