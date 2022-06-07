#ifndef _SCENE_H_
#define _SCENE_H_

#include "../Settings.h"
#include "../Common.h"

#include "../GameObject/Camera/Camera.h"
#include "../GameObject/Player.h"
#include "../GameObject/UI/Canvas.h"
#include "../GameObject/GameObject.h"

#include "../Utils/List.h"
#include "../AssetManager.h"
#include "../Input/InputManager.h"
#include "../Utils/ObjectManager.h"

void Class_InitScene();

/// @brief Description de la classe "Scene".
extern const void *const Class_Scene;

typedef struct SceneClass_s
{
    const Class base;

    // Messages
    void (*Render)(void *self);
    bool (*Update)(void *self);
} SceneClass;

typedef enum FixedUpdateMode_e
{
    FIXED_UPDATE_REALTIME,
    FIXED_UPDATE_STEP_BY_STEP
} FixedUpdateMode;

typedef struct SceneStats_s
{
    /// @brief Nombre d'objets visibles par la caméra active.
    int visibleCount;

    /// @brief Nombre d'objets présents dans la scène.
    int objectCount;

    /// @brief Nombre d'objets inactifs.
    int disabledCount;

    /// @brief Nombre d'objets à réinitialiser quand le joueur meurt.
    int toRespawnCount;

    /// @brief Nombre de corps présents dans le moteur physique.
    int bodyCount;
} SceneStats;

typedef struct Scene_s
{
    struct {
        Object base;
    } m_super;

    /// @brief Moteur de rendu.
    SDL_Renderer *m_renderer;

    /// @brief Moteur physique.
    PE_World *m_world;

    /// @brief Gestionnaire des instances de GameObject.
    ObjectManager *m_objManager;

    /// @brief Gestionnaire des entrées utilisateur.
    InputManager *m_inputManager;

    /// @brief Gestionnaire des ressources de la scène.
    AssetManager *m_assets;

    /// @brief Pointeur vers la caméra utilisée pour rendre la scène.
    Camera *m_activeCam;

    /// @brief Pas de temps fixe.
    float m_timeStep;

    /// @brief Accumulateur pour la mise à jour à pas de temps fixe.
    float m_accu;

    /// @brief Paramètre d'interpolation pour les positions des corps physiques.
    float m_alpha;

    /// @brief Booléen indiquant s'il faut afficher les "gizmos" (debug).
    bool m_drawGizmos;

    /// @brief Booléen indiquant s'il faut afficher corps (debug).
    bool m_drawBodies;

    /// @brief Mode de mise à jour du moteur physique de la scène (temps réel ou pas-à-pas).
    int m_mode;

    /// @brief Booléen indiquant s'il faut effectuer un pas fixe en mode pas-à-pas.
    bool m_makeStep;

    /// @brief Booléen indiquant s'il faut réinitialiser la scène.
    bool m_respawn;

    /// @brief Booléen indiquant s'il faut quitter la scène.
    bool m_quit;

    /// @brief Information sur l'état de la scène.
    SceneStats m_stats;
} Scene;

void Scene_Constructor(void *self, SDL_Renderer *renderer, int theme);

INLINE void Scene_Delete(void *self)
{
    Object_Delete(self);
}

INLINE bool Scene_Update(void *self)
{
    assert(Object_IsA(self, Class_Scene));
    const SceneClass *clsDesc = Object_GetClass(self);
    return (clsDesc->Update)(self);
}

INLINE void Scene_Render(void *self)
{
    assert(Object_IsA(self, Class_Scene));
    const SceneClass *clsDesc = Object_GetClass(self);
    (clsDesc->Render)(self);
}

void *Scene_AllocateObject(void *self, const void *clsDesc);
void Scene_DeleteObject(void *self, void *object);

void Scene_EnableObject(void *self, void *object);
void Scene_DisableObject(void *self, void *object);

