#include "Scene.h"
#include "../Utils/Renderer.h"

#include "../LevelParser.h"
#include "../GameObject/Background.h"

// Object virtual methods
void Scene_VM_Destructor(void *self);

// Scene virtual methods
void Scene_VM_Render(void *self);
bool Scene_VM_Update(void *self);

static SceneClass _Class_Scene = { 0 };
const void *const Class_Scene = &_Class_Scene;

void Class_InitScene()
{
    if (!Class_IsInitialized(Class_Scene))
    {
        Class_InitObject();

        void *self = (void *)Class_Scene;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Object,
            .name = "Scene",
            .instanceSize = sizeof(Scene),
            .classSize = sizeof(SceneClass)
        };
        Class_Constructor(params, Scene_VM_Destructor);
        ((SceneClass *)self)->Render = Scene_VM_Render;
        ((SceneClass *)self)->Update = Scene_VM_Update;
    }
}

void Scene_Constructor(void *self, SDL_Renderer *renderer, int theme)
{
    Object_Constructor(self);
    Object_SetClass(self, Class_Scene);

    Scene *scene = Object_Cast(self, Class_Scene);
    PE_Vec2 gravity = PE_Vec2_Set(0.0f, -40.0f);
    float timeStep = 1.0f / 60.0f;
    scene->m_accu = 0.0f;
    scene->m_timeStep = timeStep;
    scene->m_renderer = renderer;
    scene->m_quit = false;

    scene->m_stats.bodyCount = 0;
    scene->m_stats.disabledCount = 0;
    scene->m_stats.objectCount = 0;
    scene->m_stats.visibleCount = 0;

    scene->m_world = PE_World_New(gravity, timeStep);
    AssertNew(scene->m_world);

    scene->m_objManager = ObjectManager_New(scene->m_world);
    AssertNew(scene->m_objManager);

    scene->m_inputManager = InputManager_New();
    AssertNew(scene->m_inputManager);

    scene->m_assets = AssetManager_New(renderer, theme);
    AssertNew(scene->m_assets);
}

void Scene_UpdateGameObjects(Scene *scene)
{
    PE_World *world = scene->m_world;
    ListIterator it = { 0 };

    // Appelle la méthode FixedUpdate de chaque GameObject
    if (scene->m_mode == FIXED_UPDATE_REALTIME)
    {
        // Mode temps réel
        float timeStep = scene->m_timeStep;
        scene->m_accu += RE_Timer_GetDelta(g_time);
        while (scene->m_accu >= timeStep)
        {
            PE_World_FixedUpdate(world);
            ObjectManager_GetObjectIterator(scene->m_objManager, &it);
            while (ListIterator_MoveNext(&it))
            {
                GameObject *object = (GameObject *)ListIterator_Current(&it);
                if (GameObject_IsEnabled(object))
                {
                    GameObject_FixedUpdate(object);
                }
            }

            scene->m_accu -= timeStep;
        }
        scene->m_alpha = scene->m_accu / scene->m_timeStep;
    }
    else
    {
        // Mode pas à pas
        if (scene->m_makeStep)
        {
            PE_World_FixedUpdate(world);
            ObjectManager_GetObjectIterator(scene->m_objManager, &it);
            while (ListIterator_MoveNext(&it))
            {
                GameObject *object = (GameObject *)ListIterator_Current(&it);
                if (GameObject_IsEnabled(object))
                {
                    GameObject_FixedUpdate(object);
                }
            }
        }
        scene->m_makeStep = false;
        scene->m_alpha = 1.0f;
    }

    // Appelle la méthode Update de chaque GameObject
    int enabledCount = 0;
    ObjectManager_GetObjectIterator(scene->m_objManager, &it);
    while (ListIterator_MoveNext(&it))
    {
        GameObject *object = (GameObject *)ListIterator_Current(&it);
        if (GameObject_IsEnabled(object))
        {
            GameObject_Update(object);
            enabledCount++;
        }
    }

    scene->m_stats.objectCount = ObjectManager_GetObjectCount(scene->m_objManager);
    scene->m_stats.disabledCount = scene->m_stats.objectCount - enabledCount;
}

