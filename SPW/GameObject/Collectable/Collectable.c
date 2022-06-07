#include "Collectable.h"
#include "../../Scene/LevelScene.h"

// Object virtual methods
void Collectable_VM_Destructor(void *self);

// Méthodes virtuelles de la classe "Collectable"
void Collectable_VM_Collect(void *self, void *dst);

static CollectableClass _Class_Collectable = { 0 };
const void *const Class_Collectable = &_Class_Collectable;

void Class_InitCollectable()
{
    if (!Class_IsInitialized(Class_Collectable))
    {
        Class_InitGameBody();

        void *self = (void *)Class_Collectable;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameBody,
            .name = "Collectable",
            .instanceSize = sizeof(Collectable),
            .classSize = sizeof(CollectableClass)
        };
        Class_Constructor(params, Collectable_VM_Destructor);
        ((CollectableClass *)self)->Collect = Collectable_VM_Collect;
    }
}

void Collectable_Constructor(void *self, void *scene, PE_Vec2 startPos)
{
    GameBody_Constructor(self, scene, LAYER_COLLECTABLE);
    Object_SetClass(self, Class_Collectable);

    Collectable *collectable = Object_Cast(self, Class_Collectable);

    GameBody_SaveStartPosition(collectable, startPos);
}

void Collectable_VM_Destructor(void *self)
{
    Collectable *collectable = Object_Cast(self, Class_Collectable);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Collectable);
}

void Collectable_VM_Collect(void *self, void *dst)
{
    Collectable *collectable = Object_Cast(self, Class_Collectable);
    Scene *scene = GameObject_GetScene(collectable);
    Scene_DisableObject(scene, collectable);
}