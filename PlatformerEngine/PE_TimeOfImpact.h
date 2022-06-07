
#ifndef _PE_TIME_OF_IMPACT_H_
#define _PE_TIME_OF_IMPACT_H_

#include "PE_Math.h"
#include "PE_Shape.h"
#include "PE_ShapeDistance.h"

typedef struct PE_TOIInput_s
{
    PE_Shape *shapeA;
    PE_Shape *shapeB;
    PE_Sweep sweepA;
    PE_Sweep sweepB;
} PE_TOIInput;

typedef enum PE_TOIState_e {
    PE_TOI_FAILED,
    PE_TOI_WILL_TOUCH,
    PE_TOI_OVERLAPPING,
    PE_TOI_SEPARATED
} PE_TOIState;

typedef struct PE_TOIOutput_s
{
    PE_TOIState state;
    float beta;
    int iterCount;
} PE_TOIOutput;

void PE_TimeOfImpact(PE_TOIInput *input, PE_SimplexCache *cache, PE_TOIOutput *output);

#endif
