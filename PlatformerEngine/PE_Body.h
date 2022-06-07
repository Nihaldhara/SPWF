/// @file PE_Body.h
/// @defgroup PE_Body Fonctions relatives à un corps

#ifndef _PE_BODY_H_
#define _PE_BODY_H_

#include "PE_Settings.h"

#include "PE_Collider.h"
#include "PE_CollisionPair.h"
#include "PE_List.h"
#include "PE_Math.h"
#include "PE_Solver.h"

typedef struct PE_World_s PE_World;
typedef struct PE_Body_s PE_Body;
typedef struct PE_SolverBody_s PE_SolverBody;

/// @ingroup PE_Body
/// @brief Enumération donnant les types possibles pour un corps.
typedef enum PE_BodyType_e
{
    /// @brief Un corps statique est conçu pour ne jamais se déplacer sous simulation,
    /// même en cas de collision. Un corps statique ne peut entrer en collision qu'avec un
    /// corps dynamique.
    PE_STATIC_BODY = 0,

    /// @brief Un corps cinématique est conçu pour se déplacer sous simulation,
    /// mais seulement sous le contrôle explicite de l'utilisateur.
    /// Il n'est pas soumis à la gravité et ne peut entrer en collision qu'avec un
    /// corps dynamique.
    PE_KINEMATIC_BODY,

    /// @brief Un corps dynamique est conçu pour se déplacer sous simulation.
    /// Il est soumis à la gravité et peut potentiellement enter en collision avec n'importe
    /// quel autre corps.
    PE_DYNAMIC_BODY
} PE_BodyType;

/// @ingroup PE_Body
/// @brief Structure regroupant les informations nécessaires à la création d'un corps.
/// Une même définition peut servir à créer plusieurs corps.
typedef struct PE_BodyDef_s
{
    /// @brief Le type du corps.
    PE_BodyType type;

    /// @brief La position du corps exprimée dans le référentiel monde.
    PE_Vec2 position;

    /// @brief La vitesse linéaire de l'origine du corps exprimée dans le référentiel monde.
    PE_Vec2 velocity;

    /// @brief L'amortissement linéaire du corps sur l'axe des abscisses.
    /// Ce paramètre permet de réduire la vitesse linéaire horizontale du corps.
    float xDamping;

    /// @brief L'amortissement linéaire du corps sur l'axe des ordonnées.
    /// Ce paramètre permet de réduire la vitesse linéaire verticale du corps.
    float yDamping;

    /// @brief L'échelle de gravité du corps.
    float gravityScale;

    float mass;

    /// @brief Pointeur vers les données de l'utilisateur.
    /// La librairie ne le modifie pas. Il est défini à NULL par défaut.
    void *userData;

    char *name;
} PE_BodyDef;

/// @ingroup PE_Body
/// @brief Initialise les valeurs par défaut de la définition de corps.
/// @param[in] def la définition à initialiser.
void PE_BodyDef_SetDefault(PE_BodyDef *def);

typedef enum PE_BodyFlags_e
{
    PE_BODY_AWAKE        = 1 << 0,
    PE_BODY_ENABLED      = 1 << 1,
    PE_BODY_INTEGRATED   = 1 << 2,
    PE_BODY_SOLVE_FORCES = 1 << 3,
    //PE_BODY_IN_ISLAND  = 1 << 2,
} PE_BodyFlags;

// Différence entre SIMULATED et AWAKE
// not SIMULATED : aucune participation au moteur physique (les colliders ne sont pas dans la broadphase)
// not AWAKE : Pas de mise à jour de la position (plus d'intégration ni de modification par le solver)
//             Les collisions sont possibles.

/// @ingroup PE_Body
/// @brief Structure représentant un corps indéformable dans le moteur physique.
struct PE_Body_s
{
    char *m_name;

    /// @protected
    /// @brief Type du corps.
    PE_BodyType m_type;

    /// @protected
    /// @brief Flags du corps.
    int m_flags;

    /// @protected
    /// @brief Position du corps exprimée dans le référentiel monde.
    PE_Vec2 m_position;

    PE_Vec2 m_lastPos;

    /// @protected
    /// @brief Vitesse linéaire de l'origine du corps exprimée dans le référentiel monde.
    PE_Vec2 m_localVelocity;

    PE_Vec2 m_externVelocity;

