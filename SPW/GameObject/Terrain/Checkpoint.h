#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_

#include "../GameBody.h"

void Class_InitCheckpoint();

/// @brief Description de la classe "Checkpoint".
extern const void *const Class_Checkpoint;

typedef struct CheckpointClass_s {
    const GameBodyClass base;
} CheckpointClass;


typedef struct Checkpoint_s
{
    struct {
        GameBody base;
    } m_super;

    bool m_isActive;
    RE_Animator *m_animator;

} Checkpoint;

void Checkpoint_Constructor(void *self, void *scene, PE_Vec2 position);

#endif
