#ifndef _LEVEL_PARSER_H_
#define _LEVEL_PARSER_H_

#include "Settings.h"
#include "Common.h"

typedef struct Scene_s Scene;

typedef struct LevelParser_s
{
    Scene *m_scene;
    char **m_matrix;
    int m_width;
    int m_heigth;
} LevelParser;

LevelParser *LevelParser_New(char *path);
void LevelParser_Delete(LevelParser *parser);

void LevelParser_InitScene(LevelParser *parser, void *scene);

#endif
