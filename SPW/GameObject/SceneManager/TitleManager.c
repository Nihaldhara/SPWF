#include "TitleManager.h"
#include "../../Scene/TitleScene.h"

// Object virtual methods
void TitleManager_VM_Destructor(void *self);

// GameObject virtual methods
void TitleManager_VM_Update(void *self);

static TitleManagerClass _Class_TitleManager = { 0 };
const void *const Class_TitleManager = &_Class_TitleManager;

void Class_InitTitleManager()
{
    if (!Class_IsInitialized(Class_TitleManager))
    {
        Class_InitGameObject();

        void *self = (void *)Class_TitleManager;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameObject,
            .name = "TitleManager",
            .instanceSize = sizeof(TitleManager),
            .classSize = sizeof(TitleManagerClass)
        };
        Class_Constructor(params, TitleManager_VM_Destructor);
        ((GameObjectClass *)self)->Update = TitleManager_VM_Update;
    }
}

void TitleManager_Constructor(void *self, void *scene)
{
    GameObject_Constructor(self, scene, LAYER_FOREGROUND);
    Object_SetClass(self, Class_TitleManager);

    TitleManager *manager = Object_Cast(self, Class_TitleManager);
}

void TitleManager_OpenLevelSelection(void *self)
{
}

void TitleManager_VM_Destructor(void *self)
{
    TitleManager *manager = Object_Cast(self, Class_TitleManager);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_TitleManager);
}

void TitleManager_VM_Update(void *self)
{
    TitleManager *lvlManager = Object_Cast(self, Class_TitleManager);
    Scene *scene = GameObject_GetScene(lvlManager);
    InputManager *inputManager = Scene_GetInputManager(scene);
    DebugInput *debug = InputManager_GetDebug(inputManager);
    AppInput *appInput = InputManager_GetApplication(inputManager);

    if (appInput->quitPressed)
    {
        Scene_Quit(scene);
    }

    if (debug->gizmosPressed)
    {
        bool drawn = Scene_AreGizmosDrawn(scene);
        Scene_SetDrawGizmos(scene, !drawn);
    }

    if (debug->bodyPressed)
    {
        bool drawn = Scene_AreBodiesDrawn(scene);
        Scene_SetDrawBodies(scene, !drawn);
    }
}