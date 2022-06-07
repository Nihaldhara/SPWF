
#ifndef _PE_SHAPE_DISTANCE_H_
#define _PE_SHAPE_DISTANCE_H_

#include "PE_Shape.h"

#define PE_GJK_ITERS 20

/// A distance proxy is used by the GJK algorithm.
/// It encapsulates any shape.
typedef struct PE_DistanceProxy_s
{
    float radius;
    int count;
    PE_Vec2 vertices[PE_MAX_POLYGON_VERTICES];
    // pas de m_buffer (chainShape)
} PE_DistanceProxy;

void PE_DistanceProxy_Set(PE_DistanceProxy *proxy, PE_Shape *shape);

/// Used to warm start b2Distance.
/// Set count to zero on first call.
typedef struct PE_SimplexCache_s
{
    float metric; ///< length or area
    int count;
    int indicesA[3];///< vertices on shape A   -> indexA
    int indicesB[3];///< vertices on shape B   -> indexB
    //float div; // NOUVEAU c2
} PE_SimplexCache;

/// Output for b2Distance.
typedef struct PE_DistanceOutput_s
{
    PE_Vec2 pointA;        ///< closest point on shapeA
    PE_Vec2 pointB;        ///< closest point on shapeB
    int iterCount;    ///< number of GJK iterCount used
} PE_DistanceOutput;

float PE_GJK(
    PE_DistanceProxy *proxyA, PE_Vec2 transformA,
    PE_DistanceProxy *proxyB, PE_Vec2 transformB,
    int useRadius, PE_SimplexCache *cache, PE_DistanceOutput *output); // cache et out peuvent être NULL

PE_INLINE float PE_Shape_GetDistance(
    PE_Shape *shapeA, PE_Vec2 transformA,
    PE_Shape *shapeB, PE_Vec2 transformB,
    PE_DistanceOutput *output)
{
    PE_DistanceProxy proxyA, proxyB;
    PE_DistanceProxy_Set(&proxyA, shapeA);
    PE_DistanceProxy_Set(&proxyB, shapeB);

    return PE_GJK(&proxyA, transformA, &proxyB, transformB, true, NULL, output);
}

#endif
