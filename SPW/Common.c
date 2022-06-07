#include "Common.h"

RE_Timer *g_time = NULL;
Progress g_progress = { 0 };
LevelData g_levelData[LEVEL_COUNT] = { 0 };

void Progress_Init()
{
    g_progress.fireflyCount = 0;
    g_progress.lifeCount = 5;

    for (int i = 0; i < LEVEL_COUNT; i++)
    {
        g_progress.levels[i].sucessful = false;
    }
}

void LevelData_Init()
{
    int i = 0;
    g_levelData[i].title = u8"Montagnes";
    g_levelData[i].theme = THEME_MOUNTAINS;
    g_levelData[i].path = "../Assets/Level/Level01.txt";
    i++;

    g_levelData[i].title = u8"Démo";
    g_levelData[i].theme = THEME_LAKE;
    g_levelData[i].path = "../Assets/Level/LevelDemo.txt";
    i++;
}
