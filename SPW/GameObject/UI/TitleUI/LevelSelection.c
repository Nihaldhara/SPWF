#include "LevelSelection.h"
#include "../../../Scene/TitleScene.h"
#include "../Button.h"
#include "../Image.h"
#include "../UIObject.h"
#include "../Text.h"

// Object virtual methods
void LevelSelection_VM_Destructor(void *self);

// GameObject virtual methods

static LevelSelectionClass _Class_LevelSelection = { 0 };
const void *const Class_LevelSelection = &_Class_LevelSelection;

void Class_InitLevelSelection()
{
    if (!Class_IsInitialized(Class_LevelSelection))
    {
        Class_InitUIObject();

        void *self = (void *)Class_LevelSelection;
        ClassCtorParams params = {
            .self = self,
            .super = Class_UIObject,
            .name = "LevelSelection",
            .instanceSize = sizeof(LevelSelection),
            .classSize = sizeof(LevelSelectionClass)
        };
        Class_Constructor(params, LevelSelection_VM_Destructor);
    }
}

void ButtonExec_SelectLevel(void *self, void *data)
{
    Button *button = Object_Cast(self, Class_Button);
    TitleScene *scene = Object_Cast(GameObject_GetScene(button), Class_TitleScene);
    int levelIdx = *(int *)data;

    TitleScene_SetSelectedLevel(scene, levelIdx);
    Scene_Quit(scene);
}

void LevelSelection_Constructor(void *self, void *scene)
{
    UIObject_Constructor(self, scene);
    Object_SetClass(self, Class_LevelSelection);

    LevelSelection *menu = Object_Cast(self, Class_LevelSelection);
    AssetManager *assets = Scene_GetAssetManager(scene);
    TTF_Font *largeFont = AssetManager_GetLargeFont(assets);
    TTF_Font *font = AssetManager_GetNormalFont(assets);
    RE_Atlas *atlas = AssetManager_GetUIAtlas(assets);
    RE_Atlas *logoAtlas = AssetManager_GetLogoAtlas(assets);
    SDL_Color mainColor = AssetManager_GetNormalColor(assets);
    SDL_Color black = { 0, 0, 0, 255 };

    int levelCount = LEVEL_COUNT;
    menu->m_levelCount = levelCount;
    menu->m_levelIndices = (int *)calloc(levelCount, sizeof(int));
    AssertNew(menu->m_levelIndices);

    for (int i = 0; i < levelCount; i++)
    {
        menu->m_levelIndices[i] = i;
    }

    float buttonH = 55.0f;
    float topSkip = 100.0f;
    float sep = 10.0f;
    float panelW = 350.0f;
    float panelH = topSkip + LEVEL_COUNT * (buttonH + sep) - sep;

    UIRect rect;
    rect.m_anchorMin = Vec2_Set(0.5f, 0.5f);
    rect.m_anchorMax = Vec2_Set(0.5f, 0.5f);
    rect.m_offsetMin = Vec2_Set(-0.5f * panelW, -0.5f * panelH);
    rect.m_offsetMax = Vec2_Set(+0.5f * panelW, +0.5f * panelH);
    UIObject_SetLocalRect(menu, rect);

    Text *text = Scene_AllocateObject(scene, Class_Text);
    AssertNew(text);
    Text_Constructor(text, scene, u8"Sélection du niveau", largeFont, mainColor);
    Text_SetAnchor(text, RE_ANCHOR_TOP | RE_ANCHOR_CENTER);

    rect.m_anchorMin = Vec2_Set(0.0f, 0.0f);
    rect.m_anchorMax = Vec2_Set(1.0f, 0.0f);
    rect.m_offsetMin = Vec2_Set(0.0f, 0.0f);
    rect.m_offsetMax = Vec2_Set(0.0f, topSkip);

    UIObject_SetLocalRect(text, rect);
    GameObject_SetParent(text, menu);

    RE_AtlasPart *buttonAtlas = RE_Atlas_GetPart(atlas, "Button");
    AssertNew(buttonAtlas);

    float curY = topSkip;
    for (int i = 0; i < levelCount; ++i, curY += buttonH + sep)
    {
        int levelIdx = menu->m_levelIndices[i];

        Button *button = Scene_AllocateObject(scene, Class_Button);
        AssertNew(button);
        Button_Constructor(button, scene, buttonAtlas);

        Button_SetExec(button, ButtonExec_SelectLevel, &(menu->m_levelIndices[i]));

        rect.m_anchorMin = Vec2_Set(0.0f, 0.0f);
        rect.m_anchorMax = Vec2_Set(1.0f, 0.0f);
        rect.m_offsetMin = Vec2_Set(0.0f, curY);
        rect.m_offsetMax = Vec2_Set(0.0f, curY + buttonH);

        UIObject_SetLocalRect(button, rect);
        GameObject_SetParent(button, menu);

        UIBorders borders = { 15, 15, 15, 15 };
        Button_SetBorders(button, &borders);

        char *title = g_levelData[levelIdx].title;
        Button_SetTextUp(button, title, font, mainColor);
        Button_SetTextHover(button, title, font, black);
        Button_SetTextDown(button, title, font, black);
    }
}

void LevelSelection_VM_Destructor(void *self)
{
    // Tous les enfants sont supprimés automatiquement par la scène
    LevelSelection *menu = Object_Cast(self, Class_LevelSelection);

    free(menu->m_levelIndices);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_LevelSelection);
}