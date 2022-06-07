#ifndef _RECT_TRANSFORM_H_
#define _RECT_TRANSFORM_H_

#include "RE_Utils.h"

typedef struct Point_s
{
    float x, y;
} Point;

/*typedef struct Rect_s
{
    float x, y;
    float w, h;
} Rect;*/

typedef struct UITransform_s UITransform;
struct UITransform_s
{
    UITransform *m_parent;
    Vec2 m_anchorMin;
    Vec2 m_anchorMax;
    Vec2 m_offsetMin;
    Vec2 m_offsetMax;
    Vec2 m_pivot;
    float m_angle;
    float m_width, m_height;
};

// GetWorldRect
// GetLocalRect

#endif 
