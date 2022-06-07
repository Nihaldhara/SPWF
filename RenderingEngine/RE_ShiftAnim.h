#ifndef _RE_SHIFT_ANIM_H_
#define _RE_SHIFT_ANIM_H_

#include "RE_Settings.h"
#include "RE_Animation.h"

void Class_InitShiftAnim();

/// @brief Description de la classe "ShiftAnim".
extern const void *const Class_ShiftAnim;

typedef struct ShiftAnimClass_s {
    const AnimationClass base;
} ShiftAnimClass;

typedef struct RE_ShiftAnim_s
{
    struct {
        RE_Animation base;
    } m_super;

    float m_scale;
    Vec2 m_shifts[2];

} RE_ShiftAnim;

void RE_ShiftAnim_Constructor(
    void *self, RE_Animator *animator, char *name, Vec2 shift0, Vec2 shift1
);

RE_INLINE float RE_ShiftAnim_GetScale(void *self)
{
    RE_ShiftAnim *anim = (RE_ShiftAnim *)Object_Cast(self, Class_ShiftAnim);
    return anim->m_scale;
}

RE_INLINE void RE_ShiftAnim_SetScale(void *self, float scale)
{
    RE_ShiftAnim *anim = (RE_ShiftAnim *)Object_Cast(self, Class_ShiftAnim);
    anim->m_scale = scale;
}

#endif
