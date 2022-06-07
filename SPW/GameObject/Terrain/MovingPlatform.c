#include "MovingPlatform.h"
#include "../../Scene/LevelScene.h"
#include "../../Utils/Renderer.h"

// Object virtual methods
void MovingPlatform_VM_Destructor(void *self);

// GameObject virtual methods
void MovingPlatform_VM_DrawGizmos(void *self);
void MovingPlatform_VM_FixedUpdate(void *self);
void MovingPlatform_VM_OnRespawn(void *self);
void MovingPlatform_VM_Render(void *self);
void MovingPlatform_VM_Start(void *self);

// Collisions callbacks
void MovingPlatform_OnColisionEnter(PE_Collision *collision);
void MovingPlatform_OnColisionExit(PE_Collision *collision);

static MovingPlatformClass _Class_MovingPlatform = { 0 };
const void *const Class_MovingPlatform = &_Class_MovingPlatform;

void Class_InitMovingPlatform()
{
    if (!Class_IsInitialized(Class_MovingPlatform))
    {
        Class_InitGameBody();

        void *self = (void *)Class_MovingPlatform;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameBody,
            .name = "MovingPlatform",
            .instanceSize = sizeof(MovingPlatform),
            .classSize = sizeof(MovingPlatformClass)
        };
        Class_Constructor(params, MovingPlatform_VM_Destructor);
        ((GameObjectClass *)self)->DrawGizmos = MovingPlatform_VM_DrawGizmos;
        ((GameObjectClass *)self)->OnRespawn = MovingPlatform_VM_OnRespawn;
        ((GameObjectClass *)self)->FixedUpdate = MovingPlatform_VM_FixedUpdate;
        ((GameObjectClass *)self)->Render = MovingPlatform_VM_Render;
        ((GameObjectClass *)self)->Start = MovingPlatform_VM_Start;
    }
}

void MovingPlatform_Constructor(
    void *self, void *scene, PE_Vec2 position, PE_ColliderDef *colliderDef)
{
    GameBody_Constructor(self, scene, LAYER_TERRAIN_FOREGROUND);
    Object_SetClass(self, Class_MovingPlatform);

    MovingPlatform *platform = Object_Cast(self, Class_MovingPlatform);

    platform->m_index = 0;
    platform->m_speed = 1.5f;
    platform->m_capacity = 0;
    platform->m_pointCount = 0;
    platform->m_points = NULL;
    platform->m_colliderDef = *colliderDef;

    Scene_SetToRespawn(scene, platform, true);
    GameBody_SaveStartPosition(platform, position);
    

    AssetManager *assets = Scene_GetAssetManager(scene);
    RE_Atlas *atlas = AssetManager_GetTerrainAtlas(assets);
    platform->m_platformPart = RE_Atlas_GetPart(atlas, "MovingPlatform");
    AssertNew(platform->m_platformPart);
}

void MovingPlatform_VM_Start(void *self)
{
    MovingPlatform *platform = Object_Cast(self, Class_MovingPlatform);
    Scene *scene = GameObject_GetScene(platform);
    PE_World *world = Scene_GetWorld(scene);
    PE_Collider *collider = NULL;

    // Crée le corps
    PE_BodyDef bodyDef = { 0 };
    PE_BodyDef_SetDefault(&bodyDef);
    bodyDef.type = PE_KINEMATIC_BODY;
    bodyDef.position = GameBody_GetStartPosition(platform);
    bodyDef.name = "MovingPlatform";

    PE_Body *body = PE_World_CreateBody(world, &bodyDef);
    AssertNew(body);

    // Modification de la définition du collider
    PE_ColliderDef colliderDef = platform->m_colliderDef;
    colliderDef.filter.categoryBits = FILTER_TERRAIN;

    collider = PE_Body_CreateCollider(body, &colliderDef);
    AssertNew(collider);

    PE_Collider_SetOnCollisionEnter(collider, MovingPlatform_OnColisionEnter);
    PE_Collider_SetOnCollisionExit(collider, MovingPlatform_OnColisionExit);

    GameBody_SetBody(self, body);
}


void MovingPlatform_VM_OnRespawn(void *self)
{
    MovingPlatform *platform = Object_Cast(self, Class_MovingPlatform);
    PE_Body *body = GameBody_GetBody(platform);
    PE_Body_SetPosition(body, platform->m_points[0]);
    platform->m_index = 0;
}

INLINE int MovingPlatform_GetNextIndex(MovingPlatform *platform)
{
    if (platform->m_forward)
    {
        return (platform->m_index + 1) % platform->m_pointCount;
    }
    return (platform->m_index - 1 + platform->m_pointCount) % platform->m_pointCount;
}

INLINE PE_Vec2 MovingPlatform_GetTarget(MovingPlatform *platform)
{
    return platform->m_points[platform->m_index];
}

INLINE PE_Vec2 MovingPlatform_GetNextTarget(MovingPlatform *platform)
{
    int nextIndex = MovingPlatform_GetNextIndex(platform);
    return platform->m_points[nextIndex];
}

