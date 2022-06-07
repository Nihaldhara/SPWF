#include "Brick.h"
#include "../../Scene/LevelScene.h"

// Object virtual methods
void Brick_VM_Destructor(void *self);

// GameObject virtual methods
void Brick_VM_Start(void *self);
void Brick_VM_Render(void *self);

static BrickClass _Class_Brick = { 0 };
const void *const Class_Brick = &_Class_Brick;

void Class_InitBrick()
{
    if (!Class_IsInitialized(Class_Brick))
    {
        Class_InitGameBody();

        void *self = (void *)Class_Brick;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameBody,
            .name = "Brick",
            .instanceSize = sizeof(Brick),
            .classSize = sizeof(BrickClass)
        };
        Class_Constructor(params, Brick_VM_Destructor);
        ((GameObjectClass *)self)->Start = Brick_VM_Start;
        ((GameObjectClass *)self)->Render = Brick_VM_Render;
    }
}

void Brick_Constructor(void *self, void *scene, PE_Vec2 startPos)
{
    GameBody_Constructor(self, scene, LAYER_TERRAIN);
    Object_SetClass(self, Class_Brick);

    Brick *brick = Object_Cast(self, Class_Brick);
    brick->m_animator = NULL;

    GameBody_SaveStartPosition(brick, startPos);
}

void Brick_VM_Start(void *self)
{
}

void Brick_VM_Destructor(void *self)
{
    Brick *brick = Object_Cast(self, Class_Brick);

    RE_Animator_Delete(brick->m_animator);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Brick);
}

void Brick_VM_Render(void *self)
{
}