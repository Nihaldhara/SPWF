#ifndef _PE_API_H_
#define _PE_API_H_

#include <string.h>

#ifdef _WIN32
#  define PE_INLINE inline
#else
#  define PE_INLINE static inline
#endif

//.................................................................................................
// Fonctions différentes window/linux

PE_INLINE void PE_Memcpy(
    void *destination, long long destinationSize,
    void *source, long long sourceSize)
{
#ifdef _WIN32
    memcpy_s(destination, destinationSize, source, sourceSize);
#else
    memcpy(destination, source, sourceSize);
#endif
}

#endif