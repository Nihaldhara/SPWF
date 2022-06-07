#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "UIObject.h"
#include "../../Utils/PVector.h"

void Class_InitCanvas();

/// @brief Description de la classe "Canvas".
extern const void *const Class_Canvas;

typedef struct CanvasClass_s {
    const UIObjectClass base;
} CanvasClass;

typedef struct Canvas_s
{
    struct {
        UIObject base;
    } m_super;

    PVector *m_objects;
} Canvas;

void Canvas_Constructor(void *self, void *scene);

INLINE int Canvas_GetUICount(void *self)
{
    Canvas *canvas = Object_Cast(self, Class_Canvas);
    return PVector_GetSize(canvas->m_objects);
}

#endif