    /// @protected
    /// @brief Somme des forces appliquées au corps, exprimée dans le référentiel monde.
    PE_Vec2 m_forces;

    PE_Vec2 m_solverForces;

    PE_Vec2 m_solverImpulse;

    /// @protected
    /// @brief Amortissement linéaire du corps sur l'axe des abscisses.
    /// Ce paramètre permet de réduire la vitesse linéaire horizontale du corps.
    float m_xDamping;

    /// @protected
    /// @brief Amortissement linéaire du corps sur l'axe des ordonnées.
    /// Ce paramètre permet de réduire la vitesse linéaire verticale du corps.
    float m_yDamping;

    /// @protected
    /// @brief Echelle de gravité du corps.
    float m_gravityScale;

    float m_mass;
    float m_invMass;

    /// @protected
    /// @brief Pointeur vers le monde parent de ce corps.
    PE_World *m_world;

    /// @protected
    /// @brief Liste des colliders attachés à ce corps.
    PE_List m_colliderList;

    /// @protected
    /// @brief Décrit le mouvement du corps pendant un pas d'intégration.
    PE_Sweep m_sweep;

    PE_Body *m_parent;
    PE_Body **m_children;
    int m_childCount;
    int m_childCapacity;

    /// @protected
    /// @brief Données du solver associées à ce corps.
    PE_BodySolverData m_solverData;

    /// @protected
    /// @brief Pointeur vers les données de l'utilisateur.
    /// La librairie ne le modifie pas. Il est défini à NULL par défaut.
    void *m_userData;

};

void PE_Body_FactoryCreate(PE_Body *body, PE_BodyDef *def, PE_World *world);

void PE_Body_FactoryRemove(PE_Body *body);

/// @ingroup PE_Body
/// @brief Crée un nouveau collider et l'attache à un corps.
/// La forme du collider est décrite dans le référentiel local au corps.
/// @param[in,out] body le corps auquel attacher le collider.
/// @param[in] def la définition du collider.
/// @return Un pointeur vers le collider créé ou NULL en cas d'erreur.
PE_Collider *PE_Body_CreateCollider(PE_Body *body, PE_ColliderDef *def);

/// @ingroup PE_Body
/// @brief Supprime un collider attaché à un corps.
/// Le pointeur world ne doit pas être déréférencé après cet appel.
/// Il est conseillé de l'affecter ensuite à NULL.
/// @param[in,out] body le corps.
/// @param[in,out] collider le collider à supprimer du corps.
void PE_Body_RemoveCollider(PE_Body *body, PE_Collider *collider);

/// @ingroup PE_Body
/// @brief Renvoie le nombre de colliders attachés à un corps.
/// @param[in] body le corps.
/// @return Le nombre de colliders du corps.
PE_INLINE int PE_Body_GetColliderCount(PE_Body *body)
{
    return PE_List_GetNodeCount(&body->m_colliderList);
}

/// @ingroup PE_Body
/// @brief Renvoie le type d'un corps.
/// @param[in] body le corps.
/// @return Le type du corps.
PE_INLINE int PE_Body_GetType(PE_Body *body)
{
    return body->m_type;
}

/// @brief Integrate the body's position using its velocity.
/// @param[in,out] body this.
/// @param[in] timeStep the step. 
void PE_Body_Integrate(PE_Body *body, float timeStep);

void PE_Body_IntegratePosition(PE_Body *body, float timeStep);

void PE_Body_SynchronizeColliders(PE_Body *body);


PE_INLINE void PE_Body_AddFlags(PE_Body *body, int flags)
{
    body->m_flags |= flags;
}

PE_INLINE void PE_Body_RemoveFlags(PE_Body *body, int flags)
{
    body->m_flags &= ~flags;
}

PE_INLINE int PE_Body_TestFlags(PE_Body *body, int flags)
{
    return (body->m_flags & flags);
}

PE_INLINE bool PE_Body_IsAwake(PE_Body *body)
{
    return PE_Body_TestFlags(body, PE_BODY_AWAKE) == PE_BODY_AWAKE;
}

/// @brief Définit l'état de sommeil d'un corps.
/// @param body le corps.
/// @param awake booléen indiquant s'il faut réveiller le corps. 
void PE_Body_SetAwake(PE_Body *body, bool awake);

PE_INLINE bool PE_Body_IsEnabled(PE_Body *body)
{
    return PE_Body_TestFlags(body, PE_BODY_ENABLED) == PE_BODY_ENABLED;
}

