
/// @file PE_Collider.h
/// @defgroup PE_Collider Fonctions relatives à un Collider

#ifndef _PE_COLLIDER_H_
#define _PE_COLLIDER_H_

#include "PE_Settings.h"
#include "PE_Shape.h"
#include "PE_Math.h"
#include "PE_List.h"
#include "PE_Allocator.h"
#include "PE_ColliderProxy.h"
#include "PE_Solver.h"

typedef struct PE_World_s PE_World;
typedef struct PE_Body_s PE_Body;
typedef struct PE_Collision_s PE_Collision;
typedef struct PE_CollisionManager_s PE_CollisionManager;
typedef struct PE_Collider_s PE_Collider;

typedef void PE_CollisionCallback(PE_Collision *);

/// @ingroup PE_Collider
/// @brief Structure permettant de filtrer les collisions.
typedef struct PE_Filter_s
{
    /// @brief Les bits représentant les catégories d'un collider.
    /// Normalement, on ne définit qu'un seul bit à un.
    int categoryBits;

    /// @brief Les bits servant de masque pour les collisions.
    /// Ce membre définit les catégories avec lesquelles le collider peut entrer en collision.
    int maskBits;
} PE_Filter;

PE_INLINE bool PE_Filter_ShouldCollide(PE_Filter *filterA, PE_Filter *filterB)
{
    return
        (filterA->categoryBits & filterB->maskBits) &&
        (filterB->categoryBits & filterA->maskBits);
}

/// @ingroup PE_Collider
/// @brief Structure regroupant les informations nécessaires à la création d'un collider.
/// Une même définition peut servir à créer plusieurs colliders.
typedef struct PE_ColliderDef_s
{
    /// @brief Forme du collider.
    PE_Shape shape;

    /// @brief Indique si le collider est un déclencheur.
    bool isTrigger;
    
    /// @brief Indique si le collider a un comportement "one-way".
    /// Autrement dit, s'il autorise uniquement les collisions dans une seule direction.
    bool isOneWay;

    float friction;

    /// @brief Filtre à appliquer au collider pendant la détection des collisions.
    PE_Filter filter;

    /// @brief Pointeur vers les données de l'utilisateur.
    /// La librairie ne le modifie pas. Il est défini à NULL par défaut.
    void *userData;

    PE_Vec2 up;
} PE_ColliderDef;

/// @ingroup PE_Collider
/// @brief Initialise les valeurs par défaut de la définition de colliders.
/// @param[in] def la définition à initialiser.
void PE_ColliderDef_SetDefault(PE_ColliderDef *def);

typedef enum PE_ColliderFlag_e
{
    PE_COLLIDER_TRIGGER = 1 << 0,
    PE_COLLIDER_ACTIVE  = 1 << 1,
    PE_COLLIDER_ONE_WAY = 1 << 2,
} PE_ColliderFlag;

/// @ingroup PE_Collider
/// @brief Structure représentant un collider dans le moteur physique.
/// Un collider peut être affecté à un corps pour la détection de collisions.
/// Si le collider est un détecteur (trigger), il n'applique aucune force aux autres colliders/corps.
typedef struct PE_Collider_s
{
    /// @protected
    /// @brief Proxy utilisé pour connecter le collider avec le gestionnaire de collisions.
    PE_ColliderProxy *m_proxy;

    /// @protected
    /// @brief Flags du collider.
    int m_flags;

    float m_friction;

    /// @protected
    /// @brief Forme du collider.
    PE_Shape m_shape;

    /// @protected
    /// @brief Boîte englobante du collider pour le balayage courant.
    /// Cette AABB est mise à jour par le corps parent et est utilisée dans la broad phase.
    PE_AABB m_aabb;

    /// @protected
    /// @brief Filtre à appliquer au collider pendant la détection des collisions.
    PE_Filter m_filter;

    /// @protected
    /// @brief Pointeur vers le corps parent.
    PE_Body *m_body;

    PE_Vec2 m_up;

    /// @protected
    /// @brief Pointeur vers la fonction de rappel "onCollisionEnter" de ce collider.
    PE_CollisionCallback *m_onCollisionEnter;

    /// @protected
    /// @brief Pointeur vers la fonction de rappel "onCollisionStay" de ce collider.
    PE_CollisionCallback *m_onCollisionStay;

    /// @protected
    /// @brief Pointeur vers la fonction de rappel "onCollisionExit" de ce collider.
    PE_CollisionCallback *m_onCollisionExit;

    /// @protected
    /// @brief Pointeur vers les données de l'utilisateur.
    /// La librairie ne le modifie pas. Il est défini à NULL par défaut.
    void *m_userData;
} PE_Collider;

