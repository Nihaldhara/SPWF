#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "../Settings.h"
#include "../Common.h"
#include "../Utils/PVector.h"

typedef struct Scene_s Scene;

void Class_InitGameObject();

/// @brief Description de la classe "GameObject".
extern const void *const Class_GameObject;

typedef struct GameObjectClass_s {
    const Class base;

    // Messages
    void (*DrawGizmos) (void *self);
    void (*FixedUpdate)(void *self);
    void (*OnDisable)  (void *self);
    void (*OnEnable)   (void *self);
    void (*OnRespawn)  (void *self);
    void (*Render)     (void *self);
    void (*Start)      (void *self);
    void (*Update)     (void *self);
} GameObjectClass;

typedef enum CollisionFilter_e
{
    FILTER_PLAYER      = 1 << 0,
    FILTER_COLLECTABLE = 1 << 1,
    FILTER_ENEMY       = 1 << 2,
    FILTER_TERRAIN     = 1 << 3
} CollisionFilter;

typedef enum Layer_e
{
    LAYER_BACKGROUND,
    LAYER_TERRAIN_BACKGROUND,
    LAYER_TERRAIN,
    LAYER_TERRAIN_FOREGROUND,
    LAYER_COLLECTABLE,
    LAYER_ENEMY,
    LAYER_PLAYER,
    LAYER_FOREGROUND,
    LAYER_UI_BACKGROUND,
    LAYER_UI,
    LAYER_UI_FOREGROUND
} Layer;

typedef enum GameObjectFlag_e
{
    GAME_OBJECT_AUTO_RESPAWN = 1 << 0,
    GAME_OBJECT_VISIBLE      = 1 << 1,
} GameObjectFlag;

typedef enum GameObjectState_e
{
    GAME_OBJECT_RELAXED,
    GAME_OBJECT_TO_START,
    GAME_OBJECT_TO_ENABLE,
    GAME_OBJECT_TO_DISABLE,
    GAME_OBJECT_TO_DELETE
} GameObjectState;

typedef struct GameObject_s
{
    struct {
        Object base;
    } m_super;

    void *m_scene;
    bool m_enabled;
    int m_layer;
    int m_flags;
    int m_state;

    void *m_parent;
    PVector *m_children;

} GameObject;

void GameObject_Constructor(void *self, void *scene, int layer);

INLINE bool GameObject_IsEnabled(void *self)
{
    GameObject *object = Object_Cast(self, Class_GameObject);
    return object->m_enabled;
}

// Setter / Getter

INLINE void *GameObject_GetScene(const void *self)
{
    GameObject *object = Object_Cast(self, Class_GameObject);
    return object->m_scene;
}

INLINE int GameObject_GetLayer(const void *self)
{
    GameObject *object = Object_Cast(self, Class_GameObject);
    return object->m_layer;
}

INLINE void GameObject_SetLayer(void *self, int layer)
{
    GameObject *object = Object_Cast(self, Class_GameObject);
    object->m_layer = layer;
}

void GameObject_SetParent(void *self, void *parent);

INLINE void *GameObject_GetParent(void *self)
{
    GameObject *object = Object_Cast(self, Class_GameObject);
    return object->m_parent;
}

int GameObject_GetChildCount(void *self);
void **GameObject_GetChildren(void *self);
void **GameObject_CopyChildren(void *self);

// Méthodes virtuelles appelées par la scène
INLINE void GameObject_DrawGizmos(void *self)
{
    assert(Object_IsA(self, Class_GameObject));
    const GameObjectClass *clsDesc = Object_GetClass(self);
    (clsDesc->DrawGizmos)(self);
}

INLINE void GameObject_FixedUpdate(void *self)
{
    assert(Object_IsA(self, Class_GameObject));
    const GameObjectClass *clsDesc = Object_GetClass(self);
    (clsDesc->FixedUpdate)(self);
}

INLINE void GameObject_OnDisable(void *self)
{
    assert(Object_IsA(self, Class_GameObject));
    const GameObjectClass *clsDesc = Object_GetClass(self);
    (clsDesc->OnDisable)(self);
}

INLINE void GameObject_OnEnable(void *self)
{
    assert(Object_IsA(self, Class_GameObject));
    const GameObjectClass *clsDesc = Object_GetClass(self);
    (clsDesc->OnEnable)(self);
}

INLINE void GameObject_OnRespawn(void *self)
{
    assert(Object_IsA(self, Class_GameObject));
    const GameObjectClass *clsDesc = Object_GetClass(self);
    (clsDesc->OnRespawn)(self);
}

INLINE void GameObject_Render(void *self)
{
    assert(Object_IsA(self, Class_GameObject));
    const GameObjectClass *clsDesc = Object_GetClass(self);
    (clsDesc->Render)(self);
}

INLINE void GameObject_Start(void *self)
{
    assert(Object_IsA(self, Class_GameObject));
    const GameObjectClass *clsDesc = Object_GetClass(self);
    (clsDesc->Start)(self);
}

INLINE void GameObject_Update(void *self)
{
    assert(Object_IsA(self, Class_GameObject));
    const GameObjectClass *clsDesc = Object_GetClass(self);
    (clsDesc->Update)(self);
}

#endif
