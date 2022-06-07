#include "Camera.h"
#include "../../Scene/Scene.h"

// Object virtual methods
void Camera_VM_Destructor(void *self);

static CameraClass _Class_Camera = { 0 };
const void *const Class_Camera = &_Class_Camera;

void Class_InitCamera()
{
    if (!Class_IsInitialized(Class_Camera))
    {
        Class_InitGameObject();

        void *self = (void *)Class_Camera;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameObject,
            .name = "Camera",
            .instanceSize = sizeof(Camera),
            .classSize = sizeof(CameraClass)
        };
        Class_Constructor(params, Camera_VM_Destructor);
    }
}

void Camera_Constructor(void *self, void *scene, int width, int height)
{
    GameObject_Constructor(self, scene, LAYER_FOREGROUND);
    Object_SetClass(self, Class_Camera);

    Camera *camera = Object_Cast(self, Class_Camera);

    // Dimensions de la vue dans le référentiel monde
    // La hauteur est calculée pour un ratio 16/9
    float worldW = 24.0f;
    float worldH = 9.0f / 16.0f * worldW;

    Scene_SetToRespawn(scene, self, true);

    camera->m_width = width;
    camera->m_height = height;
    camera->m_worldView = PE_AABB_Set(0.0f, 0.0f, worldW, worldH);
    camera->m_worldBounds = PE_AABB_Set(0.0f, 0.0f, 100000.0f, worldH);
}

void Camera_VM_Destructor(void *self)
{
    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Camera);
}

void Camera_WorldToView(void *self, PE_Vec2 position, float *x, float *y)
{
    Camera *camera = Object_Cast(self, Class_Camera);
    float w = camera->m_worldView.upper.x - camera->m_worldView.lower.x;
    float h = camera->m_worldView.upper.y - camera->m_worldView.lower.y;
    float xScale = (float)camera->m_width  / w;
    float yScale = (float)camera->m_height / h;
    *x = (position.x - camera->m_worldView.lower.x) * xScale;
    *y = (position.y - camera->m_worldView.lower.y) * yScale;
    *y = (float)camera->m_height - *y;
}

void Camera_ViewToWorld(void *self, float x, float y, PE_Vec2 *position)
{
    Camera *camera = Object_Cast(self, Class_Camera);
    y = camera->m_height - y;
    float ratioX = (float)x / (float)camera->m_width;
    float ratioY = (float)y / (float)camera->m_height;
    float w = camera->m_worldView.upper.x - camera->m_worldView.lower.x;
    float h = camera->m_worldView.upper.y - camera->m_worldView.lower.y;
    position->x = camera->m_worldView.lower.x + ratioX * w;
    position->y = camera->m_worldView.lower.y + ratioY * h;
}

void Camera_TranslateWorldView(void *self, PE_Vec2 displacement)
{
    Camera *camera = Object_Cast(self, Class_Camera);
    PE_AABB_Translate(&camera->m_worldView, displacement);
}

int Camera_GetWidth(void *self)
{
    Camera *camera = Object_Cast(self, Class_Camera);
    return camera->m_width;
}

int Camera_GetHeight(void *self)
{
    Camera *camera = Object_Cast(self, Class_Camera);
    return camera->m_height;
}

float Camera_GetWorldToViewScale(void *self)
{
    Camera *camera = Object_Cast(self, Class_Camera);
    float w = camera->m_worldView.upper.x - camera->m_worldView.lower.x;
    return (float)camera->m_width  / w;
}
