#include "LevelScene.h"
#include "../Utils/Renderer.h"

#include "../LevelParser.h"
#include "../GameObject/Background.h"
#include "../GameObject/Camera/UserCamera.h"
#include "../GameObject/Camera/MainCamera.h"

// Object virtual methods
void LevelScene_VM_Destructor(void *self);

static LevelSceneClass _Class_LevelScene = { 0 };
const void *const Class_LevelScene = &_Class_LevelScene;

void Class_InitLevelScene()
{
    if (!Class_IsInitialized(Class_LevelScene))
    {
        Class_InitScene();

        void *self = (void *)Class_LevelScene;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Scene,
            .name = "LevelScene",
            .instanceSize = sizeof(LevelScene),
            .classSize = sizeof(LevelSceneClass)
        };
        Class_Constructor(params, LevelScene_VM_Destructor);
    }
}

void LevelScene_Constructor(void *self, SDL_Renderer *renderer, int levelIdx)
{
    int theme = g_levelData[levelIdx].theme;
    char *path = g_levelData[levelIdx].path;

    Scene_Constructor(self, renderer, theme);
    Object_SetClass(self, Class_LevelScene);

    LevelScene *scene = Object_Cast(self, Class_LevelScene);
    scene->m_levelIdx = levelIdx;

    LevelManager *manager = Scene_AllocateObject(scene, Class_LevelManager);
    AssertNew(manager);
    LevelManager_Constructor(manager, scene);
    scene->m_manager = manager;

    // Crée les caméras
    MainCamera *mainCamera = (MainCamera *)Scene_AllocateObject(scene, Class_MainCamera);
    AssertNew(mainCamera);

    int rendererW, rendererH;
    SDL_RenderGetLogicalSize(renderer, &rendererW, &rendererH);

    MainCamera_Constructor(mainCamera, scene, rendererW, rendererH);
    scene->m_cameras[0] = Object_Cast(mainCamera, Class_Camera);

    UserCamera *userCamera = (UserCamera *)Scene_AllocateObject(scene, Class_UserCamera);
    AssertNew(userCamera);

    UserCamera_Constructor(userCamera, scene, rendererW, rendererH, (Camera *)mainCamera);
    scene->m_cameras[1] = Object_Cast(userCamera, Class_Camera);

    Scene_SetActiveCamera(self, mainCamera);

    // Crée le Player
    Player *player = (Player *)Scene_AllocateObject(scene, Class_Player);
    AssertNew(player);

    Player_Constructor(player, scene);
    scene->m_player = player;

    // Crée la TileMap
    LevelParser *parser = LevelParser_New(path);
    AssertNew(parser);
    LevelParser_InitScene(parser, scene);
    LevelParser_Delete(parser);
    parser = NULL;

    // Crée l'interface utilisateur
    LevelCanvas *canvas = (LevelCanvas *)Scene_AllocateObject(scene, Class_LevelCanvas);
    AssertNew(canvas);
    LevelCanvas_Constructor(canvas, scene);
    scene->m_canvas = canvas;

    // Crée le fond
    Background *background = (Background *)Scene_AllocateObject(scene, Class_Background);
    AssertNew(background);

    PE_Vec2 worldDim = PE_Vec2_One;
    int layerCount = 0;
    float *shiftFactors = NULL;
    switch (theme)
    {
    case THEME_LAKE:
    {
        worldDim = PE_Vec2_Set(24.0f, 24.0f * 1080.0f / 1920.0f);
        layerCount = 5;
        float factors[] = { 0.0f, 0.05f, 0.3f, 0.6f, 0.7f };
        shiftFactors = factors;
    }   break;

    case THEME_MOUNTAINS:
    default:
    {
        worldDim = PE_Vec2_Set(36.0f, 36.0f * 1080.0f / 2880.0f);
        layerCount = 4;
        float factors[] = { 0.0f, 0.05f, 0.3f, 0.6f, 1.4f };
        shiftFactors = factors;
    }   break;
    }

    Background_Constructor(background, scene, LAYER_BACKGROUND, layerCount, worldDim);

    AssetManager *assets = Scene_GetAssetManager(self);
    SDL_Texture **layers = AssetManager_GetLayersTextures(assets);
    for (int i = 0; i < layerCount; i++)
    {
        Background_SetLayer(background, layers[i], i);
        Background_SetXShiftFactors(background, shiftFactors[i], i);
        Background_SetYShiftFactors(background, shiftFactors[i], i);
    }

    if (theme == THEME_MOUNTAINS)
    {
        Background *foreground = (Background *)Scene_AllocateObject(scene, Class_Background);
        AssertNew(foreground);

        Background_Constructor(
            foreground, scene, LAYER_FOREGROUND, 1,
            PE_Vec2_Set(36.0f, 36.0f * 400.0f / 2880.0f)
        );
        Background_SetLayer(foreground, layers[4], 0);
        Background_SetXShiftFactors(foreground, 1.4f, 0);
        Background_SetYShiftFactors(foreground, 1.4f, 0);
    }
}

void LevelScene_VM_Destructor(void *self)
{
    LevelScene *scene = Object_Cast(self, Class_LevelScene);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_LevelScene);
}