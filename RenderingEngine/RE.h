#ifndef _RENDERING_ENGINE_H_
#define _RENDERING_ENGINE_H_

#ifdef _WIN32
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#include "RE_HashTable.h"
#include "RE_Renderer.h"
#include "RE_Timer.h"
#include "RE_Utils.h"
#include "RE_Text.h"

#include "RE_Animator.h"

#include "RE_Animation.h"
#include "RE_TexAnim.h"
#include "RE_ShiftAnim.h"
#include "RE_AlphaAnim.h"
#include "RE_AngleAnim.h"
#include "RE_ScaleAnim.h"
#include "RE_BezierAnim.h"

#endif