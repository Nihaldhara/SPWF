#ifndef _LEVEL_CANVAS_H_
#define _LEVEL_CANVAS_H_

#include "../Canvas.h"

void Class_InitLevelCanvas();

/// @brief Description de la classe "LevelCanvas".
extern const void *const Class_LevelCanvas;

typedef struct LevelCanvasClass_s {
    const CanvasClass base;
} LevelCanvasClass;

typedef struct LevelCanvas_s
{
    struct {
        Canvas base;
    } m_super;

    UIObject *m_activeMenu;
    UIObject *m_scnStatsUI;
} LevelCanvas;

void LevelCanvas_Constructor(void *self, void *scene);

void LevelCanvas_OpenPauseMenu(void *self);
void LevelCanvas_OpenControls(void *self);
void LevelCanvas_CloseActiveMenu(void *self);
void LevelCanvas_ShowSceneStats(void *self, bool showInfo);

#endif
