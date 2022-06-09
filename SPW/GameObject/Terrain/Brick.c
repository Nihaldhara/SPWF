#include "Brick.h"
#include "../../Scene/LevelScene.h"

// Object virtual methods
void Brick_VM_Destructor(void *self);

// GameObject virtual methods
void Brick_VM_Render(void* self);
void Brick_VM_Start(void* self);
void Brick_VM_Update(void* self);

void Brick_OnCollisionEnter(PE_Collision* collision);

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
        ((GameObjectClass*)self)->Update = Brick_VM_Update;
    }
}

void Brick_CreateAnimator(Brick* brick, void* scene)
{
    RE_Animator* animator = RE_Animator_New();
    AssertNew(animator);

    AssetManager* assets = Scene_GetAssetManager(scene);
    RE_Atlas* atlas = AssetManager_GetTerrainAtlas(assets);
    RE_AtlasPart* part = NULL;
    void* anim = NULL;

    // Animation "Brick"
    part = RE_Atlas_GetPart(atlas, "Brick");
    AssertNew(part);

    anim = RE_Animator_CreateTextureAnim(animator, "Brick", part);
    AssertNew(anim);
    RE_Animation_SetCycleCount(anim, 0);

    brick->m_animator = animator;
}

void Brick_Constructor(void *self, void *scene, PE_Vec2 startPos)
{
    GameBody_Constructor(self, scene, LAYER_TERRAIN);
    Object_SetClass(self, Class_Brick);

    Brick *brick = Object_Cast(self, Class_Brick);
    brick->m_animator = NULL;

    GameBody_SaveStartPosition(brick, startPos);
    Brick_CreateAnimator(brick, scene);
}

void Brick_VM_Start(void *self)
{
    Brick* brick = Object_Cast(self, Class_Brick);
    Scene* scene = GameObject_GetScene(self);
    PE_World* world = Scene_GetWorld(scene);
    PE_Body* body = NULL;
    PE_BodyDef bodyDef = { 0 };
    PE_ColliderDef colliderDef = { 0 };
    PE_Collider* collider = NULL;

    // Crée le corps
    PE_BodyDef_SetDefault(&bodyDef);
    bodyDef.type = PE_STATIC_BODY;
    bodyDef.position = GameBody_GetStartPosition(brick);
    bodyDef.name = "Brick";

    body = PE_World_CreateBody(world, &bodyDef);
    AssertNew(body);

    PE_ColliderDef_SetDefault(&colliderDef);
    colliderDef.filter.categoryBits = FILTER_TERRAIN;
    colliderDef.isTrigger = false;
    PE_Shape_SetAsBox(&colliderDef.shape, -0.5f, 0.0f, 0.5f, 1.0f);

    collider = PE_Body_CreateCollider(body, &colliderDef);
    AssertNew(collider);

    PE_Collider_SetOnCollisionEnter(collider, Brick_OnCollisionEnter);

    GameBody_SetBody(self, body);

    // Joue l'animation par défaut
    RE_Animator_PlayAnimation(brick->m_animator, "Brick");
}

void Brick_VM_Destructor(void *self)
{
    Brick *brick = Object_Cast(self, Class_Brick);

    RE_Animator_Delete(brick->m_animator);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Brick);
}

void Brick_OnCollisionEnter(PE_Collision* collision)
{
    PE_Manifold manifold = PE_Collision_GetManifold(collision);
    PE_Body* thisBody = PE_Collision_GetBody(collision);
    PE_Body* otherBody = PE_Collision_GetOtherBody(collision);
    PE_Collider* otherCollider = PE_Collision_GetOtherCollider(collision);

    GameBody* thisGameBody = GameBody_GetFromBody(thisBody);
    GameBody* otherGameBody = GameBody_GetFromBody(otherBody);
    Brick* brick = (Brick*)GameBody_GetFromBody(thisBody);

    Scene* scene = GameObject_GetScene(brick);

    if (PE_Collider_CheckCategory(otherCollider, FILTER_PLAYER))
    {
        Player* player = Object_Cast(otherGameBody, Class_Player);
        float angle = PE_Vec2_AngleDeg(manifold.normal, PE_Vec2_Down);

        if (angle == 180.0f)
        {
            Scene_SetToRespawn(scene, brick, true);
            Scene_DisableObject(scene, brick);
        }
    }
}

void Brick_VM_Render(void *self)
{
    Brick* brick = Object_Cast(self, Class_Brick);
    Scene* scene = GameObject_GetScene(self);
    SDL_Renderer* renderer = Scene_GetRenderer(scene);
    Camera* camera = Scene_GetActiveCamera(scene);
    PE_Vec2 position = GameBody_GetPosition(brick);

    SDL_FRect dst = { 0 };
    Camera_WorldToView(camera, position, &(dst.x), &(dst.y));
    float scale = Camera_GetWorldToViewScale(camera);
    dst.w = 1.0f * scale;
    dst.h = 1.0f * scale;

    RE_Animator_RenderCopyF(
        brick->m_animator, renderer, &dst, RE_ANCHOR_CENTER | RE_ANCHOR_BOTTOM
    );
}

void Brick_VM_Update(void* self)
{
    Brick* brick = Object_Cast(self, Class_Brick);
    Scene* scene = GameObject_GetScene(brick);
    RE_Animator_Update(brick->m_animator, g_time);
}