/// @brief Crée un nouveau collider.
/// Cette fonction est utilisée par PE_Body_CreateCollider() pour créer un collider.
/// Elle ne doit pas être utilisée directement.
/// @param[in] def la définition du collider.
/// @param[in] allocator l'allocateur principal du monde.
/// @return Un pointeur vers le collider créé ou NULL en cas d'erreur.
void PE_Collider_FactoryCreate(PE_Collider *collider, PE_ColliderDef *def, PE_Body *body, PE_Allocator *allocator);

/// @brief Détruit un collider.
/// Cette fonction est utilisée par PE_Body_RemoveCollider() pour détruire un collider.
/// Elle ne doit pas être utilisée directement.
/// @param[in,out] collider la collider à supprimer.
/// @param[in] allocator l'allocateur principal du monde.
void PE_Collider_FactoryRemove(PE_Collider *collider, PE_Allocator *allocator);

/// @brief Crée un proxy d'un collider avec le gestionnaire de collisions.
/// Cette fonction est utilisée par le corps parent.
/// @param collider 
/// @param manager 
/// @param transform 
/// @return 
void PE_Collider_CreateProxy(
    PE_Collider *collider, PE_CollisionManager *manager, PE_Vec2 transform);

/// @brief Détruit le proxy d'un collider avec le gestionnaire de collisions.
/// Cette fonction est utilisée par le corps parent.
/// @param collider 
/// @param manager 
void PE_Collider_RemoveProxy(PE_Collider *collider, PE_CollisionManager *manager);

PE_INLINE PE_ColliderProxy *PE_Collider_GetProxy(PE_Collider *collider)
{
    return collider->m_proxy;
}

PE_INLINE void PE_Collider_AddFlags(PE_Collider *collider, int flags)
{
    collider->m_flags |= flags;
}

PE_INLINE void PE_Collider_RemoveFlags(PE_Collider *collider, int flags)
{
    collider->m_flags &= ~flags;
}

PE_INLINE int PE_Collider_TestFlags(PE_Collider *collider, int flags)
{
    return (collider->m_flags & flags);
}

/// @ingroup PE_Collider
/// @brief Définit si un collider est un détecteur.
/// @param[in,out] collider le collider.
/// @param isTrigger peut valoir true ou false.
PE_INLINE void PE_Collider_SetTrigger(PE_Collider *collider, bool isTrigger)
{
    if (isTrigger)
        PE_Collider_AddFlags(collider, PE_COLLIDER_TRIGGER);
    else
        PE_Collider_RemoveFlags(collider, PE_COLLIDER_TRIGGER);
}

/// @ingroup PE_Collider
/// @brief Indique si un collider est un détecteur.
/// @param[in] collider le collider.
/// @return true si le collider est un détecteur, false sinon.
PE_INLINE bool PE_Collider_IsTrigger(PE_Collider *collider)
{
    return PE_Collider_TestFlags(collider, PE_COLLIDER_TRIGGER)
        == PE_COLLIDER_TRIGGER;
}

PE_INLINE bool PE_Collider_IsOneWay(PE_Collider *collider)
{
    return PE_Collider_TestFlags(collider, PE_COLLIDER_ONE_WAY)
        == PE_COLLIDER_ONE_WAY;
}

/// @ingroup PE_Collider
/// @brief Renvoie le corps parent d'un collider.
/// @param[in] collider le collider.
/// @return Un pointeur vers le corps parent du collider.
PE_INLINE PE_Body *PE_Collider_GetBody(PE_Collider *collider)
{
    return collider->m_body;
}

/// @ingroup PE_Collider
/// @brief Renvoie la forme d'un collider.
/// @param[in] collider le collider.
/// @return La forme du collider.
PE_INLINE PE_Shape *PE_Collider_GetShape(PE_Collider *collider)
{
    return &collider->m_shape;
}

PE_INLINE PE_Vec2 PE_Collider_GetUp(PE_Collider *collider)
{
    return collider->m_up;
}

/// @ingroup PE_Collider
/// @brief Renvoie la boîte englobante d'un collider.
/// @param[in] collider le collider.
/// @return Un pointeur vers la boîte englobante du collider.
PE_AABB *PE_Collider_GetAABB(PE_Collider *collider);

/// @ingroup PE_Collider
/// @brief Définit les données de l'utilisateur d'un collider.
/// Ce pointeur sert à stocker les données spécifiques de votre application en lien avec un collider.
/// La librairie ne modifie pas ces données.
/// @param[in] collider le collider.
/// @param[in] data les données de l'utilisateur.
void PE_Collider_SetUserData(PE_Collider *collider, void *data);

