#ifndef _RE_BEZIER_ANIM_H_
#define _RE_BEZIER_ANIM_H_

#include "RE_Settings.h"
#include "RE_Animation.h"

void Class_InitBezierAnim();

/// @brief Description de la classe "BezierAnim".
extern const void *const Class_BezierAnim;

typedef struct BezierAnimClass_s {
    const AnimationClass base;
} BezierAnimClass;

typedef struct RE_BezierAnim_s
{
    struct {
        RE_Animation base;
    } m_super;

    Vec2 m_p[4];

} RE_BezierAnim;

void RE_BezierAnim_Constructor(
    void *self, RE_Animator *animator, char *name, Vec2 points[4]
);

RE_INLINE void RE_BezierAnim_SetPoints(void *self, Vec2 points[4])
{
    RE_BezierAnim *anim = (RE_BezierAnim *)Object_Cast(self, Class_BezierAnim);
    RE_Memcpy(anim->m_p, 4 * sizeof(Vec2), points, 4 * sizeof(Vec2));
}

#endif
