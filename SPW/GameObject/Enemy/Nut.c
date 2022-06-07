#include "Nut.h"
#include "../../Scene/LevelScene.h"
#include "../../Utils/Renderer.h"


// Object virtual methods
void Nut_VM_Destructor(void *self);

// GameObject virtual methods
void Nut_VM_DrawGizmos(void *self);
void Nut_VM_FixedUpdate(void *self);
void Nut_VM_OnRespawn(void *self);
void Nut_VM_Render(void *self);
void Nut_VM_Start(void *self);
void Nut_VM_Update(void *self);

// Enemy virtual methods
void Nut_VM_Damage(void *self, void *damager);


// Callbacks de collisions
void Nut_OnCollisionStay(PE_Collision *collision);

static NutClass _Class_Nut = { 0 };
const void *const Class_Nut = &_Class_Nut;

void Class_InitNut()
{
    if (!Class_IsInitialized(Class_Nut))
    {
        Class_InitEnemy();

        void *self = (void *)Class_Nut;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Enemy,
            .name = "Nut",
            .instanceSize = sizeof(Nut),
            .classSize = sizeof(NutClass)
        };
        Class_Constructor(params, Nut_VM_Destructor);
        ((GameObjectClass *)self)->DrawGizmos = Nut_VM_DrawGizmos;
        ((GameObjectClass *)self)->FixedUpdate = Nut_VM_FixedUpdate;
        ((GameObjectClass *)self)->OnRespawn = Nut_VM_OnRespawn;
        ((GameObjectClass *)self)->Render = Nut_VM_Render;
        ((GameObjectClass *)self)->Start = Nut_VM_Start;
        ((GameObjectClass *)self)->Update = Nut_VM_Update;
        ((EnemyClass *)self)->Damage = Nut_VM_Damage;
    }
}
void Nut_CreateAnimator(Nut *nut, void *scene)
{
    AssetManager *assets = Scene_GetAssetManager(scene);
    RE_Atlas *atlas = AssetManager_GetEnemyAtlas(assets);
    void *anim = NULL;

    // Crée l'animateur
    RE_Animator *animator = RE_Animator_New();
    AssertNew(animator);

    nut->m_animator = animator;

    // Animation "Idle"
    RE_AtlasPart *part = RE_Atlas_GetPart(atlas, "NutIdle");
    AssertNew(part);

    anim = RE_Animator_CreateTextureAnim(animator, "Idle", part);
    AssertNew(anim);
    RE_Animation_SetCycleCount(anim, 0);
}

void Nut_Constructor(void *self, void *scene, PE_Vec2 startPos)
{
    Enemy_Constructor(self, scene, startPos, 1);
    Object_SetClass(self, Class_Nut);

    Nut *nut = Object_Cast(self, Class_Nut);
    nut->m_state = NUT_IDLE;

    Nut_CreateAnimator(nut, scene);
    Scene_SetToRespawn(scene, self, true);
}

void Nut_VM_Start(void *self)
{
    Nut *nut = Object_Cast(self, Class_Nut);
    Scene *scene = GameObject_GetScene(nut);
    PE_World *world = Scene_GetWorld(scene);
    PE_Body *body = NULL;
    PE_BodyDef bodyDef = { 0 };
    PE_ColliderDef colliderDef = { 0 };
    PE_Collider *collider = NULL;

    // Crée le corps
    PE_BodyDef_SetDefault(&bodyDef);
    bodyDef.type = PE_DYNAMIC_BODY;
    bodyDef.position = GameBody_GetStartPosition(nut);
    bodyDef.name = "Nut";
    bodyDef.xDamping = 0.0f;
    bodyDef.yDamping = 0.0f;
    bodyDef.mass = 1.0f;

    body = PE_World_CreateBody(world, &bodyDef);
    AssertNew(body);

    PE_ColliderDef_SetDefault(&colliderDef);
    colliderDef.friction = 0.005f;
    colliderDef.filter.categoryBits = FILTER_ENEMY;
    colliderDef.filter.maskBits = FILTER_TERRAIN | FILTER_PLAYER;
    PE_Shape_SetAsCircle(
        &colliderDef.shape,
        PE_Vec2_Set(0.0f, 0.45f), 0.45f
    );

    collider = PE_Body_CreateCollider(body, &colliderDef);
    AssertNew(collider);

    PE_Collider_SetOnCollisionStay(collider, Nut_OnCollisionStay);

    GameBody_SetBody(self, body);

    // Endort le corps
    // Permet d'optimiser le calcul de la physique,
    // seuls les corps proches du joueur sont simulés
    PE_Body_SetAwake(body, false);

    // Joue l'animation par défaut
    RE_Animator_PlayAnimation(nut->m_animator, "Idle");
}

void Nut_VM_Damage(void *self, void *damager)
{
    Nut *nut = Object_Cast(self, Class_Nut);
    Scene *scene = GameObject_GetScene(nut);

    if (Object_IsA(damager, Class_Player))
    {
        Player_Bounce(damager);
    }

    Scene_DisableObject(scene, nut);
}

