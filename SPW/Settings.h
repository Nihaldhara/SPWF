#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define _CRT_SECURE_NO_WARNINGS

#include "../OOC/OOC.h"
#include "../PlatformerEngine/PE.h"
#include "../PlatformerEngine/PE_Tools.h"
#include "../RenderingEngine/RE.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#ifdef _WIN32
#  define INLINE inline
#else
#  define INLINE static inline
#endif

#define AssertNew(ptr) { if (ptr == NULL) { assert(false); abort(); } }

extern RE_Timer *g_time;

void SPW_Init();
void SPW_Quit();

#define HD_WIDTH 1600//1280
#define HD_HEIGHT 900//720

#define FHD_WIDTH 1920
#define FHD_HEIGHT 1080

#ifdef FHD
#define WINDOW_WIDTH   FHD_WIDTH
#define WINDOW_HEIGHT  FHD_HEIGHT
#else
#define WINDOW_WIDTH   HD_WIDTH
#define WINDOW_HEIGHT  HD_HEIGHT
#endif

#define LOGICAL_WIDTH  FHD_WIDTH
#define LOGICAL_HEIGHT FHD_HEIGHT

#endif
