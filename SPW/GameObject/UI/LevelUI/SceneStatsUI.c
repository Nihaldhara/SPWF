#include "SceneStatsUI.h"
#include "../../../Scene/LevelScene.h"
#include "../Button.h"
#include "../Image.h"
#include "../UIObject.h"
#include "../Text.h"

// Object virtual methods
void SceneStatsUI_VM_Destructor(void *self);

// GameObject virtual methods
void SceneStatsUI_VM_Render(void *self);
void SceneStatsUI_VM_Update(void *self);

static SceneStatsUIClass _Class_SceneStatsUI = { 0 };
const void *const Class_SceneStatsUI = &_Class_SceneStatsUI;

void Class_InitSceneStatsUI()
{
    if (!Class_IsInitialized(Class_SceneStatsUI))
    {
        Class_InitUIObject();

        void *self = (void *)Class_SceneStatsUI;
        ClassCtorParams params = {
            .self = self,
            .super = Class_UIObject,
            .name = "SceneStatsUI",
            .instanceSize = sizeof(SceneStatsUI),
            .classSize = sizeof(SceneStatsUIClass)
        };
        Class_Constructor(params, SceneStatsUI_VM_Destructor);
        ((GameObjectClass *)self)->Update = SceneStatsUI_VM_Update;
        ((GameObjectClass *)self)->Render = SceneStatsUI_VM_Render;
    }
}

void SceneStatsUI_Constructor(void *self, void *scene)
{
    UIObject_Constructor(self, scene);
    Object_SetClass(self, Class_SceneStatsUI);

    SceneStatsUI *panel = Object_Cast(self, Class_SceneStatsUI);
    AssetManager *assets = Scene_GetAssetManager(scene);
    TTF_Font *font = AssetManager_GetNormalFont(assets);
    SDL_Color color = AssetManager_GetNormalColor(assets);
    GameObject_SetLayer(self, LAYER_UI_BACKGROUND);

    int w, h;
    TTF_SizeUTF8(font, u8"GameObjects", &w, &h);
    float margin = 15.0f;
    float panelW = w + 2.0f * margin;
    float panelH = 5.0f * h + 2.0f * margin;
    float colW = w + 20.0f;
    float skip = h + 0.0f;
    TTF_SizeUTF8(font, u8"0000", &w, &h);
    panelW += w;

    UIRect rect;
    rect.m_anchorMin = Vec2_Set(1.0f, 0.0f);
    rect.m_anchorMax = Vec2_Set(1.0f, .0f);
    rect.m_offsetMin = Vec2_Set(-80.0f - panelW, 40.0f);
    rect.m_offsetMax = Vec2_Set(-80.0f, 40.0f + panelH);
    UIObject_SetLocalRect(panel, rect);
    
    float curY = margin;
    char *textStrings[5] = {
        u8"GameObjects", u8"Disabled", u8"Bodies", u8"Visibles", u8"Canvas UI"
    };
    Text *counterTexts[5] = { 0 };
    for (int i = 0; i < 5; ++i, curY += skip)
    {
        Text *label = Scene_AllocateObject(scene, Class_Text);
        AssertNew(label);
        Text_Constructor(label, scene, textStrings[i], font, color);
        Text_SetAnchor(label, RE_ANCHOR_LEFT | RE_ANCHOR_TOP);
        rect.m_anchorMin = Vec2_Set(0.0f, 0.0f);
        rect.m_anchorMax = Vec2_Set(0.0f, 0.0f);
        rect.m_offsetMin = Vec2_Set(margin, curY);
        rect.m_offsetMax = Vec2_Set(margin + colW, curY + skip);
        UIObject_SetLocalRect(label, rect);
        GameObject_SetParent(label, panel);

        counterTexts[i] = Scene_AllocateObject(scene, Class_Text);
        AssertNew(counterTexts[i]);
        Text_Constructor(counterTexts[i], scene, "0", font, color);
        Text_SetAnchor(counterTexts[i], RE_ANCHOR_LEFT | RE_ANCHOR_TOP);
        rect.m_anchorMin = Vec2_Set(0.0f, 0.0f);
        rect.m_anchorMax = Vec2_Set(1.0f, 0.0f);
        rect.m_offsetMin = Vec2_Set(margin + colW, curY);
        rect.m_offsetMax = Vec2_Set(-margin, curY + skip);
        UIObject_SetLocalRect(counterTexts[i], rect);
        GameObject_SetParent(counterTexts[i], panel);
    }

    panel->m_objectCountText = counterTexts[0];
    panel->m_disabledCountText = counterTexts[1];
    panel->m_bodyCountText = counterTexts[2];
    panel->m_visibleCountText = counterTexts[3];
    panel->m_uiCountText = counterTexts[4];
}

void SceneStatsUI_VM_Destructor(void *self)
{
    // Tous les enfants sont supprimés automatiquement par la scène
    SceneStatsUI *panel = Object_Cast(self, Class_SceneStatsUI);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_SceneStatsUI);
}

void SceneStatsUI_VM_Update(void *self)
{
    SceneStatsUI *panel = Object_Cast(self, Class_SceneStatsUI);
    Scene *scene = GameObject_GetScene(self);
    SceneStats stats = Scene_GetStats(scene);
    char str[128] = { 0 };

    sprintf(str, "%d", stats.objectCount);
    Text_SetString(panel->m_objectCountText, str);

    sprintf(str, "%d", stats.disabledCount);
    Text_SetString(panel->m_disabledCountText, str);

    sprintf(str, "%d", stats.bodyCount);
    Text_SetString(panel->m_bodyCountText, str);

    sprintf(str, "%d", stats.visibleCount);
    Text_SetString(panel->m_visibleCountText, str);

    LevelCanvas *canvas = LevelScene_GetCanvas(scene);
    sprintf(str, "%d", Canvas_GetUICount(canvas));
    Text_SetString(panel->m_uiCountText, str);
}


void SceneStatsUI_VM_Render(void *self)
{
    SceneStatsUI *panel = Object_Cast(self, Class_SceneStatsUI);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);
    SDL_FRect rect = UIObject_GetCanvasRect(self);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderFillRectF(renderer, &rect);
}