void Nut_VM_Destructor(void *self)
{
    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Nut);
}

void Nut_OnCollisionStay(PE_Collision *collision)
{
    PE_Manifold manifold = PE_Collision_GetManifold(collision);
    PE_Body *thisBody = PE_Collision_GetBody(collision);
    PE_Body *otherBody = PE_Collision_GetOtherBody(collision);
    PE_Collider *otherCollider = PE_Collision_GetOtherCollider(collision);

    GameBody *thisGameBody = GameBody_GetFromBody(thisBody);
    GameBody *otherGameBody = GameBody_GetFromBody(otherBody);
    Nut *nut = Object_Cast(thisGameBody, Class_Nut);

    // Collision avec le joueur
    if (PE_Collider_CheckCategory(otherCollider, FILTER_PLAYER))
    {
        Player *player = Object_Cast(otherGameBody, Class_Player);

        float angle = PE_Vec2_AngleDeg(manifold.normal, PE_Vec2_Down);
        if (angle > 55.0f)
        {
            Player_Damage(player);
        }
        return;
    }
}

void Nut_VM_DrawGizmos(void *self)
{
    Nut *nut = Object_Cast(self, Class_Nut);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);
    Camera *camera = Scene_GetActiveCamera(scene);

    PE_Vec2 position = GameBody_GetPosition(nut);
    PE_Vec2 velocity = GameBody_GetVelocity(nut);

    // Dessine en blanc le vecteur vitesse du joueur
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    Renderer_DrawVector(renderer, camera, position, velocity);
}

void Nut_VM_FixedUpdate(void *self)
{
    Nut *nut = Object_Cast(self, Class_Nut);
    PE_Body *body = GameBody_GetBody(self);
    PE_Vec2 position = PE_Body_GetPosition(body);
    PE_Vec2 velocity = PE_Body_GetLocalVelocity(body);

    if (PE_Body_IsAwake(body) == false)
    {
        // Ne met pas à jour la noisette si elle est endormie
        // Le joueur est loin d'elle et elle n'est plus visible par la caméra
        return;
    }

    // Tue la noisette si elle tombe dans un trou
    if (position.y < -2.0f)
    {
        Scene *scene = GameObject_GetScene(self);
        Scene_DisableObject(scene, self);
        return;
    }

    Scene *scene = GameObject_GetScene(self);
    Player *player = LevelScene_GetPlayer(scene);
    PE_Vec2 playerPos = GameBody_GetPosition(player);

    // Calcule la distance entre le joueur et la noisette
    float dist = PE_Vec2_Distance(position, playerPos);

    if (dist > 24.0f)
    {
        // La distance entre de joueur et la noisette vient de dépasser 24 tuiles.
        // On endort la noisette pour ne plus la simuler dans le moteur physique.
        PE_Body_SetAwake(body, false);
        return;
    }
    else if (dist <= 5.0f && nut->m_state == NUT_IDLE)
    {
        // Le joueur est à moins de 5 tuiles de la noisette
        nut->m_state = NUT_SPINNING;
        velocity = PE_Vec2_Set(-3.f, 10.f);
    }

    PE_Body_SetVelocity(body, velocity);
}

void Nut_VM_OnRespawn(void *self)
{
    Nut *nut = Object_Cast(self, Class_Nut);
    nut->m_state = NUT_IDLE;

    GameBody_EnableBody(self);

    PE_Vec2 startPos = GameBody_GetStartPosition(self);
    PE_Body *body = GameBody_GetBody(self);
    PE_Body_SetPosition(body, startPos);
    PE_Body_SetVelocity(body, PE_Vec2_Zero);
    PE_Body_ClearForces(body);

    RE_Animator_StopAnimations(nut->m_animator);
    RE_Animator_PlayAnimation(nut->m_animator, "Idle");
}

void Nut_VM_Render(void *self)
{
    Nut *nut = Object_Cast(self, Class_Nut);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);
    Camera *camera = Scene_GetActiveCamera(scene);

    PE_Body *body = GameBody_GetBody(self);
    PE_Vec2 position = GameBody_GetPosition(self);

    float scale = Camera_GetWorldToViewScale(camera);
    SDL_FRect rect = { 0 };
    rect.h = 1.0f * scale; // Le sprite fait 1 tuile de haut
    rect.w = 1.0f * scale; // Le sprite fait 1 tuile de large
    Camera_WorldToView(camera, position, &rect.x, &rect.y);

    RE_Animator_RenderCopyF(
        nut->m_animator, renderer, &rect, RE_ANCHOR_CENTER | RE_ANCHOR_BOTTOM
    );
}

void Nut_VM_Update(void *self)
{
    Nut *nut = Object_Cast(self, Class_Nut);

    RE_Animator_Update(nut->m_animator, g_time);
}