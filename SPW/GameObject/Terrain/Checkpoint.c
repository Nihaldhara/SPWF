#include "Checkpoint.h"
#include "../../Scene/LevelScene.h"

// Object virtual methods
void Checkpoint_VM_Destructor(void* self);

// GameObject virtual methods
void Checkpoint_VM_Render(void* self);
void Checkpoint_VM_Start(void* self);
void Checkpoint_VM_Update(void* self);

// Collisions callbacks
void Checkpoint_OnCollisionEnter(PE_Collision* collision);

static CheckpointClass _Class_Checkpoint = { 0 };
const void* const Class_Checkpoint = &_Class_Checkpoint;

void Class_InitCheckpoint()
{
    if (!Class_IsInitialized(Class_Checkpoint))
    {
        Class_InitGameBody();

        void* self = (void*)Class_Checkpoint;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameBody,
            .name = "Checkpoint",
            .instanceSize = sizeof(Checkpoint),
            .classSize = sizeof(CheckpointClass)
        };
        Class_Constructor(params, Checkpoint_VM_Destructor);
        ((GameObjectClass*)self)->Render = Checkpoint_VM_Render;
        ((GameObjectClass*)self)->Start = Checkpoint_VM_Start;
        ((GameObjectClass*)self)->Update = Checkpoint_VM_Update;
    }
}
void Checkpoint_CreateAnimator(Checkpoint* checkpoint, void* scene)
{
    RE_Animator* animator = RE_Animator_New();
    AssertNew(animator);

    AssetManager* assets = Scene_GetAssetManager(scene);
    RE_Atlas* atlas = AssetManager_GetTerrainAtlas(assets);
    RE_AtlasPart* part = NULL;
    void* anim = NULL;

    // Animation "CheckpointEmpty"
    part = RE_Atlas_GetPart(atlas, "CheckpointEmpty");
    AssertNew(part);

    anim = RE_Animator_CreateTextureAnim(animator, "CheckpointEmpty", part);
    AssertNew(anim);
    RE_Animation_SetCycleCount(anim, 0);

    // Animation "CheckpointFull"
    part = RE_Atlas_GetPart(atlas, "CheckpointFull");
    AssertNew(part);

    anim = RE_Animator_CreateTextureAnim(animator, "CheckpointFull", part);
    AssertNew(anim);
    RE_Animation_SetCycleCount(anim, 0);

    checkpoint->m_animator = animator;
}

void Checkpoint_Constructor(void* self, void* scene, PE_Vec2 position)
{
    GameBody_Constructor(self, scene, LAYER_TERRAIN);
    Object_SetClass(self, Class_Checkpoint);

    Checkpoint* checkpoint = Object_Cast(self, Class_Checkpoint);
    checkpoint->m_isActive = false;

    GameBody_SaveStartPosition(checkpoint, position);

    Checkpoint_CreateAnimator(checkpoint, scene);
}

void Checkpoint_VM_Start(void* self)
{
    Checkpoint* checkpoint = Object_Cast(self, Class_Checkpoint);
    Scene* scene = GameObject_GetScene(self);
    PE_World* world = Scene_GetWorld(scene);
    PE_Body* body = NULL;
    PE_BodyDef bodyDef = { 0 };
    PE_ColliderDef colliderDef = { 0 };
    PE_Collider* collider = NULL;

    // Cree le corps
    PE_BodyDef_SetDefault(&bodyDef);
    bodyDef.type = PE_STATIC_BODY;
    bodyDef.position = GameBody_GetStartPosition(checkpoint);
    bodyDef.name = "Checkpoint";

    body = PE_World_CreateBody(world, &bodyDef);
    AssertNew(body);

    PE_ColliderDef_SetDefault(&colliderDef);
    colliderDef.filter.categoryBits = FILTER_TERRAIN;
    colliderDef.isTrigger = true;
    PE_Shape_SetAsBox(&colliderDef.shape, -0.9f, 0.0f, 0.8f, 1.7f);

    collider = PE_Body_CreateCollider(body, &colliderDef);
    AssertNew(collider);

    PE_Collider_SetOnCollisionEnter(collider, Checkpoint_OnCollisionEnter);
    GameBody_SetBody(self, body);

    // Joue l'animation par defaut
    RE_Animator_PlayAnimation(checkpoint->m_animator, "CheckpointEmpty");
}

void Checkpoint_VM_Destructor(void* self)
{
    Checkpoint* checkpoint = Object_Cast(self, Class_Checkpoint);

    RE_Animator_Delete(checkpoint->m_animator);

    // Destructeur de la classe mere
    Object_SuperDestroy(self, Class_Checkpoint);
}

void Checkpoint_OnCollisionEnter(PE_Collision* collision)
{
    PE_Body* thisBody = PE_Collision_GetBody(collision);
    PE_Collider* otherCollider = PE_Collision_GetOtherCollider(collision);
    Checkpoint* checkpoint = (Checkpoint*)GameBody_GetFromBody(thisBody);

    if (PE_Collider_CheckCategory(otherCollider, FILTER_PLAYER) && checkpoint->m_isActive == false)
    {
        LevelScene* scene = Object_Cast(GameObject_GetScene(checkpoint), Class_LevelScene);
        RE_Animator_PlayAnimation(checkpoint->m_animator, "CheckpointFull");
        checkpoint->m_isActive = true;
        LevelScene_SetStartPosition(scene, thisBody->m_position);
    }
}

void Checkpoint_VM_Render(void* self)
{
    Checkpoint* checkpoint = Object_Cast(self, Class_Checkpoint);
    Scene* scene = GameObject_GetScene(self);
    SDL_Renderer* renderer = Scene_GetRenderer(scene);
    Camera* camera = Scene_GetActiveCamera(scene);
    PE_Vec2 position = GameBody_GetPosition(checkpoint);

    SDL_FRect dst = { 0 };
    Camera_WorldToView(camera, position, &(dst.x), &(dst.y));
    float scale = Camera_GetWorldToViewScale(camera);
    dst.w = 2.0f * scale;
    dst.h = 2.0f * scale;

    RE_Animator_RenderCopyF(
        checkpoint->m_animator, renderer, &dst, RE_ANCHOR_CENTER | RE_ANCHOR_BOTTOM
    );
}

void Checkpoint_VM_Update(void* self)
{
    Checkpoint* checkpoint = Object_Cast(self, Class_Checkpoint);
    RE_Animator_Update(checkpoint->m_animator, g_time);
}