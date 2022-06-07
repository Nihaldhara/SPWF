#ifndef _RE_ANIMATION_H_
#define _RE_ANIMATION_H_

#include "RE_Settings.h"
#include "RE_Utils.h"

typedef struct RE_Animator_s RE_Animator;

void Class_InitAnimation();

/// @brief Description de la classe "RE_Animation".
extern const void *const Class_Animation;

typedef struct AnimationClass_s {
    const Class base;

    // Messages
    void (*Update)(void *self, float dt);
} AnimationClass;

/// @ingroup RE_Animator
/// @brief Type d'une courbe d'accélération.
/// Ces fonctions sont définies sur [0,1] et vérifient généralement f(0) = 0 et f(1) = 1.
typedef float (*RE_EasingFct)(float);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération linéaire.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
float RE_EasingFct_Linear(float x);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération douce en entrée.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
float RE_EasingFct_In(float x);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération douce en sortie.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
float RE_EasingFct_Out(float x);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération avec douce en entrée et en sortie.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
float RE_EasingFct_InOut(float x);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération définie à partir du cosinus.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
float RE_EasingFct_Cos(float x);

/// @ingroup RE_Animator
/// @brief Enumération contenant les flags pouvant être utilisés
/// pour contrôler l'animation d'une texture ou d'un paramètre.
typedef enum RE_AnimFlags_e
{
    /// @brief Met l'animation en pause.
    RE_ANIM_PAUSED = 1 << 1,
    /// @brief Joue l'animation dans le sens inverse.
    RE_ANIM_REVERSED = 1 << 2,
    /// @brief Joue l'animation en avant puis en arrière.
    RE_ANIM_ALTERNATE = 1 << 3,
    /// @brief Arrête automatiquement l'animation à la fin.
    RE_ANIM_STOP_AT_END = 1 << 4,
    RE_ANIM_STOPPED = 1 << 5
} RE_AnimFlags;

typedef struct RE_Animation_s
{
    struct {
        Object base;
    } m_super;

    char *m_name;

    RE_Animator *m_animator;

    /// @brief Courbe d'accélération de l'animation.
    RE_EasingFct m_easing;

    /// @brief Phase de l'animation.
    float m_phase;

    /// @brief Le coefficient multiplicateur de la vitesse de lecture.
    float m_speed;

    /// @brief La durée d'un cycle.
    float m_cycleTime;

    /// @brief Délai avant le début de l'animation.
    float m_delay;

    /// @brief Accumulateur.
    float m_accu;

    /// @brief Accumulateur pour le délai.
    float m_delayAccu;

    /// @brief Les flags de l'animation.
    /// Les valeurs possibles des flags sont données dans l'énumération RE_AnimFlags.
    int m_flags;

    /// @brief Nombre de cycles de l'animation.
    /// L'animation tourne en boucle si ce membre vaut -1.
    int m_cycleCount;

    /// @brief Indice du cycle courant. 
    int m_cycleIdx;
} RE_Animation;

void RE_Animation_Constructor(void *self, RE_Animator *animator, char *name);

/// @brief Définit la durée d'un cycle de l'animation d'une texture.
/// @param[in,out] self l'animation.
/// @param[in] cycleTime la durée d'un cycle.
RE_INLINE void RE_Animation_SetCycleTime(void *self, float cycleTime)
{
    RE_Animation *anim = (RE_Animation *)Object_Cast(self, Class_Animation);
    anim->m_cycleTime = cycleTime;
}

/// @brief Définit le nombre de cycle de l'animation d'une texture.
/// L'animation se répète indéfiniment si le nombre de cycle vaut -1.
/// @param[in,out] self l'animation.
/// @param[in] cycleCount le nombre de cycle de l'animation.
RE_INLINE void RE_Animation_SetCycleCount(void *self, int cycleCount)
{
    RE_Animation *anim = (RE_Animation *)Object_Cast(self, Class_Animation);
    anim->m_cycleCount = cycleCount;
    anim->m_cycleIdx = 0;
    if (cycleCount == 0)
    {
        anim->m_flags |= RE_ANIM_PAUSED;
    }
}

