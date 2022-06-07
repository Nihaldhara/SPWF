#ifndef _RE_SETTINGS_H_
#define _RE_SETTINGS_H_

#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#include "../OOC/OOC.h"

#ifdef _WIN32
#  define RE_INLINE inline
#else
#  define RE_INLINE static inline
#endif

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//.................................................................................................
// Debug et informations

#define RE_AssertNew(ptr) { if (ptr == NULL) { assert(false); abort(); }}

RE_INLINE char *Strdup(const char *src)
{
    #ifdef _WIN32
    return _strdup(src);
    #else
    return strdup(src);
    #endif
}

RE_INLINE void RE_Memcpy(
    void *destination, long long destinationSize,
    const void *source, long long sourceSize)
{
#ifdef _WIN32
    memcpy_s(destination, destinationSize, source, sourceSize);
#else
    memcpy(destination, source, sourceSize);
#endif
}

int RE_Init(int sdlFlags, int imgFlags);
void RE_Quit();

#endif
