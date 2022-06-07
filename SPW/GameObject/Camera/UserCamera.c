#include "UserCamera.h"
#include "../../Scene/LevelScene.h"

// Object virtual methods
void UserCamera_VM_Destructor(void *self);

// GameObject virtual methods
void UserCamera_VM_OnRespawn(void *self);
void UserCamera_VM_Update(void *self);

static UserCameraClass _Class_UserCamera = { 0 };
const void *const Class_UserCamera = &_Class_UserCamera;

void Class_InitUserCamera()
{
    if (!Class_IsInitialized(Class_UserCamera))
    {
        Class_InitCamera();

        void *self = (void *)Class_UserCamera;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Camera,
            .name = "UserCamera",
            .instanceSize = sizeof(UserCamera),
            .classSize = sizeof(UserCameraClass)
        };
        Class_Constructor(params, UserCamera_VM_Destructor);
        ((GameObjectClass *)self)->Update = UserCamera_VM_Update;
        ((GameObjectClass *)self)->OnRespawn = UserCamera_VM_OnRespawn;
    }
}

void UserCamera_Constructor(
    void *self, void *scene,
    int width, int height, void *trackedCam)
{
    Camera_Constructor(self, scene, width, height);
    Object_SetClass(self, Class_UserCamera);

    UserCamera *userCam = Object_Cast(self, Class_UserCamera);

    userCam->m_trackedCam = trackedCam;
    userCam->m_track = true;

    PE_AABB worldView = Camera_GetWorldView(trackedCam);
    worldView = PE_AABB_Scale(worldView, PE_AABB_GetCenter(worldView), 2.0f);
    Camera_SetWorldView(userCam, worldView);
}

void UserCamera_VM_Destructor(void *self)
{
    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_UserCamera);
}

void UserCamera_VM_OnRespawn(void *self)
{
    UserCamera *userCam = Object_Cast(self, Class_UserCamera);
}

void UserCamera_VM_Update(void *self)
{
    UserCamera *userCam = Object_Cast(self, Class_UserCamera);
    Scene *scene = GameObject_GetScene(self);

    if (Scene_GetActiveCamera(scene) != self)
    {
        // La caméra n'est pas active
        return;
    }

    InputManager *inputManager = Scene_GetInputManager(scene);
    MouseInput *mouse = InputManager_GetMouse(inputManager);
    DebugInput *debug = InputManager_GetDebug(inputManager);

    PE_Vec2 mousePos = PE_Vec2_Zero;
    PE_Vec2 mouseDelta = PE_Vec2_Zero;
    PE_AABB worldView = Camera_GetWorldView(self);
    PE_AABB trackedView = Camera_GetWorldView(userCam->m_trackedCam);

    Camera_ViewToWorld(self, mouse->viewPos.x, mouse->viewPos.y, &mousePos);
    Camera_ViewToWorld(self, mouse->lastViewPos.x, mouse->lastViewPos.y, &mouseDelta);
    mouseDelta = PE_Vec2_Sub(mouseDelta, mousePos);

    if ((mouse->wheel > 0) && (worldView.upper.x - worldView.lower.x > 1.0f))
    {
        worldView = PE_AABB_Scale(worldView, mousePos, 0.8f);
    }
    else if ((mouse->wheel < 0))
    {
        worldView = PE_AABB_Scale(worldView, mousePos, 1.0f / 0.8f);
    }

    if (mouse->rightDown)
    {
        PE_AABB_Translate(&worldView, mouseDelta);
    }

    if (debug->trackPressed)
    {
        userCam->m_track = !(userCam->m_track);
        userCam->m_trackedLower = trackedView.lower;
    }

    if (userCam->m_track)
    {
        PE_AABB_Translate(
            &worldView,
            PE_Vec2_Sub(trackedView.lower, userCam->m_trackedLower)
        );
        userCam->m_trackedLower = trackedView.lower;
    }

    if (mouse->leftClickCount > 1)
    {
        Player *player = LevelScene_GetPlayer(scene);
        PE_Body *body = GameBody_GetBody((GameBody *)player);
        PE_Body_SetPosition(body, mousePos);
        PE_Body_SetVelocity(body, PE_Vec2_Zero);
    }

    if (mouse->middleClick)
    {
        worldView = PE_AABB_Scale(trackedView, PE_AABB_GetCenter(trackedView), 2.0f);
    }

    Camera_SetWorldView(self, worldView);
}