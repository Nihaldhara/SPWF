#ifndef _RE_ALPHA_ANIM_H_
#define _RE_ALPHA_ANIM_H_

#include "RE_Settings.h"
#include "RE_Animation.h"

void Class_InitAlphaAnim();

/// @brief Description de la classe "AlphaAnim".
extern const void *const Class_AlphaAnim;

typedef struct AlphaAnimClass_s {
    const AnimationClass base;
} AlphaAnimClass;

typedef struct RE_AlphaAnim_s
{
    struct {
        RE_Animation base;
    } m_super;

    float m_alpha0;
    float m_alpha1;

} RE_AlphaAnim;

void RE_AlphaAnim_Constructor(
    void *self, RE_Animator *animator, char *name, float alpha0, float alpha1
);

#endif