void Scene_SetVisible(void *self, void *object);
void Scene_SetToRespawn(void *self, void *object, bool toRespawn);

INLINE void Scene_SetActiveCamera(void *self, void *camera)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    scene->m_activeCam = camera;
}

INLINE void *Scene_GetActiveCamera(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    return scene->m_activeCam;
}

INLINE InputManager *Scene_GetInputManager(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    return scene->m_inputManager;
}

INLINE AssetManager *Scene_GetAssetManager(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    return scene->m_assets;
}

INLINE PE_World *Scene_GetWorld(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    return scene->m_world;
}

INLINE SDL_Renderer *Scene_GetRenderer(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    return scene->m_renderer;
}

INLINE void Scene_Respawn(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    scene->m_respawn = true;
}

INLINE void Scene_Quit(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    scene->m_quit = true;
}

INLINE float Scene_GetAlpha(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    return scene->m_alpha;
}

INLINE float Scene_GetFixedTimeStep(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    return scene->m_timeStep;
}

INLINE void Scene_SetDrawGizmos(void *self, bool drawGizmos)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    scene->m_drawGizmos = drawGizmos;
}

INLINE bool Scene_AreGizmosDrawn(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    return scene->m_drawGizmos;
}

INLINE void Scene_SetDrawBodies(void *self, bool drawBodies)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    scene->m_drawBodies = drawBodies;
}

INLINE bool Scene_AreBodiesDrawn(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    return scene->m_drawBodies;
}

INLINE void Scene_SetFixedUpdateMode(void *self, int mode)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    scene->m_mode = mode;
}

INLINE int Scene_GetFixedUpdateMode(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    return scene->m_mode;
}

INLINE void Scene_MakeStep(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    if (scene->m_mode == FIXED_UPDATE_STEP_BY_STEP)
    {
        scene->m_makeStep = true;
    }
}

INLINE SceneStats Scene_GetStats(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    return scene->m_stats;
}

INLINE ObjectManager *Scene_GetObjectManager(void *self)
{
    Scene *scene = Object_Cast(self, Class_Scene);
    return scene->m_objManager;
}

/*
    Terrain
    - Ground, Steep Slope, Gentle Slope
    - Pipe, [3D] Clear Pipe
    - Spike Trap, Mushroom Platform
    - Semisolid Platform (~One way), Bridge (~One way)
    - Block (~ Brick), ? Block (~ Bonus), Hard Block (~ Wood), Hidden Block
    - Donut Block (tombe quand on monte dessus), Note Block (rebondit)
    - Cloud Block, Ice Block

    Items
    - Coin, 10-coin, Pink Coin (clé)
    - Super mushroom, Fire flower, Super leaf, Frog suit, Super star, 1-up Mushroom, Shoe Goomba

    Enemies
    - Goomba, Koopa Troopa, Buzzy beetle, Spike top, Spiny, Blooper, Cheep cheep
    - Piranha Plant, Muncher, Thwomp, Monty Mole, Rocky Wrench, Hammer Bro, Chain Chomp, Spike

    Gizmos
    - Burner, Bill Blaster, Banzai Bill, Cannon, Icicle, Twister,
    - Key, Wrap Door, P Bock, P Swicth, POW Block,
    - Trampoline, Vine, Arrow sign, Checkpoint Flag,
    - Lift, Lava lift (~ Moving)
    - Seesaw (bascule)
    - Grinder, Bumper, Spike Pillar, Swinging Claw
    - Track, Conveyor Belt, One-way Wall
    - Fire Bar, Snake Block
    - ON/OFF Switch, Dotted-Line Block
    [3D World]
    - Wrap Box, Blinking Block (alternent bleu-rouge disco)
    - Dash Block (fait courir)
    - Cloud lift (comme lift + one-way)
    - Mushroom trampoline, ON/OFF Trampoline
    - Track Block (Moving ?), Spike block
*/

#endif
