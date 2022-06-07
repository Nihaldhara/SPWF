#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "../GameBody.h"

void Class_InitEnemy();

/// @brief Description de la classe "Enemy".
extern const void *const Class_Enemy;

typedef struct EnemyClass_s {
    const GameBodyClass base;

    void (*Damage)(void *self, void *damager);
} EnemyClass;

typedef struct Enemy_s
{
    struct {
        GameBody base;
    } m_super;

    int m_lifeCount;
} Enemy;

void Enemy_Constructor(void *self, void *scene, PE_Vec2 startPos, int lifeCount);

INLINE void Enemy_Damage(void *self, void *damager)
{
    assert(Object_IsA(self, Class_Enemy));
    const EnemyClass *clsDesc = Object_GetClass(self);
    (clsDesc->Damage)(self, damager);
}

INLINE int Enemy_GetLifeCount(void *self)
{
    Enemy *enemy = Object_Cast(self, Class_Enemy);
    return enemy->m_lifeCount;
}

INLINE void Enemy_SetLifeCount(void *self, int lifeCount)
{
    Enemy *enemy = Object_Cast(self, Class_Enemy);
    enemy->m_lifeCount = lifeCount;
}

#endif