void PE_Body_SetEnabled(PE_Body *body, bool enabled);

//PE_INLINE void PE_Body_SetType(PE_Body *body, PE_BodyType type)


/// @ingroup PE_Body
/// @brief Définit la position de l'origine du corps (dans le référentiel monde).
/// Les collisions seront mises à jour lors du prochain appel à PE_World_fixedUpdate().
/// Cette fonction est verrouillée dans les fonctions de rappels (callback).
/// @param[in,out] body le corps.
/// @param[in] position la nouvelle position de l'origine du corps.
void PE_Body_SetPosition(PE_Body *body, PE_Vec2 position);

/// @ingroup PE_Body
/// @brief Renvoie la position de l'origine du corps (dans le référentiel monde).
/// Cette position correspond à la position du corps lors du dernier appel à PE_World_fixedUpdate().
/// @param[in] body le corps.
/// @param[out] position pointeur vers le vecteur dans lequel copier la position du corps.
PE_INLINE PE_Vec2 PE_Body_GetPosition(PE_Body *body)
{
    return body->m_position;
}

/// @ingroup PE_Body
/// @brief Renvoie la position interpolée de l'origine du corps (dans le référentiel monde).
/// Cette position correspond l'interpolation linéaire entre la position du corps lors
/// du dernier appel à PE_World_fixedUpdate() et celle du prochain appel.
/// @param[in] body le corps.
/// @param[in] alpha ratio entre 0.0f et 1.0f servant à l'interpolation.
/// @param[out] position pointeur vers le vecteur dans lequel copier la position du corps.
PE_INLINE PE_Vec2 PE_Body_GetInterpolation(PE_Body *body, float alpha)
{

    return PE_Vec2_Add(
        PE_Vec2_Scale(body->m_lastPos, 1.0f - alpha),
        PE_Vec2_Scale(body->m_position, alpha)
    );
}

/// @ingroup PE_Body
/// @brief Définit la vitesse de l'origine du corps (dans le référentiel monde).
/// Les collisions seront mises à jour lors du prochain appel à PE_World_fixedUpdate().
/// Cette fonction est verrouillée dans les fonctions de rappel (callback).
/// @param[in,out] body le corps.
/// @param[in] velocity la nouvelle vitesse de l'origine du corps.
/// @sa PE_Body_SetVelocity()
void PE_Body_SetVelocity(PE_Body *body, PE_Vec2 velocity);

PE_INLINE PE_Vec2 PE_Body_GetLocalVelocity(PE_Body *body)
{
    return body->m_localVelocity;
}


PE_INLINE PE_Vec2 PE_Body_GetExternVelocity(PE_Body *body)
{
    return body->m_externVelocity;
}

/// @ingroup PE_Body
/// @brief Renvoie la vitesse de l'origine du corps (dans le référentiel monde).
/// Cette vitesse correspond à la vitesse du corps lors du dernier appel à PE_World_fixedUpdate().
/// @param[in] body le corps.
/// @param[out] velocity pointeur vers le vecteur dans lequel copier la vitesse du corps.
PE_Vec2 PE_Body_GetVelocity(PE_Body *body);

void PE_Body_SetParent(PE_Body *body, PE_Body *parent);

PE_INLINE PE_Body *PE_Body_GetParent(PE_Body *body)
{
    assert(body);
    return body->m_parent;
}

/// @ingroup PE_Body
/// @brief Définit la nouvelle vitesse de l'origine du corps (dans le référentiel monde)
/// en réponse à une collision.
/// Contrairement à PE_Body_SetVelocity(), cette fonction n'est pas verrouillée dans les fonctions de rappel.
/// Elle ne réveille pas un corps endormi.
/// @param[in,out] body le corps.
/// @param[in] velocity la nouvelle vitesse de l'origine du corps.
/// @sa PE_Body_SetVelocity()
PE_INLINE void PE_Body_SetCollisionResponse(PE_Body *body, PE_Vec2 velocity)
{
    if (body->m_type != PE_DYNAMIC_BODY)
    {
        return;
    }
    
    body->m_localVelocity = velocity;
}

