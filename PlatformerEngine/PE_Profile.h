
#ifndef _PE_PROFILE_H_
#define _PE_PROFILE_H_

#include <string.h>
#include <omp.h>
#include "PE_API.h"

typedef struct PE_Profile_s
{
    double m_step;
    double m_broadPhase;
    double m_narrowPhase;
    double m_timeOfImpact;
    double m_solver;
    double m_integration;

    double m_startStep;
    double m_startBP;
    double m_startNP;
    double m_startTOI;
    double m_startSolver;
    double m_startInt;

    int m_stepCount;
    int m_collisionCount;
    int m_islandCount;
    int m_iterationCount;
} PE_Profile;

extern PE_Profile g_profile;

PE_INLINE void PE_Profile_Reset(PE_Profile *profile)
{
    memset(profile, 0, sizeof(PE_Profile));
}

PE_INLINE void PE_Profile_StartStep(PE_Profile *profile)
{
    profile->m_startStep = omp_get_wtime();
}

PE_INLINE void PE_Profile_StartIntegration(PE_Profile *profile)
{
    profile->m_startInt = omp_get_wtime();
}

PE_INLINE void PE_Profile_StartBroadPhase(PE_Profile *profile)
{
    profile->m_startBP = omp_get_wtime();
}

PE_INLINE void PE_Profile_StartNarrowPhase(PE_Profile *profile)
{
    profile->m_startNP = omp_get_wtime();
}

PE_INLINE void PE_Profile_StartTOI(PE_Profile *profile)
{
    profile->m_startTOI = omp_get_wtime();
}

PE_INLINE void PE_Profile_StartSolver(PE_Profile *profile)
{
    profile->m_startSolver = omp_get_wtime();
}

PE_INLINE void PE_Profile_StopStep(PE_Profile *profile)
{
    profile->m_step += omp_get_wtime() - profile->m_startStep;
    profile->m_stepCount++;
}

PE_INLINE void PE_Profile_StopIntegration(PE_Profile *profile)
{
    profile->m_integration += omp_get_wtime() - profile->m_startInt;
}

PE_INLINE void PE_Profile_StopBroadPhase(PE_Profile *profile)
{
    profile->m_broadPhase += omp_get_wtime() - profile->m_startBP;
}

PE_INLINE void PE_Profile_StopNarrowPhase(PE_Profile *profile)
{
    profile->m_narrowPhase += omp_get_wtime() - profile->m_startNP;
}

PE_INLINE void PE_Profile_StopTOI(PE_Profile *profile)
{
    profile->m_timeOfImpact += omp_get_wtime() - profile->m_startTOI;
}

PE_INLINE void PE_Profile_StopSolver(PE_Profile *profile)
{
    profile->m_solver += omp_get_wtime() - profile->m_startSolver;
}

PE_INLINE void PE_Profile_AddCollisions(PE_Profile *profile, int collisionCount)
{
    profile->m_collisionCount += collisionCount;
}

PE_INLINE void PE_Profile_AddIslands(PE_Profile *profile, int islandCount)
{
    profile->m_islandCount += islandCount;
}

PE_INLINE void PE_Profile_AddSolverIterations(PE_Profile *profile, int iterationCount)
{
    profile->m_iterationCount += iterationCount;
}

void PE_Profile_Print(PE_Profile *profile);

#endif
