#include "Checkpoint.h"
#include "../../Scene/LevelScene.h"

// Object virtual methods
void Checkpoint_VM_Destructor(void *self);

// GameObject virtual methods
void Checkpoint_VM_Render(void *self);
void Checkpoint_VM_Start(void *self);
void Checkpoint_VM_Update(void *self);

// Collisions callbacks
void Checkpoint_OnCollisionEnter(PE_Collision *collision);

static CheckpointClass _Class_Checkpoint = { 0 };
const void *const Class_Checkpoint = &_Class_Checkpoint;

void Class_InitCheckpoint()
{
    if (!Class_IsInitialized(Class_Checkpoint))
    {
        Class_InitGameBody();

        void *self = (void *)Class_Checkpoint;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameBody,
            .name = "Checkpoint",
            .instanceSize = sizeof(Checkpoint),
            .classSize = sizeof(CheckpointClass)
        };
        Class_Constructor(params, Checkpoint_VM_Destructor);
        ((GameObjectClass *)self)->Render = Checkpoint_VM_Render;
        ((GameObjectClass *)self)->Start = Checkpoint_VM_Start;
        ((GameObjectClass *)self)->Update = Checkpoint_VM_Update;
    }
}

void Checkpoint_Constructor(void *self, void *scene, PE_Vec2 position)
{
    GameBody_Constructor(self, scene, LAYER_TERRAIN);
    Object_SetClass(self, Class_Checkpoint);

    Checkpoint *checkpoint = Object_Cast(self, Class_Checkpoint);
    checkpoint->m_isActive = false;
    checkpoint->m_animator = NULL;

    GameBody_SaveStartPosition(checkpoint, position);
}

void Checkpoint_VM_Start(void *self)
{
}

void Checkpoint_VM_Destructor(void *self)
{
    Checkpoint *checkpoint = Object_Cast(self, Class_Checkpoint);

    RE_Animator_Delete(checkpoint->m_animator);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Checkpoint);
}

void Checkpoint_OnCollisionEnter(PE_Collision *collision)
{
}

void Checkpoint_VM_Render(void *self)
{
}

void Checkpoint_VM_Update(void *self)
{
}