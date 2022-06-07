#ifndef _TEX_ANIM_H_
#define _TEX_ANIM_H_

#include "RE_Settings.h"
#include "RE_Animation.h"
#include "RE_Atlas.h"

void Class_InitTexAnim();

/// @brief Description de la classe "TexAnim".
extern const void *const Class_TexAnim;

typedef struct TexAnimClass_s {
    const AnimationClass base;
} TexAnimClass;

typedef struct RE_TexAnim_s
{
    struct {
        RE_Animation base;
    } m_super;

    RE_AtlasPart *m_atlasPart;

} RE_TexAnim;

void RE_TexAnim_Constructor(
    void *self, RE_Animator *animator, char *name, RE_AtlasPart *atlasPart
);

#endif
