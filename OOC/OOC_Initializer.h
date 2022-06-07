#ifndef _OOC_INITIALIZER_H_
#define _OOC_INITIALIZER_H_

#include "OOC_Object.h"

typedef void (*InitFct)();
extern InitFct OOC_Initializers[];

void OOC_Init();

#endif
