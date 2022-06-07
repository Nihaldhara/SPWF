#include "Background.h"
#include "../Scene/Scene.h"
#include "Camera/UserCamera.h"

// Object virtual methods
void Background_VM_Destructor(void *self);

// GameObject virtual methods
void Background_VM_Render(void *self);
void Background_VM_Update(void *self);

static BackgroundClass _Class_Background = { 0 };
const void *const Class_Background = &_Class_Background;

void Class_InitBackground()
{
    if (!Class_IsInitialized(Class_Background))
    {
        Class_InitGameObject();

        void *self = (void *)Class_Background;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameObject,
            .name = "Background",
            .instanceSize = sizeof(Background),
            .classSize = sizeof(BackgroundClass)
        };
        Class_Constructor(params, Background_VM_Destructor);
        ((GameObjectClass *)self)->Render = Background_VM_Render;
        ((GameObjectClass *)self)->Update = Background_VM_Update;
    }
}

void Background_Constructor(
    void *self, void *scene, int renderLayer, int layerCount, PE_Vec2 worldDim)
{
    GameObject_Constructor(self, scene, renderLayer);
    Object_SetClass(self, Class_Background);

    Background *background = Object_Cast(self, Class_Background);

    background->m_layerCount = layerCount;
    background->m_worldDim = worldDim;

    background->m_layers = (SDL_Texture **)calloc(layerCount, sizeof(SDL_Texture *));
    AssertNew(background->m_layers);

    background->m_xShiftFactors = (float *)calloc(layerCount, sizeof(float));
    AssertNew(background->m_xShiftFactors);

    background->m_yShiftFactors = (float *)calloc(layerCount, sizeof(float));
    AssertNew(background->m_yShiftFactors);
}

void Background_VM_Destructor(void *self)
{
    Background *background = Object_Cast(self, Class_Background);
    
    free(background->m_layers);
    free(background->m_xShiftFactors);
    free(background->m_yShiftFactors);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Background);
}

void Background_VM_Render(void *self)
{
    Background *background = Object_Cast(self, Class_Background);
    Scene *scene = GameObject_GetScene(background);
    Camera *camera = Scene_GetActiveCamera(scene);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);
    PE_AABB view = Camera_GetWorldView(camera);

    if (Object_IsA(camera, Class_UserCamera))
    {
        return;
    }

    int layerCount = background->m_layerCount;
    float *xShiftFactors = background->m_xShiftFactors;
    float *yShiftFactors = background->m_yShiftFactors;

    // Dimension du fond dans le référentiel monde
    float scale = Camera_GetWorldToViewScale(camera);
    float layerW = scale * background->m_worldDim.x;
    float layerH = scale * background->m_worldDim.y;

    // Dessine les différents calques du fond (parallax)
    for (int i = 0; i < layerCount; ++i)
    {
        PE_Vec2 origin = PE_Vec2_Set(
            view.lower.x * (1.0f - xShiftFactors[i]),
            view.lower.y * (1.0f - yShiftFactors[i])
        );

        if (background->m_layers[i] == NULL)
        {
            continue;
        }

        float x, y;
        Camera_WorldToView(camera, origin, &x, &y);

        while (x < layerW)
        {
            x += layerW;
        }
        while (x > layerW)
        {
            x -= layerW;
        }

        SDL_FRect dstRect = { 0 };
        dstRect.x = x;
        dstRect.y = y;
        dstRect.w = layerW;
        dstRect.h = layerH;

        RE_RenderCopyF(
            renderer, background->m_layers[i], NULL, &dstRect,
            RE_ANCHOR_LEFT | RE_ANCHOR_BOTTOM
        );

        dstRect.x = x - layerW;
        RE_RenderCopyF(
            renderer, background->m_layers[i], NULL, &dstRect,
            RE_ANCHOR_LEFT | RE_ANCHOR_BOTTOM
        );
    }
}

void Background_VM_Update(void *self)
{
    Background *background = Object_Cast(self, Class_Background);
    Scene *scene = GameObject_GetScene(background);
    Scene_SetVisible(scene, background);
}