/// @ingroup PE_Collider
/// @brief Renvoie les données de l'utilisateur affectées à un collider.
/// @param[in] collider le collider.
/// @return Les données de l'utilisateur affectées au collider.
void *PE_Collider_GetUserData(PE_Collider *collider);

/// @ingroup PE_Collider
/// @brief Définit la fonction de rappel (callback) appelée à chaque fois qu'un collider
//  entre en collision avec un autre collider.
/// Les références vers les objets donnés dans un callback ne doivent pas être sauvegardées,
/// elle sont invalidées après la fin de la fonction.
/// @param[in,out] collider le collider auquel affecter la fonction de rappel.
/// @param[in] onCollisionEnter la fonction de rappel.
PE_INLINE void PE_Collider_SetOnCollisionEnter(
    PE_Collider *collider,
    PE_CollisionCallback *onCollisionEnter)
{
    collider->m_onCollisionEnter = onCollisionEnter;
}

/// @ingroup PE_Collider
/// @brief Définit la fonction de rappel (callback) appelée à chaque fois qu'un collider
/// touche un autre collider.
/// Les références vers les objets donnés dans un callback ne doivent pas être sauvegardées,
/// elle sont invalidées après la fin de la fonction.
/// @param[in,out] collider le collider auquel affecter la fonction de rappel.
/// @param[in] onCollisionStay la fonction de rappel.
PE_INLINE void PE_Collider_SetOnCollisionStay(
    PE_Collider *collider,
    PE_CollisionCallback *onCollisionStay)
{
    collider->m_onCollisionStay = onCollisionStay;
}

/// @ingroup PE_Collider
/// @brief Définit la fonction de rappel (callback) appelée à chaque fois qu'un collider
/// cesse d'être en collision avec un autre collider.
/// Les références vers les objets donnés dans un callback ne doivent pas être sauvegardées,
/// elle sont invalidées après la fin de la fonction.
/// @param[in,out] collider le collider auquel affecter la fonction de rappel.
/// @param[in] onCollisionExit la fonction de rappel.
PE_INLINE void PE_Collider_SetOnCollisionExit(
    PE_Collider *collider,
    PE_CollisionCallback *onCollisionExit)
{
    collider->m_onCollisionExit = onCollisionExit;
}

/// @ingroup PE_Collider
/// @brief Renvoie la fonction de rappel "onCollisionEnter" affectée à un collider.
/// @param[in] collider le collider.
/// @return La fonction de rappel "onCollisionEnter" du collider.
PE_INLINE PE_CollisionCallback *PE_Collider_GetOnCollisionEnter(PE_Collider *collider)
{
    return collider->m_onCollisionEnter;
}

/// @ingroup PE_Collider
/// @brief Renvoie la fonction de rappel "onCollisionStay" affectée à un collider.
/// @param[in] collider le collider.
/// @return La fonction de rappel "onCollisionStay" du collider.
PE_INLINE PE_CollisionCallback *PE_Collider_GetOnCollisionStay(PE_Collider *collider)
{
    return collider->m_onCollisionStay;
}

/// @ingroup PE_Collider
/// @brief Renvoie la fonction de rappel "onCollisionExit" affectée à un collider.
/// @param[in] collider le collider.
/// @return La fonction de rappel "onCollisionExit" du collider.
PE_INLINE PE_CollisionCallback *PE_Collider_GetOnCollisionExit(PE_Collider *collider)
{
    return collider->m_onCollisionExit;
}

PE_INLINE void PE_Collider_OnCollisionEnter(PE_Collider *collider, PE_Collision *collision)
{
    if (collider->m_onCollisionEnter)
        collider->m_onCollisionEnter(collision);
}

PE_INLINE void PE_Collider_OnCollisionStay(PE_Collider *collider, PE_Collision *collision)
{
    if (collider->m_onCollisionStay)
        collider->m_onCollisionStay(collision);
}

PE_INLINE void PE_Collider_OnCollisionExit(PE_Collider *collider, PE_Collision *collision)
{
    if (collider->m_onCollisionExit)
        collider->m_onCollisionExit(collision);
}

void PE_Collider_Synchronize(
    PE_Collider *collider, PE_CollisionManager *manager,
    PE_Vec2 transform0, PE_Vec2 transform1);

PE_INLINE bool PE_Collider_ShouldCollide(PE_Collider *colliderA, PE_Collider *colliderB)
{
    return PE_Filter_ShouldCollide(&colliderA->m_filter, &colliderB->m_filter)
        && (colliderA->m_body != colliderB->m_body);
}

PE_INLINE bool PE_Collider_CheckCategory(PE_Collider *collider, int maskBits)
{
    return (collider->m_filter.categoryBits & maskBits) != 0;
}

/// @}

#endif