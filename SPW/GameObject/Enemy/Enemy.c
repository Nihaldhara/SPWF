#include "Enemy.h"
#include "../../Scene/Scene.h"


// Object virtual methods
void Enemy_VM_Destructor(void *self);

// Enemy virtual methods
void Enemy_VM_Damage(void *self, void *damager);

static EnemyClass _Class_Enemy = { 0 };
const void *const Class_Enemy = &_Class_Enemy;

void Class_InitEnemy()
{
    if (!Class_IsInitialized(Class_Enemy))
    {
        Class_InitGameBody();

        void *self = (void *)Class_Enemy;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameBody,
            .name = "Enemy",
            .instanceSize = sizeof(Enemy),
            .classSize = sizeof(EnemyClass)
        };
        Class_Constructor(params, Enemy_VM_Destructor);
        ((EnemyClass *)self)->Damage = Enemy_VM_Damage;
    }
}

void Enemy_Constructor(void *self, void *scene, PE_Vec2 startPos, int lifeCount)
{
    GameBody_Constructor(self, scene, LAYER_ENEMY);
    Object_SetClass(self, Class_Enemy);

    Enemy *enemy = Object_Cast(self, Class_Enemy);
    enemy->m_lifeCount = lifeCount;

    GameBody_SaveStartPosition(enemy, startPos);
}

void Enemy_VM_Destructor(void *self)
{
    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Enemy);
}

void Enemy_VM_Damage(void *self, void *damager)
{
    Enemy *enemy = Object_Cast(self, Class_Enemy);

    Scene *scene = GameObject_GetScene(self);
    Scene_DisableObject(scene, self);
}