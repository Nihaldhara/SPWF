#ifndef _TITLE_CANVAS_H_
#define _TITLE_CANVAS_H_

#include "../Canvas.h"

void Class_InitTitleCanvas();

/// @brief Description de la classe "TitleCanvas".
extern const void *const Class_TitleCanvas;

typedef struct TitleCanvasClass_s {
    const CanvasClass base;
} TitleCanvasClass;

typedef struct TitleCanvas_s
{
    struct {
        Canvas base;
    } m_super;

    UIObject *m_activeMenu;
} TitleCanvas;

void TitleCanvas_Constructor(void *self, void *scene);
void TitleCanvas_OpenLevelSelection(void *self);

#endif
