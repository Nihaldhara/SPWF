
/// @file RE_Timer.h
/// @defgroup RE_Timer Fonctions relatives à la gestion du temps

#ifndef _RE_TIMER_H_
#define _RE_TIMER_H_

#include "RE_Settings.h"

/// @ingroup RE_Timer
/// @brief Structure représentant un chronomètre.
typedef struct RE_Timer_s
{
    /// @protected
    /// @brief Temps de départ.
    float m_startTime;

    /// @protected
    /// @brief Temps du dernier appel à RE_Timer_Update().
    float m_currentTime;

    /// @protected
    /// @brief Temps de l'avant dernier appel à RE_Timer_Update().
    float m_previousTime;

    /// @protected
    /// @brief Ecart entre les deux derniers appels à RE_Timer_Update().
    float m_delta;
    float m_unscaledDelta;

    float m_scale;

    float m_maxDelta;

    float m_elapsed;
    float m_unscaledElapsed;
} RE_Timer;

/// @ingroup RE_Timer
/// @brief Crée un nouveau timer.
/// @return Le timer créé ou NULL en cas d'erreur.
RE_Timer* RE_Timer_New();

/// @ingroup RE_Timer
/// @brief Détruit un timer préalablement alloué avec RE_Timer_New().
/// @param[in,out] timer le timer à détruire.
void RE_Timer_Delete(RE_Timer* timer);

/// @ingroup RE_Timer
/// @brief Lance le timer.
/// @param[in,out] timer le timer.
void RE_Timer_Start(RE_Timer* timer);

/// @ingroup RE_Timer
/// @brief Met à jour le timer en fonction du temps actuel.
/// Cette fonction est typiquement appelée à chaque tour de la boucle de rendu.
/// @param[in,out] timer le timer.
void RE_Timer_Update(RE_Timer* timer);

RE_INLINE void RE_Timer_SetMaximumDeltaTime(RE_Timer *timer, float maxDelta)
{
    timer->m_maxDelta = maxDelta;
}

RE_INLINE void RE_Timer_SetTimeScale(RE_Timer *timer, float scale)
{
    assert(scale >= 0.0f);
    timer->m_scale = scale;
}

RE_INLINE float RE_Timer_GetTimeScale(RE_Timer *timer)
{
    return timer->m_scale;
}

/// @ingroup RE_Timer
/// @brief Renvoie l'écart de temps (en secondes) entre les deux derniers appels à la fonction
/// RE_Timer_Update().
/// @param[in] timer le timer.
/// @return L'écart de temps entre les deux dernières mises à jour.
RE_INLINE float RE_Timer_GetDelta(RE_Timer *timer)
{
    return timer->m_delta;
}

RE_INLINE float RE_Timer_GetUnscaledDelta(RE_Timer *timer)
{
    return timer->m_unscaledDelta;
}

/// @ingroup RE_Timer
/// @brief Renvoie l'écart de temps (en secondes) entre le lancement du timer avec la fonction
/// RE_Timer_Start() et le dernier appel à la fonction RE_Timer_Update().
/// @param[in] timer le timer.
/// @return Le nombre de secondes écoulées depuis le lancement du timer et la dernière mise à jour.
RE_INLINE float RE_Timer_GetElapsed(RE_Timer *timer)
{
    return timer->m_elapsed;
}

RE_INLINE float RE_Timer_GetUnscaledElapsed(RE_Timer *timer)
{
    return timer->m_unscaledElapsed;
}

#endif