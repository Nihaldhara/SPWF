#ifndef _LEVEL_MANAGER_H_
#define _LEVEL_MANAGER_H_

#include "../GameObject.h"

void Class_InitLevelManager();

/// @brief Description de la classe "LevelManager".
extern const void *const Class_LevelManager;

typedef struct LevelManagerClass_s {
    const GameObjectClass base;
} LevelManagerClass;

typedef struct LevelManager_s
{
    struct {
        GameObject base;
    } m_super;

    bool m_paused;
    bool m_showSceneStatsUI;
    int m_camIndex;
    float m_stepDelay;

} LevelManager;

void LevelManager_Constructor(void *self, void *scene);

INLINE bool LevelManager_IsGamePaused(void *self)
{
    LevelManager *manager = Object_Cast(self, Class_LevelManager);
    return manager->m_paused;
}

void LevelManager_SetPause(void *self, bool pauseStatus);

#endif