/// @ingroup PRE_Animator
/// @brief Définit le délai avant le début de l'animation d'une texture.
/// @param[in,out] self l'animation.
/// @param[in] delay le délai de l'animation.
RE_INLINE void RE_Animation_SetDelay(void *self, float delay)
{
    RE_Animation *anim = (RE_Animation *)Object_Cast(self, Class_Animation);
    anim->m_delay = delay;
}

/// @ingroup PRE_Animator
/// @brief Définit la vitesse de lecture d'une texture animée. La valeur par défaut est 1.f.
/// Si elle vaut par exemple 2.f, l'animation se déroulera deux fois plus vite.
/// @param[in,out] self l'animation.
/// @param[in] speed coefficient multiplicateur de la vitesse de lecture.
RE_INLINE void RE_Animation_SetSpeed(void *self, float speed)
{
    RE_Animation *anim = (RE_Animation *)Object_Cast(self, Class_Animation);
    anim->m_speed = speed;
}

RE_INLINE void RE_Animation_SetPhase(void *self, float phase)
{
    RE_Animation *anim = (RE_Animation *)Object_Cast(self, Class_Animation);
    anim->m_phase = phase;
}

RE_INLINE void RE_Animation_SetEasing(void *self, RE_EasingFct easing)
{
    RE_Animation *anim = (RE_Animation *)Object_Cast(self, Class_Animation);
    anim->m_easing = easing;
}

/// @ingroup PRE_Animator
/// @brief Ajoute des flags à une texture animée.
/// Les valeurs possibles des flags sont données dans l'énumération RE_AnimFlags.
/// @param[in,out] self l'animation.
/// @param[in] flags les flags à ajouter.
RE_INLINE void RE_Animation_AddFlags(void *self, int flags)
{
    RE_Animation *anim = (RE_Animation *)Object_Cast(self, Class_Animation);
    anim->m_flags |= flags;
}

/// @ingroup PRE_Animator
/// @brief Enlève des flags à une texture animée.
/// Les valeurs possibles des flags sont données dans l'énumération RE_AnimFlags.
/// @param[in,out] self l'animation.
/// @param[in] flags les flags à enlever.
RE_INLINE void RE_Animation_RemoveFlags(void *self, int flags)
{
    RE_Animation *anim = (RE_Animation *)Object_Cast(self, Class_Animation);
    anim->m_flags &= ~flags;
}

/// @brief Progression avant courbe d'accélération et flags
/// @param self 
/// @return 
float RE_Animation_GetRawProgression(void *self);
float RE_Animation_GetProgression(void *self);

RE_INLINE RE_Animator *RE_Animation_GetAnimator(void *self)
{
    RE_Animation *anim = (RE_Animation *)Object_Cast(self, Class_Animation);
    return anim->m_animator;
}

void RE_Animation_OnPlay(void *self);
void RE_Animation_OnResume(void *self);
void RE_Animation_OnStop(void *self);

RE_INLINE void RE_Animation_Update(void *self, float dt)
{
    assert(Object_IsA(self, Class_Animation));
    const AnimationClass *clsDesc = (AnimationClass *)Object_GetClass(self);
    (clsDesc->Update)(self, dt);
}

RE_INLINE bool RE_Animation_IsStopped(void *self)
{
    RE_Animation *anim = (RE_Animation *)Object_Cast(self, Class_Animation);
    return (anim->m_flags & RE_ANIM_STOPPED) == RE_ANIM_STOPPED;
}
RE_INLINE char *RE_Animation_GetName(void *self)
{
    RE_Animation *anim = (RE_Animation *)Object_Cast(self, Class_Animation);
    return anim->m_name;
}

#endif