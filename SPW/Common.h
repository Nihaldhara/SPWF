#ifndef _COMMON_H_
#define _COMMON_H_

#include "Settings.h"

extern RE_Timer *g_time;

#define LEVEL_COUNT 2

typedef enum GameTheme_e
{
    THEME_MOUNTAINS,
    THEME_LAKE
} GameTheme;

typedef struct LevelProgress_s
{
    bool sucessful;
    // float bestTime;
} LevelProgress;

typedef struct Progress_s
{
    int lifeCount;
    int fireflyCount;
    LevelProgress levels[LEVEL_COUNT];
} Progress;

typedef struct LevelData_s
{
    char *title;
    char *path;
    int theme;
} LevelData;

extern Progress  g_progress;
extern LevelData g_levelData[LEVEL_COUNT];

void Progress_Init();
void LevelData_Init();

#endif