void *Scene_AllocateObject(void *self, const void *clsDesc)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    assert(Object_IsA(clsDesc, Class_GameObject));
    size_t size = Object_SizeOf(clsDesc);
    return ObjectManager_AllocateObject(scene->m_objManager, (int)size);
}

void Scene_DeleteObject(void *self, void *object)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    ObjectManager_DeleteObject(scene->m_objManager, object);
}

void Scene_EnableObject(void *self, void *object)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    ObjectManager_EnableObject(scene->m_objManager, object);
}

void Scene_DisableObject(void *self, void *object)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    ObjectManager_DisableObject(scene->m_objManager, object);
}

void Scene_SetVisible(void *self, void *object)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    ObjectManager_SetVisible(scene->m_objManager, object);
}

void Scene_SetToRespawn(void *self, void *object, bool toRespawn)
{
    GameObject *gameObject = Object_Cast(object, Class_GameObject);
    if (toRespawn)
    {
        gameObject->m_flags |= GAME_OBJECT_AUTO_RESPAWN;
    }
    else
    {
        gameObject->m_flags &= ~GAME_OBJECT_AUTO_RESPAWN;
    }
}

void Scene_VM_Destructor(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);

    ObjectManager_Delete(scene->m_objManager);
    InputManager_Delete(scene->m_inputManager);
    AssetManager_Delete(scene->m_assets);

    PE_World_Free(scene->m_world);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Scene);
}

void Scene_VM_Render(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);

    // Détermine les objets visibles par la caméra
    ObjectManager *objManager = scene->m_objManager;
    if (scene->m_activeCam)
    {
        PE_AABB view = Camera_GetWorldView(scene->m_activeCam);
        ObjectManager_AddVisibleBodies(objManager, view);
    }
    GameObject **visibleObjects = ObjectManager_GetVisibleObjects(objManager);
    int visibleCount = ObjectManager_GetVisibleCount(objManager);

    // Dessine les objets visibles par la caméra
    // Message : Render()
    for (int i = 0; i < visibleCount; ++i)
    {
        GameObject_Render(visibleObjects[i]);
    }

    // Dessine les corps présents dans le moteur physique
    if (scene->m_drawBodies)
    {
        for (int i = 0; i < visibleCount; ++i)
        {
            GameObject *object = visibleObjects[i];
            if (Object_IsA(object, Class_GameBody))
            {
                GameBody_DrawBody(object);
            }
        }
    }

    // Dessine les gizmos
    // Message : DrawGizmos()
    if (scene->m_drawGizmos)
    {
        for (int i = 0; i < visibleCount; ++i)
        {
            GameObject_DrawGizmos(visibleObjects[i]);
        }
    }
    scene->m_stats.visibleCount = visibleCount;

    // Vide le buffer des objets visibles
    ObjectManager_ClearVisibleObjects(objManager);
}

bool Scene_VM_Update(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    float timeStep = scene->m_timeStep;

    // Appelle les méthodes asynchrones
    // Messages : Start(), OnEnable(), OnDisable(), Delete()
    ObjectManager_ProcessObjects(scene->m_objManager);

    if (scene->m_respawn)
    {
        // Réinitialise la scene
        // Message : OnRespawn() et si nécessaire OnEnable()
        ObjectManager_RespawnObjects(scene->m_objManager);
        scene->m_respawn = false;
    }

    // Met à jour les entrées de l'utilisateur
    InputManager *inputManager = Scene_GetInputManager(self);
    InputManager_ProcessEvents(inputManager);

    // Met à jour les objets
    // Messages : Update(), FixedUpdate()
    Scene_UpdateGameObjects(scene);

    if (scene->m_quit)
    {
        return true;
    }

    scene->m_stats.bodyCount = PE_World_GetBodyCount(scene->m_world);

    return false;
}