/// @ingroup PE_Body
/// @brief Applique une force à un corps.
/// Comme tous les corps ont une masse unitaire, la somme des forces appliquées à un corps
/// correspond à son accélération.
/// @param[in,out] body le corps.
/// @param[in] force la force à appliquer au corps.
PE_INLINE void PE_Body_ApplyForce(PE_Body *body, PE_Vec2 force)
{
    if (body->m_type != PE_DYNAMIC_BODY)
    {
        return;
    }

    PE_Body_SetAwake(body, true);
    body->m_forces = PE_Vec2_Add(body->m_forces, force);
}

PE_INLINE void PE_Body_ApplySolverForce(PE_Body *body, PE_Vec2 force)
{
    if (body->m_type != PE_DYNAMIC_BODY)
    {
        return;
    }
    body->m_solverForces = PE_Vec2_Add(body->m_solverForces, force);
}

/// @ingroup PE_Body
/// @brief Applique une impulsion à un corps.
/// L'impulsion s'ajoute directement à la vitesse du corps.
/// @param[in,out] body le corps.
/// @param[in] impulse l'impulsion à appliquer au corps.
PE_INLINE void PE_Body_ApplyImpulse(PE_Body *body, PE_Vec2 impulse)
{
    if (body->m_type != PE_DYNAMIC_BODY)
    {
        return;
    }

    PE_Body_SetAwake(body, true);

    body->m_localVelocity = PE_Vec2_Add(
        body->m_localVelocity,
        PE_Vec2_Scale(impulse, body->m_invMass));
}

PE_INLINE void PE_Body_ApplySolverImpulse(PE_Body *body, PE_Vec2 impulse)
{
    if (body->m_type != PE_DYNAMIC_BODY)
    {
        return;
    }
    body->m_localVelocity = PE_Vec2_Add(body->m_localVelocity, impulse);
    body->m_solverImpulse = PE_Vec2_Add(body->m_solverImpulse, impulse);
}

/// @ingroup PE_Body
/// @brief Annule les forces appliquées à un corps.
/// @param[in,out] body le corps.
PE_INLINE void PE_Body_ClearForces(PE_Body *body)
{
    body->m_forces = PE_Vec2_Set(0.0f, 0.0f);
}

PE_INLINE PE_Vec2 PE_Body_GetNetForce(PE_Body *body)
{
    return body->m_forces;
}

PE_INLINE PE_Vec2 PE_Body_GetNetSolverForce(PE_Body *body)
{
    return body->m_solverForces;
}

/// @ingroup PE_Body
/// @brief Définit l'amortissement linéaire d'un corps sur l'axe des abscisses.
/// Ce paramètre permet de réduire la vitesse linéaire horizontale du corps.
/// @param[in,out] body le corps.
/// @param[in] xDamping l'amortissement linéaire sur l'axe des abscisses.
PE_INLINE void PE_Body_SetXDamping(PE_Body *body, float xDamping)
{
    body->m_xDamping = xDamping;
}

/// @ingroup PE_Body
/// @brief Définit l'amortissement linéaire d'un corps sur l'axe des ordonnées.
/// Ce paramètre permet de réduire la vitesse linéaire verticale du corps.
/// @param[in,out] body le corps.
/// @param[in] yDamping l'amortissement linéaire sur l'axe des ordonnées.
PE_INLINE void PE_Body_SetYDamping(PE_Body *body, float yDamping)
{
    body->m_yDamping = yDamping;
}

/// @ingroup PE_Body
/// @brief Renvoie l'amortissement linéaire d'un corps sur l'axe des abscisses.
/// @param[in] body le corps.
/// @return L'amortissement linéaire du corps sur l'axe des abscisses.
PE_INLINE float PE_Body_GetXDamping(PE_Body *body)
{
    return body->m_xDamping;
}

/// @ingroup PE_Body
/// @brief Renvoie l'amortissement linéaire d'un corps sur l'axe des ordonnées.
/// @param[in] body le corps.
/// @return L'amortissement linéaire du corps sur l'axe des ordonnées.
PE_INLINE float PE_Body_GetYDamping(PE_Body *body)
{
    return body->m_yDamping;
}

/// @ingroup PE_Body
/// @brief Définit l'échelle de gravité d'un corps.
/// La force de gravité appliquée à un corps est égale au vecteur gravité du monde
/// multiplié par l'échelle de gravité du corps.
/// @param[in] body le corps.
/// @param[in] gravityScale l'échelle de gravité du corps. 
PE_INLINE void PE_Body_SetGravityScale(PE_Body *body, float gravityScale)
{
    body->m_gravityScale = gravityScale;
}

