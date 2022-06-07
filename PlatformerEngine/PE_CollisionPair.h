
/// @file PE_CollisionPair.h
/// @defgroup PE_Collision Fonctions relatives à une collision

#ifndef _PE_COLLISION_PAIR_H_
#define _PE_COLLISION_PAIR_H_

#include "PE_API.h"

#include "PE_Collider.h"
#include "PE_List.h"
#include "PE_Math.h"
#include "PE_Shape.h"
#include "PE_ShapeCollision.h"
#include "PE_ShapeDistance.h"
#include "PE_Solver.h"
#include "PE_World.h"

typedef struct PE_CollisionPair_s PE_CollisionPair;

/// @ingroup PE_Collision
/// @brief Enumération donnant les positions relatives possible d'un collider par rapport à un autre.
typedef enum PE_RelativePosition_e
{
    PE_ABOVE = 0,
    PE_LEFT  = 1,
    PE_BELOW = 2,
    PE_RIGHT = 3
} PE_RelativePosition;

int PE_RelativePosition_Compute(PE_Vec2 normal);

/// @ingroup PE_Collision
/// @brief Structure contenant les informations sur une collision entre deux colliders.
typedef struct PE_Collision_s
{
    /// @protected
    /// @brief Noeud contenant un pointeur vers cette collision dans la listes des collisions du collider parent.
    /// Ce membre appartient au collider parent.

    /// @protected
    /// @brief Pointeur vers le collider principal impliqué dans la collision.
    PE_Collider *m_collider;

    /// @protected
    /// @brief Pointeur vers le second collider impliqué dans la collision.
    PE_Collider *m_otherCollider;

    /// @protected
    /// @brief Pointeur vers la paire de collisions.
    /// Il s'agit de la structure contenant cette collision dans le détecteur de collisions.
    PE_CollisionPair *m_cPair;
} PE_Collision;

PE_INLINE PE_CollisionPair *PE_Collision_GetCollisionPair(PE_Collision *collision)
{
    return collision->m_cPair;
}

typedef enum PE_CollisionFlag_e
{
    ///@brief Flag indiquant si les deux corps d'une collision sont en contact.
    /// Il sert uniquement au solver.
    PE_COLLISION_TOUCHING    = 1 << 0,

    ///@brief Flag indiquant si la collision représente un contact persistant.
    /// Il sert uniquement pour la gestion des callbacks.
    PE_COLLISION_STAY        = 1 << 1,

    ///@brief Flag indiquant si le moment de l'impact de la collision est à jour.
    PE_COLLISION_TOI         = 1 << 2,

    ///@brief Flag indiquant que la collision est activée.
    /// Une collision peut être désactivée automatiquement lors d'une collision one-way
    /// ou manuellement par l'utilisateur dans un callback.
    PE_COLLISION_ENABLED     = 1 << 3,

    ///@brief Flag indiquant si la collision appartient à une île.
    PE_COLLISION_IN_ISLAND   = 1 << 4,
} PE_CollisionFlag;

typedef struct PE_CollisionPair_s
{
    int m_flags;

    PE_SimplexCache m_cache;

    /// @protected
    /// @brief The time of impact between the two colliders.
    float m_timeOfImpact;

    float m_friction;

    /// @protected
    /// @brief Le manifold de la collision.
    PE_Manifold m_manifold;

    /// @protected
    /// @brief Pointer to the first collider in collision.
    PE_Collider *m_colliderA;

    /// @protected
    /// @brief Pointer to the second collider in collision.
    PE_Collider *m_colliderB;

    /// @protected
    PE_ListNode m_proxyNodeA;

    /// @protected
    /// @brief Collision for the first collider.
    PE_Collision m_collisionA;

    /// @protected
    PE_ListNode m_proxyNodeB;

    /// @protected
    /// @brief Collision for the second collider.
    PE_Collision m_collisionB;

    /// @protected
    /// @brief A pointer to the solver data.
    PE_CPairSolverData m_solverData;
} PE_CollisionPair;

void PE_CollisionPair_FactoryCreate(
    PE_CollisionPair *cPair, PE_ColliderProxy *proxyA, PE_ColliderProxy *proxyB);

void PE_CollisionPair_FactoryRemove(PE_CollisionPair *cPair);

PE_INLINE int PE_CollisionPair_TestFlags(PE_CollisionPair *cPair, int flags)
{
    return cPair->m_flags & flags;
}

PE_INLINE void PE_CollisionPair_AddFlags(PE_CollisionPair *cPair, int flags)
{
    cPair->m_flags |= flags;
}

PE_INLINE void PE_CollisionPair_RemoveFlags(PE_CollisionPair *cPair, int flags)
{
    cPair->m_flags &= ~flags;
}

