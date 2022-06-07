#ifndef _ASSET_MANAGER_H_
#define _ASSET_MANAGER_H_

#include "Settings.h"
#include "Common.h"

typedef struct GameColors_s
{
    SDL_Color m_normal;
    SDL_Color m_black;
    SDL_Color m_white;
} GameColors;

typedef struct GameAtlases_s
{
    RE_Atlas *m_collectable;
    RE_Atlas *m_enemy;
    RE_Atlas *m_logo;
    RE_Atlas *m_player;
    RE_Atlas *m_terrain;
    RE_Atlas *m_ui;
} GameAtlases;

typedef struct GameFonts_s
{
    TTF_Font *m_large;
    TTF_Font *m_normal;
} GameFonts;

#define LAYER_COUNT 5

typedef struct AssetManager_s
{
    SDL_Renderer *m_renderer;
    int m_theme;
    char *m_themeFolder;

    GameFonts m_fonts;
    GameColors m_colors;
    GameAtlases m_atlases;

    SDL_Texture *m_layers[LAYER_COUNT];
} AssetManager;

AssetManager *AssetManager_New(SDL_Renderer *renderer, int theme);
void AssetManager_Delete(AssetManager *textures);

RE_Atlas *AssetManager_GetCollectableAtlas(AssetManager *assets);
RE_Atlas *AssetManager_GetEnemyAtlas(AssetManager *assets);
RE_Atlas *AssetManager_GetLogoAtlas(AssetManager *assets);
RE_Atlas *AssetManager_GetPlayerAtlas(AssetManager *assets);
RE_Atlas *AssetManager_GetTerrainAtlas(AssetManager *assets);
RE_Atlas *AssetManager_GetUIAtlas(AssetManager *assets);

SDL_Texture **AssetManager_GetLayersTextures(AssetManager *assets);

TTF_Font *AssetManager_GetLargeFont(AssetManager *assets);
TTF_Font *AssetManager_GetNormalFont(AssetManager *assets);

INLINE SDL_Color AssetManager_GetNormalColor(AssetManager *assets)
{
    return assets->m_colors.m_normal;
}

INLINE SDL_Color AssetManager_GetBlack(AssetManager *assets)
{
    return assets->m_colors.m_black;
}

INLINE SDL_Color AssetManager_GetWhite(AssetManager *assets)
{
    return assets->m_colors.m_white;
}

#endif
