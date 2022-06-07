#ifndef _UI_OBJECT_H_
#define _UI_OBJECT_H_

#include "../GameObject.h"

void Class_InitUIObject();

/// @brief Description de la classe "UIObject".
extern const void *const Class_UIObject;

typedef struct UIObjectClass_s {
    const GameObjectClass base;
} UIObjectClass;

typedef struct UIRect_s
{
    Vec2 m_anchorMax; // Position relative
    Vec2 m_anchorMin; // Position relative
    Vec2 m_offsetMax; // Position absolue
    Vec2 m_offsetMin; // Position absolue
} UIRect;

typedef struct UIBorders_s
{
    int left, right, top, bottom;
} UIBorders;

typedef struct UIObject_s
{
    struct {
        GameObject base;
    } m_super;

    UIRect m_rect;
    SDL_Color m_gizmosColor;
    int m_gizmosAlpha;
} UIObject;

void UIObject_Constructor(void *self, void *scene);

INLINE void UIObject_SetLocalRect(void *self, UIRect rect)
{
    assert(rect.m_anchorMin.x <= rect.m_anchorMax.x);
    assert(rect.m_anchorMin.y <= rect.m_anchorMax.y);
    UIObject *ui = Object_Cast(self, Class_UIObject);
    ui->m_rect = rect;
}

INLINE UIRect UIObject_GetLocalRect(void *self)
{
    UIObject *ui = Object_Cast(self, Class_UIObject);
    return ui->m_rect;
}

SDL_FRect UIObject_GetCanvasRect(void *self);

INLINE void UIObject_SetGizmosColor(void *self, SDL_Color color)
{
    UIObject *ui = Object_Cast(self, Class_UIObject);
    ui->m_gizmosColor = color;
}

INLINE void UIObject_SetGizmosFillOpacity(void *self, int alpha)
{
    UIObject *ui = Object_Cast(self, Class_UIObject);
    ui->m_gizmosAlpha = alpha;
}

#endif
