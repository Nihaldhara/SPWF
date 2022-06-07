#ifndef _GAME_BODY_H_
#define _GAME_BODY_H_

#include "GameObject.h"

void Class_InitGameBody();

/// @brief Description de la classe "GameBody".
extern const void *const Class_GameBody;

typedef struct GameBodyClass_s {
    const GameObjectClass base;

    void (*DrawBody)(void *self);
} GameBodyClass;

typedef struct GameBody_s
{
    struct {
        GameObject base;
    } m_super;

    SDL_Color m_debugColor;
    SDL_Color m_sleepColor;

    PE_Body *m_body;
    PE_Vec2 m_startPos;
} GameBody;

void GameBody_Constructor(void *self, void *scene, int layer);

void GameBody_SetBody(void *self, PE_Body *body);

INLINE PE_Body *GameBody_GetBody(void *self)
{
    GameBody *gameBody = Object_Cast(self, Class_GameBody);
    return gameBody->m_body;
}

void GameBody_DeleteBody(void *self);

INLINE void GameBody_EnableBody(void *self)
{
    GameBody *gameBody = Object_Cast(self, Class_GameBody);
    if (gameBody->m_body)
    {
        PE_Body_SetEnabled(gameBody->m_body, true);
    }
}

INLINE void GameBody_DisableBody(void *self)
{
    GameBody *gameBody = Object_Cast(self, Class_GameBody);
    if (gameBody->m_body)
    {
        PE_Body_SetEnabled(gameBody->m_body, false);
    }
}

INLINE GameBody *GameBody_GetFromBody(PE_Body *body)
{
    void *data = PE_Body_GetUserData(body);
    assert(data);
    return Object_Cast(data, Class_GameBody);
}

INLINE void GameBody_SaveStartPosition(void *self, PE_Vec2 startPos)
{
    GameBody *gameBody = Object_Cast(self, Class_GameBody);
    gameBody->m_startPos = startPos;
}

INLINE PE_Vec2 GameBody_GetStartPosition(void *self)
{
    GameBody *gameBody = Object_Cast(self, Class_GameBody);
    return gameBody->m_startPos;
}

PE_Vec2 GameBody_GetPosition(const void *self);
PE_Vec2 GameBody_GetVelocity(const void *self);



INLINE void GameBody_DrawBody(void *self)
{
    assert(Object_IsA(self, Class_GameBody));
    const GameBodyClass *clsDesc = Object_GetClass(self);
    (clsDesc->DrawBody)(self);
}

INLINE void GameBody_SetDebugColor(void *self, SDL_Color color)
{
    GameBody *gameBody = Object_Cast(self, Class_GameBody);
    gameBody->m_debugColor = color;
}

#endif