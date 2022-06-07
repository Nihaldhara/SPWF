#ifndef _LEVEL_SCENE_H_
#define _LEVEL_SCENE_H_

#include "Scene.h"
#include "../GameObject/UI/LevelUI/LevelCanvas.h"
#include "../GameObject/SceneManager/LevelManager.h"

void Class_InitLevelScene();

/// @brief Description de la classe "LevelScene".
extern const void *const Class_LevelScene;

typedef struct LevelSceneClass_s
{
    const SceneClass base;
} LevelSceneClass;

typedef struct LevelScene_s
{
    struct {
        Scene base;
    } m_super;

    int m_levelIdx;
    Camera *m_cameras[2];
    Player *m_player;
    LevelCanvas *m_canvas;
    LevelManager *m_manager;

    PE_Vec2 m_startPos;
} LevelScene;

void LevelScene_Constructor(void *self, SDL_Renderer *renderer, int levelIdx);

INLINE PE_Vec2 LevelScene_GetStartPosition(void *self)
{
    LevelScene *scene = Object_Cast(self, Class_LevelScene);
    return scene->m_startPos;
}

INLINE void LevelScene_SetStartPosition(void *self, PE_Vec2 startPos)
{
    LevelScene *scene = Object_Cast(self, Class_LevelScene);
    scene->m_startPos = startPos;
}

INLINE LevelCanvas *LevelScene_GetCanvas(void *self)
{
    LevelScene *scene = Object_Cast(self, Class_LevelScene);
    return scene->m_canvas;
}

INLINE void LevelScene_ChangeCamera(void *self, int index)
{
    LevelScene *lvlScene = Object_Cast(self, Class_LevelScene);
    assert(0 <= index && index < 2);
    Scene_SetActiveCamera(self, lvlScene->m_cameras[index]);
}

INLINE Camera* LevelScene_GetCamera(void *self, int index)
{
    LevelScene *lvlScene = Object_Cast(self, Class_LevelScene);
    assert(0 <= index && index < 2);
    return lvlScene->m_cameras[index];
}

INLINE Player *LevelScene_GetPlayer(void *self)
{
    LevelScene *scene = Object_Cast(self, Class_LevelScene);
    return scene->m_player;
}

INLINE LevelManager *LevelScene_GetManager(void *self)
{
    LevelScene *scene = Object_Cast(self, Class_LevelScene);
    return scene->m_manager;
}

INLINE int LevelScene_GetLevelIndex(void *self)
{
    LevelScene *scene = Object_Cast(self, Class_LevelScene);
    return scene->m_levelIdx;
}

#endif
