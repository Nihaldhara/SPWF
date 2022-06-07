#ifndef _BONUS_H_
#define _BONUS_H_

#include "../GameBody.h"

void Class_InitBonus();

/// @brief Description de la classe "Bonus".
extern const void *const Class_Bonus;

typedef struct BonusClass_s {
    const GameBodyClass base;
} BonusClass;


typedef struct Bonus_s
{
    struct {
        GameBody base;
    } m_super;

    bool m_hit;
    RE_Animator *m_animator;
} Bonus;

void Bonus_Constructor(void *self, void *scene, PE_Vec2 position);

#endif
