
#ifndef _PE_SHAPE_RAY_CAST_H_
#define _PE_SHAPE_RAY_CAST_H_

#include "PE_Shape.h"

/// Ray-cast input data. The ray extends from p1 to p1 + maxFraction * (p2 - p1).
typedef struct PE_RayCastInput_s
{
	PE_Vec2 p1, p2;
	float maxFraction;
} PE_RayCastInput;

/// Ray-cast output data. The ray hits at p1 + fraction * (p2 - p1), where p1 and p2
/// come from b2RayCastInput.
typedef struct PE_RayCastOutput_s
{
	PE_Vec2 normal;
	float fraction;
} PE_RayCastOutput;

bool PE_Shape_RayCast(
	PE_Shape *shape, PE_Vec2 transform,
	PE_RayCastInput *input, PE_RayCastOutput *output);

#endif