void MovingPlatform_OnColisionEnter(PE_Collision *collision)
{
    PE_Body *thisBody = PE_Collision_GetBody(collision);
    PE_Body *otherBody = PE_Collision_GetOtherBody(collision);

    PE_Body_SetParent(otherBody, thisBody);
    PE_Body_ApplyImpulse(otherBody, PE_Vec2_Neg(PE_Body_GetVelocity(thisBody)));
}

void MovingPlatform_OnColisionExit(PE_Collision *collision)
{
    PE_Body *thisBody = PE_Collision_GetBody(collision);
    PE_Body *otherBody = PE_Collision_GetOtherBody(collision);

    if (PE_Body_GetParent(otherBody) == thisBody)
        PE_Body_SetParent(otherBody, NULL);
    PE_Body_ApplyImpulse(otherBody, PE_Body_GetVelocity(thisBody));
}

void MovingPlatform_SetPointCount(void *self, int pointCount)
{
    assert(pointCount >= 0);
    MovingPlatform *platform = Object_Cast(self, Class_MovingPlatform);

    int oldPointCount = platform->m_pointCount;
    int oldCapacity = platform->m_capacity;

    if (pointCount < oldCapacity)
    {
        if (platform->m_index >= pointCount)
        {
            platform->m_index = pointCount - 1;
        }
        platform->m_pointCount = pointCount;
        return;
    }

    // Crée le nouveau tableau de points
    PE_Vec2 *newPoints = (PE_Vec2 *)calloc(pointCount, sizeof(PE_Vec2));
    AssertNew(newPoints);

    // Copie puis destruction de l'ancien tableau
    if (platform->m_points)
    {
        PE_Memcpy(
            newPoints, pointCount * sizeof(PE_Vec2),
            platform->m_points, oldPointCount * sizeof(PE_Vec2)
        );

        free(platform->m_points);
    }

    platform->m_points = newPoints;
    platform->m_pointCount = pointCount;
    platform->m_capacity = pointCount;
}

void MovingPlatform_VM_Destructor(void *self)
{
    MovingPlatform *platform = Object_Cast(self, Class_MovingPlatform);

    if (platform->m_points)
    {
        free(platform->m_points);
    }

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_MovingPlatform);
}

void MovingPlatform_VM_DrawGizmos(void *self)
{
    MovingPlatform *platform = Object_Cast(self, Class_MovingPlatform);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = scene->m_renderer;
    Camera *camera = Scene_GetActiveCamera(scene);

    // Chemin
    for (int i = 0; i < platform->m_pointCount; i++)
    {
        PE_Vec2 pA = platform->m_points[i];
        PE_Vec2 pB = platform->m_points[(i + 1) % platform->m_pointCount];
        SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
        Renderer_DrawVector(renderer, camera, pA, PE_Vec2_Sub(pB, pA));
    }
}

void MovingPlatform_VM_FixedUpdate(void *self)
{
    MovingPlatform *platform = Object_Cast(self, Class_MovingPlatform);
    Scene *scene = GameObject_GetScene(self);
    PE_Body *body = GameBody_GetBody(self);
    PE_Vec2 position = PE_Body_GetPosition(body);
    PE_Vec2 target = MovingPlatform_GetTarget(platform);
    PE_Vec2 nextTarget = MovingPlatform_GetNextTarget(platform);
    PE_Vec2 displacement = PE_Vec2_Sub(target, position);
    float distance = PE_Vec2_Length(displacement);
    float timeStep = Scene_GetFixedTimeStep(scene);
    float speed = platform->m_speed;

    PE_Vec2 velocity;
    if (timeStep * speed < distance)
    {
        velocity = PE_Vec2_Normalize(displacement);
        velocity = PE_Vec2_Scale(velocity, speed);
    }
    else
    {
        PE_Vec2 velocityA = PE_Vec2_Scale(displacement, 1.0f / timeStep);
        PE_Vec2 velocityB = PE_Vec2_Scale(
            PE_Vec2_Normalize(PE_Vec2_Sub(nextTarget, target)),
            speed - distance / timeStep);

        velocity = PE_Vec2_Add(velocityA, velocityB);

        platform->m_index = MovingPlatform_GetNextIndex(platform);
    }
    PE_Body_SetVelocity(body, velocity);
}

void MovingPlatform_VM_Render(void *self)
{
    MovingPlatform *platform = Object_Cast(self, Class_MovingPlatform);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);
    Camera *camera = Scene_GetActiveCamera(scene);
    PE_Vec2 position = GameBody_GetPosition(platform);

    float scale = Camera_GetWorldToViewScale(camera);
    SDL_FRect rect = { 0 };
    rect.h = 0.5f * scale;
    rect.w = 4.0f * scale;
    Camera_WorldToView(camera, position, &rect.x, &rect.y);
    RE_AtlasPart_RenderCopyF(
        platform->m_platformPart, 0, renderer,
        &rect, RE_ANCHOR_CENTER | RE_ANCHOR_MIDDLE
    );
}