/// @ingroup PE_Body
/// @brief Renvoie l'échelle de gravité d'un corps.
/// @param[in] body le corps.
/// @return L'échelle de gravité du corps.
PE_INLINE float PE_Body_GetGravityScale(PE_Body *body)
{
    return body->m_gravityScale;
}

PE_INLINE PE_BodySolverData *PE_Body_GetSolverData(PE_Body *body)
{
    return &body->m_solverData;
}

/// @ingroup PE_Body
/// @brief Définit les données de l'utilisateur d'un corps.
/// Ce pointeur sert à stocker les données spécifiques de votre application en lien avec un corps.
/// La librairie ne modifie pas ces données.
/// @param[in] body le corps.
/// @param[in] data les données de l'utilisateur.
PE_INLINE void PE_Body_SetUserData(PE_Body *body, void *data)
{
    body->m_userData = data;
}

/// @ingroup PE_Body
/// @brief Renvoie les données de l'utilisateur affectées à un corps.
/// @param[in] body le corps.
/// @return Les données de l'utilisateur affectées au corps.
PE_INLINE void *PE_Body_GetUserData(PE_Body *body)
{
    return body->m_userData;
}

/// @ingroup PE_Body
/// @brief Renvoie le monde parent d'un corps.
/// @param[in] body le corps.
/// @return Le monde parent du corps.
PE_INLINE PE_World *PE_Body_GetWorld(PE_Body *body)
{
    return body->m_world;
}

PE_INLINE bool PE_Body_ShouldCollide(PE_Body *bodyA, PE_Body *bodyB)
{
    return (PE_Body_IsAwake(bodyA) || PE_Body_IsAwake(bodyB))
        && (bodyA->m_type == PE_DYNAMIC_BODY || bodyB->m_type == PE_DYNAMIC_BODY);
}

PE_INLINE PE_Sweep PE_Body_GetSweep(PE_Body *body)
{
    return body->m_sweep;
}

// Probablement inutile si la position est synchronisée
PE_INLINE PE_Vec2 PE_Body_GetSweepPosition(PE_Body *body)
{
    return body->m_sweep.position0;
}

PE_INLINE void PE_Body_SetSweepPosition(PE_Body *body, PE_Vec2 position)
{
    body->m_sweep.position0 = position;
}

PE_INLINE void PE_Body_SetSweep(PE_Body *body, PE_Sweep sweep)
{
    body->m_sweep = sweep;
}

PE_INLINE void PE_Body_SynchronizeTransform(PE_Body *body)
{
    body->m_position = body->m_sweep.position1;
}

PE_INLINE void PE_Body_Advance(PE_Body *body, float alpha)
{
    PE_Sweep_Advance(&body->m_sweep, alpha);
    PE_Body_SynchronizeTransform(body);
}

void PE_Body_FinalizeStep(PE_Body *body);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Iterator

typedef PE_ListIterator PE_ColliderIterator;

/// @ingroup PE_Body
/// @brief Renvoie un itérateur sur la collection des colliders attaché à un corps dans le monde.
/// @param[in] body le corps.
/// @param[out] it pointeur vers l'itérateur à initialiser.
/// 
/// @sa PE_BodyIterator_MoveNext(), PE_BodyIterator_Current()
PE_INLINE void PE_Body_GetColliderIterator(PE_Body *body, PE_ColliderIterator *it)
{
    PE_List_GetIterator(&body->m_colliderList, it);
}

/// @ingroup PE_Body
/// @brief Avance l'itérateur pour qu'il pointe sur le prochain élément de la collection.
/// @param[in,out] it l'itérateur.
/// @return false si l'itérateur pointe sur le dernier élément de la collection, true sinon.
PE_INLINE bool PE_ColliderIterator_MoveNext(PE_ColliderIterator *it)
{
    return PE_ListIterator_MoveNext(it);
}

/// @ingroup PE_Body
/// @brief Renvoie un pointeur vers l'élément de la collection à la position courante de l'itérateur.
/// @param[in] it l'itérateur.
/// @return L'élément de la collection à la position courante de l'itérateur.
PE_INLINE PE_Collider *PE_ColliderIterator_Current(PE_ColliderIterator *it)
{
    return (PE_Collider *)PE_ListIterator_Current(it);
}

#endif
