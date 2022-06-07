#include "GameBody.h"
#include "../Scene/Scene.h"
#include "Player.h"
#include "../Utils/Renderer.h"

// Object virtual methods
void GameBody_VM_Destructor(void *self);

// GameObject virtual methods
void GameBody_VM_OnDisable(void *self);
void GameBody_VM_OnEnable(void *self);

// GameBody virtual methods
void GameBody_VM_DrawBody(void *self);

static GameBodyClass _Class_GameBody = { 0 };
const void *const Class_GameBody = &_Class_GameBody;

void Class_InitGameBody()
{
    if (!Class_IsInitialized(Class_GameBody))
    {
        Class_InitGameObject();

        void *self = (void *)Class_GameBody;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameObject,
            .name = "GameBody",
            .instanceSize = sizeof(GameBody),
            .classSize = sizeof(GameBodyClass)
        };
        Class_Constructor(params, GameBody_VM_Destructor);
        ((GameObjectClass *)self)->OnDisable = GameBody_VM_OnDisable;
        ((GameObjectClass *)self)->OnEnable = GameBody_VM_OnEnable;

        ((GameBodyClass *)self)->DrawBody = GameBody_VM_DrawBody;
    }
}

void GameBody_Constructor(void *self, void *scene, int layer)
{
    GameObject_Constructor(self, scene, layer);
    Object_SetClass(self, Class_GameBody);

    GameBody *gameBody = Object_Cast(self, Class_GameBody);

    SDL_Color debugColor = { .r = 255, .g = 255, .b = 255, .a = 255 };
    SDL_Color sleepColor = { .r =  50, .g = 100, .b = 255, .a = 255 };

    gameBody->m_body = NULL;
    gameBody->m_debugColor = debugColor;
    gameBody->m_sleepColor = sleepColor;
}

void GameBody_SetBody(void *self, PE_Body *body)
{
    assert(body);
    GameBody *gameBody = Object_Cast(self, Class_GameBody);

    gameBody->m_body = body;
    PE_Body_SetUserData(body, (void *)gameBody);
}

void GameBody_DeleteBody(void *self)
{
    GameBody *gameBody = Object_Cast(self, Class_GameBody);
    if (!gameBody->m_body)
    {
        return;
    }

    PE_Body *body = gameBody->m_body;
    PE_World *world = PE_Body_GetWorld(body);
    PE_World_RemoveBody(world, body);

    gameBody->m_body = NULL;
}

PE_Vec2 GameBody_GetPosition(const void *self)
{
    GameBody *gameBody = Object_Cast(self, Class_GameBody);
    if (gameBody->m_body)
    {
        Scene *scene = GameObject_GetScene((GameObject *)gameBody);
        return PE_Body_GetInterpolation(gameBody->m_body, Scene_GetAlpha(scene));
    }
    else
    {
        assert(false);
        return PE_Vec2_Zero;
    }
}

PE_Vec2 GameBody_GetVelocity(const void *self)
{
    GameBody *gameBody = Object_Cast(self, Class_GameBody);
    if (gameBody->m_body)
    {
        return PE_Body_GetVelocity(gameBody->m_body);
    }
    else
    {
        assert(false);
        return PE_Vec2_Zero;
    }
}

void GameBody_VM_DrawBody(void *self)
{
    GameBody *gameBody = Object_Cast(self, Class_GameBody);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);
    Camera *camera = Scene_GetActiveCamera(scene);
    float alpha = Scene_GetAlpha(scene);

    PE_Body *body = GameBody_GetBody(self);
    if (!body)
        return;

    if (PE_Body_IsAwake(body))
    {
        SDL_Color color = gameBody->m_debugColor;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }
    else
    {
        SDL_Color color = gameBody->m_sleepColor;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }
    Renderer_DrawBody(renderer, camera, body, alpha);
}

void GameBody_VM_Destructor(void *self)
{
    GameBody *gameBody = Object_Cast(self, Class_GameBody);

    if (gameBody->m_body)
    {
        Scene *scene = GameObject_GetScene(self);
        PE_World *world = Scene_GetWorld(scene);
        PE_World_RemoveBody(world, gameBody->m_body);
    }

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_GameBody);
}

void GameBody_VM_OnDisable(void *self)
{
    GameBody_DisableBody(self);
}

void GameBody_VM_OnEnable(void *self)
{
    GameBody_EnableBody(self);
}