PE_INLINE PE_CPairSolverData *PE_CollisionPair_GetSolverData(PE_CollisionPair *cPair)
{
    return &cPair->m_solverData;
}

PE_INLINE PE_Manifold PE_CollisionPair_GetManifold(PE_CollisionPair *cPair)
{
    return cPair->m_manifold;
}

PE_INLINE PE_CPairSolverData *PE_Collision_GetSolverData(PE_Collision *collision)
{
    return PE_CollisionPair_GetSolverData(collision->m_cPair);
}

void PE_CollisionPair_Update(PE_CollisionPair *cPair);
void PE_CollisionPair_ComputeTOI(PE_CollisionPair *cPair);

PE_INLINE float PE_CollisionPair_MixFrictions(float frictionA, float frictionB)
{
    return sqrtf(frictionA * frictionB);
}

// COLLISION USER METHODS

PE_INLINE bool PE_Collision_IsEnabled(PE_Collision *collision)
{
    return (PE_CollisionPair_TestFlags(collision->m_cPair, PE_COLLISION_ENABLED)
            == PE_COLLISION_ENABLED);
}

PE_INLINE void PE_Collision_SetEnabled(PE_Collision *collision, bool enabled)
{
    if (enabled)
    {
        PE_CollisionPair_AddFlags(collision->m_cPair, PE_COLLISION_ENABLED);
    }
    else
    {
        PE_CollisionPair_RemoveFlags(collision->m_cPair, PE_COLLISION_ENABLED);
    }
}

/// @ingroup PE_Collision
/// @brief Renvoie le collider principal impliqué dans une collision.
/// @param[in] collision la collision.
/// @return Le collider principal impliqué dans la collision.
PE_INLINE PE_Collider *PE_Collision_GetCollider(PE_Collision *collision)
{
    return collision->m_collider;
}

/// @ingroup PE_Collision
/// @brief Renvoie le second collider impliqué dans une collision.
/// @param[in] collision la collision.
/// @return Le second collider impliqué dans la collision.
PE_INLINE PE_Collider *PE_Collision_GetOtherCollider(PE_Collision *collision)
{
    return collision->m_otherCollider;
}

/// @ingroup PE_Collision
/// @brief Renvoie le corps principal impliqué dans une collision.
/// @param[in] collision la collision.
/// @return Le corps principal impliqué dans la collision.
PE_INLINE PE_Body *PE_Collision_GetBody(PE_Collision *collision)
{
    return PE_Collider_GetBody(collision->m_collider);
}

/// @ingroup PE_Collision
/// @brief Renvoie le second corps impliqué dans une collision.
/// @param[in] collision la collision.
/// @return Le second corps impliqué dans la collision.
PE_INLINE PE_Body *PE_Collision_GetOtherBody(PE_Collision *collision)
{
    return PE_Collider_GetBody(collision->m_otherCollider);
}

/// @ingroup PE_Collision
/// @brief Renvoie la position relative du collider principal par rapport au second collider
/// impliqué dans une collision.
/// @param[in] collision la collision.
/// @return La position relative du collider principal par rapport au second collider.
PE_INLINE int PE_Collision_GetRelativePosition(PE_Collision *collision)
{
    PE_CollisionPair *cPair = collision->m_cPair;
    PE_Manifold manifold = cPair->m_manifold;
    if (collision == &cPair->m_collisionA)
    {
        return PE_RelativePosition_Compute(manifold.normal);
    }
    else
    {
        return PE_RelativePosition_Compute(PE_Vec2_Neg(manifold.normal));
    }
}

/// @ingroup PE_Collision
/// @brief Renvoie le manifold d'une collision.
/// @param[in] collision la collision.
/// @return Un pointeur vers le manifold de la collision.
PE_INLINE PE_Manifold PE_Collision_GetManifold(PE_Collision *collision)
{
    PE_CollisionPair *cPair = collision->m_cPair;
    if (collision == &cPair->m_collisionA)
    {
        return cPair->m_manifold;
    }
    else
    {
        PE_Manifold manifold = cPair->m_manifold;
        manifold.normal = PE_Vec2_Neg(manifold.normal);
        return manifold;
    }
}

PE_INLINE void PE_Collision_ResolveUp(PE_Collision *collision)
{
    PE_CollisionPair *cPair = collision->m_cPair;
    if (collision == &cPair->m_collisionA)
    {
        PE_Solver_SetCollisionAUp(cPair);
    }
    else
    {
        PE_Solver_SetCollisionBUp(cPair);
    }
}

PE_INLINE void PE_Collision_SetFriction(PE_Collision *collision, bool hasFriction)
{
    PE_CollisionPair *cPair = collision->m_cPair;
    PE_Solver_SetFriction(cPair, hasFriction);
}

#endif
