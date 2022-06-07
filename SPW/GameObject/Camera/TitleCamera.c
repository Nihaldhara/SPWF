#include "TitleCamera.h"
#include "../../Scene/TitleScene.h"

// Object virtual methods
void TitleCamera_VM_Destructor(void *self);

// GameObject virtual methods
void TitleCamera_VM_Update(void *self);

static TitleCameraClass _Class_TitleCamera = { 0 };
const void *const Class_TitleCamera = &_Class_TitleCamera;

void Class_InitTitleCamera()
{
    if (!Class_IsInitialized(Class_TitleCamera))
    {
        Class_InitCamera();

        void *self = (void *)Class_TitleCamera;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Camera,
            .name = "TitleCamera",
            .instanceSize = sizeof(TitleCamera),
            .classSize = sizeof(TitleCameraClass)
        };
        Class_Constructor(params, TitleCamera_VM_Destructor);
        ((GameObjectClass *)self)->Update = TitleCamera_VM_Update;
    }
}

void TitleCamera_Constructor(void *self, void *scene, int width, int height)
{
    Camera_Constructor(self, scene, width, height);
    Object_SetClass(self, Class_TitleCamera);

    TitleCamera *ttlCam = Object_Cast(self, Class_TitleCamera);

    Camera_SetWorldView(ttlCam, PE_AABB_Set(0.0f, 0.0f, 24.0f, 13.5f));
}

void TitleCamera_VM_Destructor(void *self)
{
    TitleCamera *ttlCam = Object_Cast(self, Class_TitleCamera);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_TitleCamera);
}

void TitleCamera_VM_Update(void *self)
{
    TitleCamera *ttlCam = Object_Cast(self, Class_TitleCamera);
    Scene *scene = GameObject_GetScene(self);

    // Déplace la caméra vers la droite de 0.3 tuile par secondes
    float dx = 0.3f * RE_Timer_GetDelta(g_time);
    Camera_TranslateWorldView(ttlCam, PE_Vec2_Set(dx, 0.0f));
}
