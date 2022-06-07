
#ifndef _PE_SOLVER_H_
#define _PE_SOLVER_H_

#include "PE_Settings.h"
#include "PE_Allocator.h"
#include "PE_List.h"
#include "PE_Vector.h"
#include "PE_Math.h"

typedef struct PE_Island_s PE_Island;
typedef struct PE_World_s PE_World;
typedef struct PE_Body_s PE_Body;
typedef struct PE_CollisionPair_s PE_CollisionPair;
typedef struct PE_CollisionManager_s PE_CollisionManager;

///////////////////////////////////////////////////////////////////////////////////////////////////
//  PE_SolverBody

typedef struct PE_BodySolverData_s
{
    /// @brief Number of times the solver has moved the body during the current fixed update.
    int m_iterCount;
} PE_BodySolverData;

void PE_Solver_InitBody(PE_Body *body);

///////////////////////////////////////////////////////////////////////////////////////////////////
//  PE_CPairSolverData

typedef enum PE_SolverFlag_e
{
    PE_SOLVER_A_UP           = 1 << 0,
    PE_SOLVER_B_UP           = 1 << 1,
    PE_SOLVER_FRICTION       = 1 << 2,
    PE_SOLVER_FORCES_APPLIED = 1 << 3,
} PE_SolverFlag;

/// @brief A SolverCollisionPair contains the solver data associated with a collision pair.
/// Each collision pair contains this object.
typedef struct PE_CPairSolverData_s
{
    int m_flags;

    /// @brief Nombre d'itérations du solver sur cette pair pendant la mise à jour actuelle.
    int m_iterCount;
} PE_CPairSolverData;

PE_INLINE int PE_CPairSolverData_TestFlags(PE_CPairSolverData *cpData, int flags)
{
    return cpData->m_flags & flags;
}

PE_INLINE void PE_CPairSolverData_AddFlags(PE_CPairSolverData *cpData, int flags)
{
    cpData->m_flags |= flags;
}

PE_INLINE void PE_CPairSolverData_RemoveFlags(PE_CPairSolverData *cpData, int flags)
{
    cpData->m_flags &= ~flags;
}

void PE_Solver_InitCollisionPair(PE_CollisionPair *cPair);
void PE_Solver_SetCollisionAUp(PE_CollisionPair *cPair);
void PE_Solver_SetCollisionBUp(PE_CollisionPair *cPair);
void PE_Solver_SetFriction(PE_CollisionPair *cPair, bool hasFriction);

///////////////////////////////////////////////////////////////////////////////////////////////////
//  PE_Solver

/// @brief The solver is used to solved the collisions computed by the collision manager.
typedef struct PE_Solver_s
{
    /// @protected
    /// @brief Pointer to the world allocator.
    PE_Allocator *m_allocator;

    /// @brief Pour la liste des PE_Body
    PE_World *m_world;

    /// @brief Pour la liste de collisions;
    PE_CollisionManager *m_manager;
} PE_Solver;

/// @brief Contructs the slover of the physics engine.
/// @param world the world of the physics engine.
/// @return A pointer to the new solver.
PE_Solver *PE_Solver_New(PE_World *world);

/// @brief Destructs the solver.
/// @param solver the solver to destruct.
void PE_Solver_Free(PE_Solver *solver);


void PE_Solver_Solve(PE_Solver *solver);

/// @brief Solves the collisions computed by the collision manager.
/// @param solver the solver of the physics engine.
void PE_Solver_SolveTOI(PE_Solver *solver);

/// @brief Resets the solver data associated with each body and each collision pair in the world.
/// @param solver the solver of the physics engine.
void PE_Solver_Reset(PE_Solver *solver);

#endif