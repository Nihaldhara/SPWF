
/// @file PE_World.h
/// @defgroup PE_World Fonctions relatives au monde

#ifndef _PE_WORLD_H_
#define _PE_WORLD_H_

#include "PE_Settings.h"

#include "PE_Allocator.h"
#include "PE_BroadPhase.h"
#include "PE_CollisionManager.h"
#include "PE_List.h"
#include "PE_Math.h"
#include "PE_Profile.h"

typedef struct PE_Body_s PE_Body;
typedef struct PE_BodyDef_s PE_BodyDef;
typedef struct PE_Solver_s PE_Solver;

/// @ingroup PE_World
/// @struct PE_World_s
/// @brief Le monde gère et crée tous les corps physiques.
typedef struct PE_World_s
{
    /// @protected
    /// @brief Allocateur principal utilisé pour toutes les allocations mémoire dans le monde.
    PE_Allocator *m_allocator;

    /// @protected
    /// @brief Pas de temps utilisé pour chaque mise à jour du monde.
    /// Le pas de temps est fixe et varie typiquement entre 1.0f/50.0f et 1.0f/60.0f.
    float m_timeStep;

    /// @protected
    /// @brief Vecteur de gravité global du monde.
    /// Note that the gravity of a body can be scaled.
    PE_Vec2 m_gravity;

    PE_Vec2 m_up;

    /// @protected
    /// @brief Liste des corps du monde.
    PE_List m_bodyList;

    /// @protected
    /// @brief Détecteur de collisions.
    PE_CollisionManager *m_manager;

    /// @protected
    /// @brief Solver utilisé pour résoudre toutes les collisions dans ce monde.
    PE_Solver *m_solver;

    /// @protected
    /// @brief booléen permettant de bloquer l'accès à certaines fonctions lors de callback.
    bool m_locked;
} PE_World;

/// @ingroup PE_World
/// @brief Crée un nouveau monde.
/// @param[in] gravity le vecteur de gravité du monde.
/// @param[in] timeStep le pas de temps utilisé pour chaque mise à jour du monde.
/// @return Le monde créé ou NULL en cas d'erreur.
PE_World *PE_World_New(PE_Vec2 gravity, float timeStep);

/// @ingroup PE_World
/// @brief Détruit le monde.
/// Chaque entité physique appartenant à ce monde est également détruite.
/// Le pointeur world ne doit pas être déréférencé après cet appel.
/// Il est conseillé de l'affecter ensuite à NULL.
/// @param[in,out] world le monde à détruire.
void PE_World_Free(PE_World *world);

/// @ingroup PE_World
/// @brief Crée un corps indéformable dans le monde.
/// Cette fonction est verrouillée dans les fonctions de rappels (callback).
/// @param[in,out] world le monde.
/// @param[in] def la définition du corps.
/// @return Un pointeur vers le corps créé ou NULL en cas d'erreur.
PE_Body *PE_World_CreateBody(PE_World *world, PE_BodyDef *def);

/// @ingroup PE_World
/// @brief Détruit un corps indéformable.
/// Le pointeur world ne doit pas être déréférencé après cet appel.
/// Il est conseillé de l'affecter ensuite à NULL.
/// Cette fonction est verrouillée dans les fonctions de rappels (callback).
/// @param[in,out] world le monde.
/// @param[in,out] body le corps à détruire.
void PE_World_RemoveBody(PE_World *world, PE_Body *body);

/// @ingroup PE_World
/// @brief Renvoie le nombre de corps présents dans le monde.
/// @param[in] world le monde.
/// @return Le nombre de corps.
int PE_World_GetBodyCount(PE_World *world);

/// @ingroup PE_World
/// @brief Définit le vecteur de gravité global appliqué à tous les corps dynamiques du monde.
/// @param[in,out] world le monde.
/// @param[in] gravity le nouveau vecteur de gravité.
PE_INLINE void PE_World_SetGravity(PE_World *world, PE_Vec2 gravity)
{
    world->m_gravity = gravity;
    float norm = PE_Vec2_Length(gravity);
    if (norm > 0.0f)
    {
        world->m_up = PE_Vec2_Scale(gravity, -1.0f / norm);
    }
    else
    {
        world->m_up = PE_Vec2_Up;
    }
}

/// @ingroup PE_World
/// @brief Renvoie le vecteur de gravité global appliqué dans le monde.
/// @param[in] world le monde.
/// @return Le vecteur de gravité global.
PE_INLINE PE_Vec2 PE_World_GetGravity(PE_World *world)
{
    return world->m_gravity;
}

