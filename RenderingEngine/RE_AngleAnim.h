#ifndef _RE_ANGLE_ANIM_H_
#define _RE_ANGLE_ANIM_H_

#include "RE_Settings.h"
#include "RE_Animation.h"

void Class_InitAngleAnim();

/// @brief Description de la classe "AngleAnim".
extern const void *const Class_AngleAnim;

typedef struct AngleAnimClass_s {
    const AnimationClass base;
} AngleAnimClass;

typedef struct RE_AngleAnim_s
{
    struct {
        RE_Animation base;
    } m_super;

    float m_angle0;
    float m_angle1;

} RE_AngleAnim;

void RE_AngleAnim_Constructor(
    void *self, RE_Animator *animator, char *name, float angle0, float angle1
);

#endif
