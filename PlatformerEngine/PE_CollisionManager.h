
#ifndef _PE_COLLISION_MANAGER_H_
#define _PE_COLLISION_MANAGER_H_

#include "PE_Allocator.h"
#include "PE_List.h"
#include "PE_Vector.h"
#include "PE_CollisionContainer.h"
#include "PE_Collider.h"

#include <stdlib.h>

typedef struct PE_World_s PE_World;
typedef struct PE_BroadPhase_s PE_BroadPhase;
typedef struct PE_CollisionPair_s PE_CollisionPair;
typedef struct PE_Collision_s PE_Collision;

typedef struct PE_CollisionManager_s
{
    /// @protected
    /// @brief Pointer to the world allocator.
    PE_Allocator *m_allocator;

    /// @protected
    /// @brief Pointer to the parent world.
    PE_World *m_world;

    /// @protected
    /// @brief The broadPhase of the collision detection algorithm.
    PE_BroadPhase *m_broadPhase;

    PE_CollisionContainer *m_container;
} PE_CollisionManager;

PE_CollisionManager *PE_CollisionManager_New(PE_World *world);
void PE_CollisionManager_Free(PE_CollisionManager *manager);

//.................................................................................................
// Collider methods

void PE_CollisionManager_CreateProxy(
    PE_CollisionManager *manager, PE_AABB *aabb, PE_ColliderProxy *proxy);

/// @brief Supprime un proxy pour un collider avec le gestionnaire de collision.
/// @param manager 
/// @param proxy 
void PE_CollisionManager_RemoveProxy(PE_CollisionManager *manager, PE_ColliderProxy *proxy);

void PE_CollisionManager_SynchronizeProxy(
    PE_CollisionManager *manager, PE_ColliderProxy *proxy, PE_Vec2 displacement);

//.................................................................................................
// Fixed update methods

void PE_CollisionManager_FindNewCollisions(PE_CollisionManager *manager);
void PE_CollisionManager_Collide(PE_CollisionManager *manager);

//.................................................................................................
// Getter

PE_INLINE PE_CollisionContainer *PE_CollisionManager_GetContainer(PE_CollisionManager *manager)
{
    return manager->m_container;
}

PE_INLINE PE_BroadPhase *PE_CollisionManager_GetBroadPhase(PE_CollisionManager *manager)
{
    return manager->m_broadPhase;
}

#endif