PE_INLINE float PE_World_GetTimeStep(PE_World *world)
{
    return world->m_timeStep;
}

PE_INLINE PE_Vec2 PE_World_GetUp(PE_World *world)
{
    return world->m_up;
}

/// @ingroup PE_World
/// @brief Met à jour les corps présents dans le monde d'un pas de temps.
/// Cette fonction fait met à jour les positions et effectue la détection des collisions.
/// @param[in,out] world le monde.
void PE_World_FixedUpdate(PE_World *world);

//.................................................................................................
// Query / Overlap

PE_Collider *PE_World_OverlapArea(
    PE_World *world, PE_AABB *aabb, int maskBits);

int PE_World_OverlapAreaAll(
    PE_World *world, PE_AABB *aabb, int maskBits,
    PE_Collider **dest, int destSize);

typedef bool PE_QueryCallback(PE_Collider *collider, void *data);

void PE_World_OverlapAreaExpert(
    PE_World *world, PE_AABB *aabb,
    PE_QueryCallback *callback, void *data);

//.................................................................................................
// RayCast

typedef struct PE_RayCastHit_s
{
    PE_Collider *collider;
    PE_Vec2 normal;
    PE_Vec2 hitPoint;
    float fraction;
} PE_RayCastHit;

PE_RayCastHit PE_World_RayCast(
    PE_World *world, PE_Vec2 origin, PE_Vec2 direction, float distance,
    int maskBits, bool solidOnly
);

/*int PE_World_RayCastAll(
    PE_World *world, PE_Vec2 origin, PE_Vec2 direction, float distance, int maskBits,
    PE_RayCastHit *dest, int destSize);*/

typedef bool PE_RayCastCallback(PE_RayCastHit *rayHit, void *data);

void PE_World_RayCastExpert(
    PE_World *world, PE_Vec2 origin, PE_Vec2 direction, float distance,
    PE_RayCastCallback *callback, void *data);


//.................................................................................................
// Itérateur sur les corps présents dans le monde

typedef PE_ListIterator PE_BodyIterator;

/// @ingroup PE_World
/// @brief Renvoie un itérateur sur la collection des corps présents dans le monde.
/// @param[in] world le monde.
/// @param[out] it pointeur vers l'itérateur à initialiser.
/// 
/// @sa PE_BodyIterator_MoveNext(), PE_BodyIterator_Current()
PE_INLINE void PE_World_GetBodyIterator(PE_World *world, PE_BodyIterator *it)
{
    PE_List_GetIterator(&world->m_bodyList, it);
}

/// @ingroup PE_World
/// @brief Avance l'itérateur pour qu'il pointe sur le prochain élément de la collection.
/// @param[in,out] it l'itérateur.
/// @return false si l'itérateur pointe sur le dernier élément de la collection, true sinon.
PE_INLINE bool PE_BodyIterator_MoveNext(PE_BodyIterator *it)
{
    return PE_ListIterator_MoveNext(it);
}

/// @ingroup PE_World
/// @brief Renvoie un pointeur vers l'élément de la collection à la position courante de l'itérateur.
/// @param[in] it l'itérateur.
/// @return L'élément de la collection à la position courante de l'itérateur.
PE_INLINE PE_Body *PE_BodyIterator_Current(PE_BodyIterator *it)
{
    return (PE_Body *)PE_ListIterator_Current(it);
}

// ................................................................................................
// Méthodes internes

/// @brief Renvoie l'allocateur utilisé dans ce monde.
/// @param[in] world le monde.
/// @return Un pointeur vers l'allocateur du monde.
PE_INLINE PE_Allocator *PE_World_GetAllocator(PE_World *world)
{
    return world->m_allocator;
}

/// @brief Renvoie le détecteur de collisions de ce monde.
/// @param[in] world le monde.
/// @return Un pointeur vers le détecteur de collisions.
PE_INLINE PE_CollisionManager *PE_World_GetCollisionManager(PE_World *world)
{
    return world->m_manager;
}

/// @brief Indique si le monde est bloqué.
/// Cela permet d'interdir l'utilisation de certaines fonctions de la librairie pendant un callback.
/// @param[in] world le monde.
/// @return Un booléen indiquant si le monde est bloqué.
PE_INLINE bool PE_World_IsLocked(PE_World *world)
{
    return world->m_locked;
}


#endif
