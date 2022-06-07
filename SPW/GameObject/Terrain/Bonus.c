#include "Bonus.h"
#include "../../Scene/LevelScene.h"
#include "../Collectable/Heart.h"

// Object virtual methods
void Bonus_VM_Destructor(void *self);

// GameObject virtual methods
void Bonus_VM_Render(void *self);
void Bonus_VM_Start(void *self);
void Bonus_VM_Update(void *self);
void Bouns_VM_OnRespawn(void *self);

static BonusClass _Class_Bonus = { 0 };
const void *const Class_Bonus = &_Class_Bonus;

void Class_InitBonus()
{
    if (!Class_IsInitialized(Class_Bonus))
    {
        Class_InitGameBody();

        void *self = (void *)Class_Bonus;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameBody,
            .name = "Bonus",
            .instanceSize = sizeof(Bonus),
            .classSize = sizeof(BonusClass)
        };
        Class_Constructor(params, Bonus_VM_Destructor);
        ((GameObjectClass *)self)->OnRespawn = Bouns_VM_OnRespawn;
        ((GameObjectClass *)self)->Render = Bonus_VM_Render;
        ((GameObjectClass *)self)->Start = Bonus_VM_Start;
        ((GameObjectClass *)self)->Update = Bonus_VM_Update;
    }
}

void Bonus_Constructor(void *self, void *scene, PE_Vec2 position)
{
    GameBody_Constructor(self, scene, LAYER_TERRAIN);
    Object_SetClass(self, Class_Bonus);

    Bonus *bonus = Object_Cast(self, Class_Bonus);
    bonus->m_hit = false;
    bonus->m_animator = NULL;

    GameBody_SaveStartPosition(bonus, position);
}

void Bonus_VM_Start(void *self)
{
}

void Bonus_VM_Destructor(void *self)
{
    Bonus *bonus = Object_Cast(self, Class_Bonus);

    RE_Animator_Delete(bonus->m_animator);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Bonus);
}

void Bouns_VM_OnRespawn(void *self)
{
}

void Bonus_VM_Render(void *self)
{
}

void Bonus_VM_Update(void *self)
{
}