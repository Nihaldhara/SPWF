#ifndef _NUT_H_
#define _NUT_H_

#include "Enemy.h"

void Class_InitNut();

/// @brief Description de la classe "Nut".
extern const void *const Class_Nut;

typedef struct NutClass_s {
    const EnemyClass base;
} NutClass;

typedef enum NutState_e
{
    NUT_IDLE,
    NUT_SPINNING,
    //NUT_DYING
} NutState;

typedef struct Nut_s
{
    struct {
        Enemy base;
    } m_super;

    RE_Animator *m_animator;

    int m_state;

} Nut;

void Nut_Constructor(void *self, void *scene, PE_Vec2 startPos);

#endif
