#include "LevelEnd.h"
#include "../../Scene/LevelScene.h"

// Object virtual methods
void LevelEnd_VM_Destructor(void *self);

// GameObject virtual methods
void LevelEnd_VM_Render(void *self);
void LevelEnd_VM_Start(void *self);
void LevelEnd_VM_Update(void *self);

// Collisions callbacks
void LevelEnd_OnCollisionEnter(PE_Collision *collision);

static LevelEndClass _Class_LevelEnd = { 0 };
const void *const Class_LevelEnd = &_Class_LevelEnd;

void Class_InitLevelEnd()
{
    if (!Class_IsInitialized(Class_LevelEnd))
    {
        Class_InitGameBody();

        void *self = (void *)Class_LevelEnd;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameBody,
            .name = "LevelEnd",
            .instanceSize = sizeof(LevelEnd),
            .classSize = sizeof(LevelEndClass)
        };
        Class_Constructor(params, LevelEnd_VM_Destructor);
        ((GameObjectClass *)self)->Render = LevelEnd_VM_Render;
        ((GameObjectClass *)self)->Start = LevelEnd_VM_Start;
        ((GameObjectClass *)self)->Update = LevelEnd_VM_Update;
    }
}


void LevelEnd_CreateAnimator(LevelEnd *lvlEnd, void *scene)
{
    RE_Animator *animator = RE_Animator_New();
    AssertNew(animator);

    AssetManager *assets = Scene_GetAssetManager(scene);
    RE_Atlas *atlas = AssetManager_GetTerrainAtlas(assets);
    RE_AtlasPart *part = NULL;
    void *anim = NULL;

    // Animation "End"
    part = RE_Atlas_GetPart(atlas, "LevelEnd");
    AssertNew(part);

    anim = RE_Animator_CreateTextureAnim(animator, "End", part);
    AssertNew(anim);
    RE_Animation_SetCycleCount(anim, 0);

    lvlEnd->m_animator = animator;
}

void LevelEnd_Constructor(void *self, void *scene, PE_Vec2 position)
{
    GameBody_Constructor(self, scene, LAYER_TERRAIN);
    Object_SetClass(self, Class_LevelEnd);

    LevelEnd *lvlEnd = Object_Cast(self, Class_LevelEnd);
    lvlEnd->m_isActive = false;

    GameBody_SaveStartPosition(
        lvlEnd,
        PE_Vec2_Add(position, PE_Vec2_Set(0.5f, 0.0f))
    );

    SDL_Color color = { .r = 245, .g = 80, .b = 30, .a = 255 };
    GameBody_SetDebugColor(self, color);

    LevelEnd_CreateAnimator(lvlEnd, scene);
}

void LevelEnd_VM_Start(void *self)
{
    LevelEnd *lvlEnd = Object_Cast(self, Class_LevelEnd);
    Scene *scene = GameObject_GetScene(self);
    PE_World *world = Scene_GetWorld(scene);
    PE_Body *body = NULL;
    PE_BodyDef bodyDef = { 0 };
    PE_ColliderDef colliderDef = { 0 };
    PE_Collider *collider = NULL;

    // Crée le corps
    PE_BodyDef_SetDefault(&bodyDef);
    bodyDef.type = PE_STATIC_BODY;
    bodyDef.position = GameBody_GetStartPosition(lvlEnd);
    bodyDef.name = "LevelEnd";

    body = PE_World_CreateBody(world, &bodyDef);
    AssertNew(body);

    PE_ColliderDef_SetDefault(&colliderDef);
    colliderDef.filter.categoryBits = FILTER_TERRAIN;
    colliderDef.isTrigger = true;
    PE_Shape_SetAsBox(&colliderDef.shape, -0.4f, 0.0f, 0.4f, 1.9f);

    collider = PE_Body_CreateCollider(body, &colliderDef);
    AssertNew(collider);

    PE_Collider_SetOnCollisionEnter(collider, LevelEnd_OnCollisionEnter);

    GameBody_SetBody(self, body);

    // Joue l'animation par défaut
    RE_Animator_PlayAnimation(lvlEnd->m_animator, "End");
}

void LevelEnd_VM_Destructor(void *self)
{
    LevelEnd *lvlEnd = Object_Cast(self, Class_LevelEnd);

    RE_Animator_Delete(lvlEnd->m_animator);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_LevelEnd);
}

void LevelEnd_OnCollisionEnter(PE_Collision *collision)
{
    PE_Body *thisBody = PE_Collision_GetBody(collision);
    PE_Collider *otherCollider = PE_Collision_GetOtherCollider(collision);
    LevelEnd *lvlEnd = (LevelEnd *)GameBody_GetFromBody(thisBody);

    if (PE_Collider_CheckCategory(otherCollider, FILTER_PLAYER))
    {
        LevelScene *scene = Object_Cast(GameObject_GetScene(lvlEnd), Class_LevelScene);
        int i = LevelScene_GetLevelIndex(scene);

        g_progress.levels[i].sucessful = true;

        Scene_Quit(scene);
    }
}

void LevelEnd_VM_Render(void *self)
{
    LevelEnd *lvlEnd = Object_Cast(self, Class_LevelEnd);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);
    Camera *camera = Scene_GetActiveCamera(scene);
    PE_Vec2 position = GameBody_GetPosition(lvlEnd);

    SDL_FRect dst = { 0 };
    Camera_WorldToView(camera, position, &(dst.x), &(dst.y));
    float scale = Camera_GetWorldToViewScale(camera);
    dst.w = 1.0f * scale;
    dst.h = 2.0f * scale;

    RE_Animator_RenderCopyF(
        lvlEnd->m_animator, renderer, &dst, RE_ANCHOR_CENTER | RE_ANCHOR_BOTTOM
    );
}

void LevelEnd_VM_Update(void *self)
{
    LevelEnd *lvlEnd = Object_Cast(self, Class_LevelEnd);
    RE_Animator_Update(lvlEnd->m_animator, g_time);
}