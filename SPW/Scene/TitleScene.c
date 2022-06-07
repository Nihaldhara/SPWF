#include "TitleScene.h"
#include "../Utils/Renderer.h"
#include "../GameObject/Camera/TitleCamera.h"
#include "../GameObject/Background.h"

// Object virtual methods
void TitleScene_VM_Destructor(void *self);

static TitleSceneClass _Class_TitleScene = { 0 };
const void *const Class_TitleScene = &_Class_TitleScene;

void Class_InitTitleScene()
{
    if (!Class_IsInitialized(Class_TitleScene))
    {
        Class_InitScene();

        void *self = (void *)Class_TitleScene;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Scene,
            .name = "TitleScene",
            .instanceSize = sizeof(TitleScene),
            .classSize = sizeof(TitleSceneClass)
        };
        Class_Constructor(params, TitleScene_VM_Destructor);
    }
}

void TitleScene_Constructor(void *self, SDL_Renderer *renderer)
{
    Scene_Constructor(self, renderer, THEME_MOUNTAINS);
    Object_SetClass(self, Class_TitleScene);

    TitleScene *scene = Object_Cast(self, Class_TitleScene);
    scene->m_selectedLevel = -1;

    TitleManager *manager = Scene_AllocateObject(scene, Class_TitleManager);
    AssertNew(manager);
    TitleManager_Constructor(manager, scene);
    scene->m_manager = manager;

    TitleCamera *ttlCam = (TitleCamera *)Scene_AllocateObject(scene, Class_TitleCamera);
    AssertNew(ttlCam);

    int rendererW, rendererH;
    SDL_RenderGetLogicalSize(renderer, &rendererW, &rendererH);

    TitleCamera_Constructor(ttlCam, scene, rendererW, rendererH);
    Scene_SetActiveCamera(self, ttlCam);

    // Crée l'interface utilisateur
    TitleCanvas *ttlCanvas = (TitleCanvas *)Scene_AllocateObject(scene, Class_TitleCanvas);
    AssertNew(ttlCanvas);
    TitleCanvas_Constructor(ttlCanvas, scene);
    scene->m_canvas = ttlCanvas;

    // Crée le fond
    Background *background = (Background *)Scene_AllocateObject(scene, Class_Background);
    AssertNew(background);

    PE_Vec2 worldDim = PE_Vec2_One;
    int layerCount = 0;
    float *shiftFactors = NULL;

    worldDim = PE_Vec2_Set(36.0f, 36.0f * 1080.0f / 2880.0f);
    layerCount = 4;
    float factors[] = { 0.0f, 0.05f, 0.3f, 0.6f, 1.4f };
    shiftFactors = factors;

    Background_Constructor(background, scene, LAYER_BACKGROUND, layerCount, worldDim);

    AssetManager *assets = Scene_GetAssetManager(self);
    SDL_Texture **layers = AssetManager_GetLayersTextures(assets);
    for (int i = 0; i < layerCount; i++)
    {
        Background_SetLayer(background, layers[i], i);
        Background_SetXShiftFactors(background, shiftFactors[i], i);
        Background_SetYShiftFactors(background, shiftFactors[i], i);
    }
}

void TitleScene_VM_Destructor(void *self)
{
    TitleScene *scene = Object_Cast(self, Class_TitleScene);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_TitleScene);
}