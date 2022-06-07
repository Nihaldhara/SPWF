#ifndef _RE_SCALE_ANIM_H_
#define _RE_SCALE_ANIM_H_

#include "RE_Settings.h"
#include "RE_Animation.h"

void Class_InitScaleAnim();

/// @brief Description de la classe "ScaleAnim".
extern const void *const Class_ScaleAnim;

typedef struct ScaleAnimClass_s {
    const AnimationClass base;
} ScaleAnimClass;

typedef struct RE_ScaleAnim_s
{
    struct {
        RE_Animation base;
    } m_super;

    Vec2 m_scale0;
    Vec2 m_scale1;

} RE_ScaleAnim;

void RE_ScaleAnim_Constructor(
    void *self, RE_Animator *animator, char *name, Vec2 scale0, Vec2 scale1
);

#endif
