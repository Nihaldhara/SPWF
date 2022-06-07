#include "Firefly.h"
#include "../../Scene/LevelScene.h"

// Object virtual methods
void Firefly_VM_Destructor(void *self);

// GameObject virtual methods
void Firefly_VM_OnRespawn(void *self);
void Firefly_VM_FixedUpdate(void *self);
void Firefly_VM_Render(void *self);
void Firefly_VM_Start(void *self);
void Firefly_VM_Update(void *self);

// Collectable virtual methods
void Firefly_VM_Collect(void *self, void *dst);

static FireflyClass _Class_Firefly = { 0 };
const void *const Class_Firefly = &_Class_Firefly;

void Class_InitFirefly()
{
    if (!Class_IsInitialized(Class_Firefly))
    {
        Class_InitCollectable();

        void *self = (void *)Class_Firefly;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Collectable,
            .name = "Firefly",
            .instanceSize = sizeof(Firefly),
            .classSize = sizeof(FireflyClass)
        };
        Class_Constructor(params, Firefly_VM_Destructor);
        ((GameObjectClass *)self)->FixedUpdate = Firefly_VM_FixedUpdate;
        ((GameObjectClass *)self)->OnRespawn = Firefly_VM_OnRespawn;
        ((GameObjectClass *)self)->Render = Firefly_VM_Render;
        ((GameObjectClass *)self)->Update = Firefly_VM_Update;
        ((GameObjectClass *)self)->Start = Firefly_VM_Start;
        ((CollectableClass *)self)->Collect = Firefly_VM_Collect;
    }
}

void Firefly_CreateAnimator(Firefly *firefly, void *scene)
{
    AssetManager *assets = Scene_GetAssetManager(scene);
    RE_Atlas *atlas = AssetManager_GetCollectableAtlas(assets);
    RE_AtlasPart *part = NULL;
    void *anim = NULL;

    // Crée l'animateur
    RE_Animator *animator = RE_Animator_New();
    AssertNew(animator);

    firefly->m_animator = animator;

    // Animation "Firefly"
    part = RE_Atlas_GetPart(atlas, "Firefly");
    AssertNew(part);

    anim = RE_Animator_CreateTextureAnim(animator, "Firefly", part);
    AssertNew(anim);
    RE_Animation_SetCycleCount(anim, -1);
    RE_Animation_SetCycleTime(anim, 0.3f);
}

void Firefly_Constructor(void *self, void *scene, PE_Vec2 startPos)
{
    startPos = PE_Vec2_Add(startPos, PE_Vec2_Set(0.5f, 0.5f));

    Collectable_Constructor(self, scene, startPos);
    Object_SetClass(self, Class_Firefly);

    Firefly *firefly = Object_Cast(self, Class_Firefly);

    SDL_Color color = { .r = 255, .g = 255, .b = 0, .a = 255 };
    GameBody_SetDebugColor(self, color);

    Firefly_CreateAnimator(firefly, scene);
}

void Firefly_VM_Start(void *self)
{
    Firefly *firefly = Object_Cast(self, Class_Firefly);
    Scene *scene = GameObject_GetScene(firefly);
    PE_World *world = Scene_GetWorld(scene);
    PE_Body *body = NULL;
    PE_BodyDef bodyDef = { 0 };
    PE_ColliderDef colliderDef = { 0 };
    PE_Collider *collider = NULL;

    // Crée le corps
    PE_BodyDef_SetDefault(&bodyDef);
    bodyDef.type = PE_STATIC_BODY;
    bodyDef.position = GameBody_GetStartPosition(firefly);
    bodyDef.name = "Firefly";

    body = PE_World_CreateBody(world, &bodyDef);
    AssertNew(body);

    // Crée le collider
    PE_ColliderDef_SetDefault(&colliderDef);
    colliderDef.isTrigger = true;
    colliderDef.filter.categoryBits = FILTER_COLLECTABLE;
    PE_Shape_SetAsCircle(&colliderDef.shape, PE_Vec2_Set(0.0f, 0.0f), 0.25f);

    collider = PE_Body_CreateCollider(body, &colliderDef);
    AssertNew(collider);

    GameBody_SetBody(self, body);

    // Joue l'animation par défaut
    RE_Animator_PlayAnimation(firefly->m_animator, "Firefly");
}

void Firefly_VM_Destructor(void *self)
{
    Firefly *firefly = Object_Cast(self, Class_Firefly);

    RE_Animator_Delete(firefly->m_animator);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Firefly);
}

void Firefly_VM_FixedUpdate(void *self)
{
    Firefly *firefly = Object_Cast(self, Class_Firefly);
}

void Firefly_VM_Update(void *self)
{
    Firefly *firefly = Object_Cast(self, Class_Firefly);
    RE_Animator_Update(firefly->m_animator, g_time);
}

void Firefly_VM_OnRespawn(void *self)
{
    Firefly *firefly = Object_Cast(self, Class_Firefly);
    Scene *scene = GameObject_GetScene(self);

    Scene_SetToRespawn(scene, self, false);

    RE_Animator_StopAnimations(firefly->m_animator);
    RE_Animator_PlayAnimation(firefly->m_animator, "Firefly");
}

void Firefly_VM_Render(void *self)
{
    Firefly *firefly = Object_Cast(self, Class_Firefly);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);
    Camera *camera = Scene_GetActiveCamera(scene);
    PE_Vec2 position = GameBody_GetPosition(self);

    SDL_FRect dst = { 0 };
    Camera_WorldToView(camera, position, &(dst.x), &(dst.y));
    float scale = Camera_GetWorldToViewScale(camera);
    dst.w = scale * 1.0f; // Le sprite fait 1 tuile de large
    dst.h = scale * 1.0f; // Le sprite fait 1 tuile de haut

    RE_Animator_RenderCopyF(
        firefly->m_animator, renderer, &dst, RE_ANCHOR_CENTER | RE_ANCHOR_MIDDLE
    );
}

void Firefly_VM_Collect(void *self, void *dst)
{
    Firefly *firefly = Object_Cast(self, Class_Firefly);
    Scene *scene = GameObject_GetScene(self);

    // Vérifie que le destinataire est bien le joueur
    // On ne souhaite pas qu'une noisette nous vole nos luciolles !
    if (Object_IsA(dst, Class_Player))
    {
        Player *player = Object_Cast(dst, Class_Player);

        // Ajoute une luciolle au joueur
        Player_AddFirefly(player);
    }

    // Indique qu'il faut regénérer l'objet si le joueur meurt
    Scene_SetToRespawn(scene, self, true);

    // Désactive la luciolle
    Scene_DisableObject(scene, firefly);
}