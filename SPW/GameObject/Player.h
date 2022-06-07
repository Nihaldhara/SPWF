#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameBody.h"

void Class_InitPlayer();

/// @brief Description de la classe "Player".
extern const void *const Class_Player;

typedef struct PlayerClass_s {
    const GameBodyClass base;
} PlayerClass;

#define MAX_HEART_COUNT 2

typedef enum PlayerState_e
{
    PLAYER_IDLE,
    PLAYER_FALLING,
    PLAYER_WALKING,
    PLAYER_RUNNING,
    PLAYER_SKIDDING,
    PLAYER_DYING
} PlayerState;

typedef struct Player_s
{
    struct {
        GameBody base;
    } m_super;

    RE_Animator *m_animator;

    int m_state;

    float m_hDirection;
    bool m_jump;
    bool m_bounce;
    bool m_facingRight;

    int m_lifeCount;
    int m_heartCount;
    int m_fireflyCount;

} Player;

#define PLAYER_DAMAGE_ANGLE 55.0f

void Player_Constructor(void *self, void *scene);

void Player_Damage(Player *player);
void Player_Kill(Player *player);

INLINE int Player_GetLifeCount(const void *self)
{
    Player *player = Object_Cast(self, Class_Player);
    return player->m_lifeCount;
}

INLINE int Player_GetFireflyCount(const void *self)
{
    Player *player = Object_Cast(self, Class_Player);
    return player->m_fireflyCount;
}

INLINE int Player_GetHeartCount(const void *self)
{
    Player *player = Object_Cast(self, Class_Player);
    return player->m_heartCount;
}

INLINE void Player_Bounce(void *self)
{
    Player *player = Object_Cast(self, Class_Player);
    player->m_bounce = true;
}

void Player_AddFirefly(void *self);
void Player_AddHeart(void *self);


#endif
