#include "OOC_Initializer.h"

InitFct OOC_Initializers[] = {
    Class_InitObject, Class_InitClass,
};

void OOC_Init()
{
    int initCount = sizeof(OOC_Initializers) / sizeof(InitFct);
    for (int i = 0; i < initCount; ++i)
    {
        (OOC_Initializers[i])();
    }
}
