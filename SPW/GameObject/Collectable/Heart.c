#include "Heart.h"
#include "../../Scene/LevelScene.h"

// Object virtual methods
void Heart_VM_Destructor(void *self);

// GameObject virtual methods
void Heart_VM_OnRespawn(void *self);
void Heart_VM_FixedUpdate(void *self);
void Heart_VM_Render(void *self);
void Heart_VM_Start(void *self);
void Heart_VM_Update(void *self);

// Collectable virtual methods
void Heart_VM_Collect(void *self, void *dst);

static HeartClass _Class_Heart = { 0 };
const void *const Class_Heart = &_Class_Heart;

void Class_InitHeart()
{
    if (!Class_IsInitialized(Class_Heart))
    {
        Class_InitCollectable();

        void *self = (void *)Class_Heart;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Collectable,
            .name = "Heart",
            .instanceSize = sizeof(Heart),
            .classSize = sizeof(HeartClass)
        };
        Class_Constructor(params, Heart_VM_Destructor);
        ((GameObjectClass *)self)->FixedUpdate = Heart_VM_FixedUpdate;
        ((GameObjectClass *)self)->OnRespawn = Heart_VM_OnRespawn;
        ((GameObjectClass *)self)->Render = Heart_VM_Render;
        ((GameObjectClass *)self)->Start = Heart_VM_Start;
        ((GameObjectClass *)self)->Update = Heart_VM_Update;
        ((CollectableClass *)self)->Collect = Heart_VM_Collect;
    }
}

void Heart_CreateAnimator(Heart *heart, void *scene)
{
    heart->m_animator = NULL;
}

void Heart_Constructor(void *self, void *scene, PE_Vec2 startPos)
{
    Collectable_Constructor(self, scene, startPos);
    Object_SetClass(self, Class_Heart);

    Heart *heart = Object_Cast(self, Class_Heart);

    Heart_CreateAnimator(heart, scene);
}

void Heart_VM_Start(void *self)
{
}

void Heart_VM_Destructor(void *self)
{
    Heart *heart = Object_Cast(self, Class_Heart);

    RE_Animator_Delete(heart->m_animator);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Heart);
}

void Heart_VM_FixedUpdate(void *self)
{
}

void Heart_VM_Update(void *self)
{
}

void Heart_VM_OnRespawn(void *self)
{
}

void Heart_VM_Render(void *self)
{
}

void Heart_VM_Collect(void *self, void *dst)
{
}