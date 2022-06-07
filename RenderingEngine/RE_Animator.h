#ifndef _RE_ANIMATOR_H_
#define _RE_ANIMATOR_H_

#include "RE_Settings.h"
#include "RE_Timer.h"
#include "RE_Renderer.h"
#include "RE_Utils.h"
#include "RE_HashTable.h"
#include "RE_Atlas.h"

#include "RE_Animation.h"
#include "RE_AlphaAnim.h"
#include "RE_AngleAnim.h"
#include "RE_BezierAnim.h"
#include "RE_ScaleAnim.h"
#include "RE_ShiftAnim.h"
#include "RE_TexAnim.h"

typedef struct RE_Animation_s RE_Animation;

typedef enum RE_UpdateMode_e
{
    RE_UPDATE_SCALED_TIME,
    RE_UPDATE_UNSCALED_TIME
} RE_UpdateMode;

/// @brief Structure représentant un animateur.
/// Cela permet d'animer un ensemble de plusieurs atlas de textures
/// et d'y ajouter des effets comme des variations de position ou d'opacité.
typedef struct RE_Animator_s
{
    /// @protected
    /// @brief Table de hachage contenant les textures animées.
    RE_HashTable *m_animHT;

    RE_AtlasPart *m_atlasPart;

    /// @protected
    /// @brief Indice de la partie à rendre dans l'atlas de texture actif.
    int m_frameIdx;
    Vec2 m_center;
    Vec2 m_shift;
    Vec2 m_scale;
    float m_angle;
    float m_alpha;

    bool m_unscaledTime;
} RE_Animator;

/// @brief Crée un nouvel animateur.
/// @return L'animateur créé ou NULL en cas d'erreur.
RE_Animator *RE_Animator_New();

/// @brief Détruit un animateur préalablement créé avec RE_Animator_New().
/// Détruit également toutes les animations contenues dans l'animateur.
/// Ne libère pas les textures associées.
/// @param[in,out] animator l'animateur à détruire.
void RE_Animator_Delete(RE_Animator *animator);

RE_AlphaAnim *RE_Animator_CreateAlphaAnim(
    RE_Animator *animator, char *name, float alpha0, float alpha1
);
RE_AngleAnim *RE_Animator_CreateAngleAnim(
    RE_Animator *animator, char *name, float angle0, float angle1
);
RE_BezierAnim *RE_Animator_CreateBezierAnim(
    RE_Animator *animator, char *name, Vec2 points[4]
);
RE_ScaleAnim *RE_Animator_CreateScaleAnim(
    RE_Animator *animator, char *name, Vec2 scale0, Vec2 scale1
);
RE_ShiftAnim *RE_Animator_CreateShiftAnim(
    RE_Animator *animator, char *name, Vec2 shift0, Vec2 shift1
);
RE_TexAnim *RE_Animator_CreateTextureAnim(
    RE_Animator *animator, char *name, RE_AtlasPart *atlasPart
);

/// @brief Renvoie un pointeur sur une texture animée d'un animateur à partir de son nom.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return Un pointeur vers la texture animée
/// ou NULL s'il n'y a aucune animation correspondant au nom donné.
RE_INLINE RE_Animation *RE_Animator_GetAnimation(RE_Animator *animator, char *name)
{
    return (RE_Animation *)RE_HashTable_Get(animator->m_animHT, name);
}

/// @brief Détruit une texture animée d'un animateur.
/// Cette fonction ne libère pas la texture associée.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return EXIT_SUCCESS ou EXIT_FAILURE
/// s'il n'y a aucune animation correspondant au nom donné.
int RE_Animator_DeleteAnimation(RE_Animator *animator, char *name);

/// @brief Lance l'animation d'une texture d'un animateur.
/// Si l'animateur joue déjà une animation, elle est remplacée par la nouvelle.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return EXIT_SUCCESS ou EXIT_FAILURE
/// s'il n'y a aucune animation correspondant au nom donné.
int RE_Animator_PlayAnimation(RE_Animator *animator, char *name);
int RE_Animator_StopAnimation(RE_Animator *animator, char *name);
int RE_Animator_ResumeAnimation(RE_Animator *animator, char *name);

void RE_Animator_StopAnimations(RE_Animator *animator);

/// @brief Met à jour l'animateur en fonction du temps écoulé.
/// @param[in,out] animator l'animateur.
/// @param[in] timer le timer principal de la boucle de rendu.
void RE_Animator_Update(RE_Animator *animator, RE_Timer *timer);

RE_INLINE void RE_Animator_SetUpdateMode(RE_Animator *animator, int mode)
{
    animator->m_unscaledTime = (mode == RE_UPDATE_UNSCALED_TIME);
}

int RE_Animator_RenderCopyExF(
    RE_Animator *animator, SDL_Renderer *renderer, const SDL_FRect *dstRect, int anchor,
    const double angle, Vec2 center, int flip
);

RE_INLINE int RE_Animator_RenderCopyF(
    RE_Animator *animator, SDL_Renderer *renderer, const SDL_FRect *dstRect, int anchor)
{
    assert(animator);
    return RE_Animator_RenderCopyExF(
        animator, renderer, dstRect, anchor,
        0.0f, animator->m_center, 0
    );
}

RE_INLINE void RE_Animator_SetAtlasPart(RE_Animator *animator, RE_AtlasPart *atlasPart)
{
    animator->m_atlasPart = atlasPart;
}
RE_INLINE void RE_Animator_SetFrame(RE_Animator *animator, int frameIdx)
{
    animator->m_frameIdx = frameIdx;
}
RE_INLINE void RE_Animator_AddShift(RE_Animator *animator, Vec2 shift)
{
    animator->m_shift.x += shift.x;
    animator->m_shift.y += shift.y;
}
RE_INLINE void RE_Animator_AddAngle(RE_Animator *animator, float angle)
{
    animator->m_angle += angle;
}
RE_INLINE void RE_Animator_MultScale(RE_Animator *animator, Vec2 scale)
{
    animator->m_scale.x *= scale.x;
    animator->m_scale.y *= scale.y;
}
RE_INLINE void RE_Animator_MultAlpha(RE_Animator *animator, float alpha)
{
    animator->m_alpha *= alpha;
}

#endif