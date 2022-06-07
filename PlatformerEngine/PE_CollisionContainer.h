
#ifndef _PE_COLLISION_CONTAINER_H_
#define _PE_COLLISION_CONTAINER_H_

#include "PE_List.h"
#include "PE_Allocator.h"

typedef struct PE_World_s PE_World;
typedef struct PE_CollisionPair_s PE_CollisionPair;
typedef struct PE_Collision_s PE_Collision;
typedef struct PE_Collider_s PE_Collider;

typedef struct PE_CollisionContainer_s
{
    /// @protected
    /// @brief Pointer to the world allocator.
    PE_Allocator *m_allocator;

    /// @protected
    /// @brief Pointer to the parent world.
    PE_World *m_world;

    /// @protected
    /// @brief The list of every current collision pairs.
    PE_List m_list;

} PE_CollisionContainer;

PE_CollisionContainer *PE_CollisionContainer_New(PE_Allocator *allocator);
void PE_CollisionContainer_Free(PE_CollisionContainer *container);

PE_CollisionPair *PE_CollisionContainer_CreateCPair(
    PE_CollisionContainer *container, PE_Collider *colliderA, PE_Collider *colliderB);

void PE_CollisionContainer_RemoveCPair(PE_CollisionContainer *container, PE_CollisionPair *cPair);

PE_INLINE int PE_CollisionContainer_GetCollisionCount(PE_CollisionContainer *container)
{
    return PE_List_GetNodeCount(&container->m_list);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PE_CPairIterator

typedef PE_ListIterator PE_CPairIterator;

/// @brief Gets an iterator over the collisions of the collider.
/// @param[in] collider this.
/// @param it the iterator over the collision collection.
///
/// @sa PE_CPairIterator_MoveNext(), PE_CPairIterator_Current()
PE_INLINE void PE_CollisionContainer_GetCPairIterator(
    PE_CollisionContainer *container, PE_CPairIterator *it)
{
    PE_List_GetIterator(&container->m_list, it);
}

/// @brief Advances the iterator to the next element of the collection.
/// @param[in,out] it this.
/// @return false if the iterator has passed the end of the collection, true otherwise.
PE_INLINE bool PE_CPairIterator_MoveNext(PE_CPairIterator *it)
{
    return PE_ListIterator_MoveNext(it);
}

/// @brief Gets a pointer to the element in the collection at the current position of the iterator.
/// @param[in] it this.
/// @return The element in the collection at the current position of the iterator.
PE_INLINE PE_CollisionPair *PE_CPairIterator_Current(PE_CPairIterator *it)
{
    return (PE_CollisionPair *)PE_ListIterator_Current(it);
}

#endif
