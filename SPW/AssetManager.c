#include "AssetManager.h"

AssetManager *AssetManager_New(SDL_Renderer *renderer, int theme)
{
    AssetManager *assets = calloc(1, sizeof(AssetManager));
    AssertNew(assets);

    assets->m_renderer = renderer;
    assets->m_theme = theme;

    switch (theme)
    {
    case THEME_LAKE:
        assets->m_themeFolder = "../Assets/Theme_Lake";
        break;

    case THEME_MOUNTAINS:
    default:
        assets->m_themeFolder = "../Assets/Theme_Mountains";
        break;
    }

    // Couleurs
    GameColors *colors = &assets->m_colors;

    SDL_Color normalColor = { .r = 252, .g = 242, .b = 225, .a = 255 };
    SDL_Color blackColor = { .r = 0, .g = 0, .b = 0, .a = 255 };
    SDL_Color whiteColor = { .r = 255, .g = 255, .b = 255, .a = 255 };

    colors->m_normal = normalColor;
    colors->m_black = blackColor;
    colors->m_white = whiteColor;

    return assets;
}

void AssetManager_Delete(AssetManager *assets)
{
    if (!assets)
        return;

    GameAtlases *atlases = &assets->m_atlases;

    RE_Atlas_Delete(atlases->m_collectable);
    RE_Atlas_Delete(atlases->m_enemy);
    RE_Atlas_Delete(atlases->m_logo);
    RE_Atlas_Delete(atlases->m_player);
    RE_Atlas_Delete(atlases->m_terrain);
    RE_Atlas_Delete(atlases->m_ui);

    for (int i = 0; i < LAYER_COUNT; ++i)
    {
        if (assets->m_layers[i])
        {
            SDL_DestroyTexture(assets->m_layers[i]);
        }
    }

    GameFonts *fonts = &assets->m_fonts;

    if (fonts->m_large)
    {
        TTF_CloseFont(fonts->m_large);
    }
    if (fonts->m_normal)
    {
        TTF_CloseFont(fonts->m_normal);
    }

    free(assets);
}

RE_Atlas *AssetManager_GetPlayerAtlas(AssetManager *assets)
{
    GameAtlases *atlases = &assets->m_atlases;
    if (!atlases->m_player)
    {
        atlases->m_player = RE_Atlas_New(
            assets->m_renderer,
            "../Assets/Atlas/Player.json"
        );
        AssertNew(atlases->m_player);
    }
    return atlases->m_player;
}

RE_Atlas *AssetManager_GetEnemyAtlas(AssetManager *assets)
{
    GameAtlases *atlases = &assets->m_atlases;
    if (!atlases->m_enemy)
    {
        atlases->m_enemy = RE_Atlas_New(
            assets->m_renderer,
            "../Assets/Atlas/Enemy.json"
        );
        AssertNew(atlases->m_enemy);
    }
    return atlases->m_enemy;
}

RE_Atlas *AssetManager_GetLogoAtlas(AssetManager *assets)
{
    GameAtlases *atlases = &assets->m_atlases;
    if (!atlases->m_logo)
    {
        atlases->m_logo = RE_Atlas_New(
            assets->m_renderer,
            "../Assets/Atlas/Logo.json"
        );
        AssertNew(atlases->m_logo);
    }
    return atlases->m_logo;
}

RE_Atlas *AssetManager_GetTerrainAtlas(AssetManager *assets)
{
    GameAtlases *atlases = &assets->m_atlases;
    if (!atlases->m_terrain)
    {
        atlases->m_terrain = RE_Atlas_New(
            assets->m_renderer,
            "../Assets/Atlas/Terrain.json"
        );
        AssertNew(atlases->m_terrain);
    }
    return atlases->m_terrain;
}

RE_Atlas *AssetManager_GetUIAtlas(AssetManager *assets)
{
    GameAtlases *atlases = &assets->m_atlases;
    if (!atlases->m_ui)
    {
        atlases->m_ui = RE_Atlas_New(
            assets->m_renderer,
            "../Assets/Atlas/UI.json"
        );
        AssertNew(atlases->m_ui);
    }
    return atlases->m_ui;
}

RE_Atlas *AssetManager_GetCollectableAtlas(AssetManager *assets)
{
    GameAtlases *atlases = &assets->m_atlases;
    if (!atlases->m_collectable)
    {
        atlases->m_collectable = RE_Atlas_New(
            assets->m_renderer,
            "../Assets/Atlas/Collectable.json"
        );
        AssertNew(atlases->m_collectable);
    }
    return atlases->m_collectable;
}

SDL_Texture **AssetManager_GetLayersTextures(AssetManager *assets)
{
    GameAtlases *atlases = &assets->m_atlases;
    char path[1024] = { 0 };

    for (int i = 0; i < LAYER_COUNT; ++i)
    {
        if (!assets->m_layers[i])
        {
            sprintf(path, "%s/Layer_%02d.png", assets->m_themeFolder, i);
            assets->m_layers[i] = IMG_LoadTexture(assets->m_renderer, path);
            if (!assets->m_layers[i])
            {
                printf("ERROR - IMG_LoadTexture %s\n", SDL_GetError());
                assert(false); abort();
            }
        }
    }
    return assets->m_layers;
}

TTF_Font *AssetManager_GetLargeFont(AssetManager *assets)
{
    GameFonts *fonts = &assets->m_fonts;
    if (!fonts->m_large)
    {
        fonts->m_large = TTF_OpenFont(
            "../Assets/Font/Berlin_Sans_FB_Demi_Bold.ttf",
            64
        );
        AssertNew(fonts->m_large);
    }
    return fonts->m_large;
}

TTF_Font *AssetManager_GetNormalFont(AssetManager *assets)
{
    GameFonts *fonts = &assets->m_fonts;
    if (!fonts->m_normal)
    {
        fonts->m_normal = TTF_OpenFont(
            "../Assets/Font/Berlin_Sans_FB_Demi_Bold.ttf",
            28
        );
        AssertNew(fonts->m_normal);
    }
    return fonts->m_normal;
}