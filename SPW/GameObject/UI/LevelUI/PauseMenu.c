#include "PauseMenu.h"
#include "../../../Scene/LevelScene.h"
#include "../Button.h"
#include "../Image.h"
#include "../UIObject.h"
#include "../Text.h"
#include "LevelCanvas.h"

// Object virtual methods
void PauseMenu_VM_Destructor(void *self);

// GameObject virtual methods

static PauseMenuClass _Class_PauseMenu = { 0 };
const void *const Class_PauseMenu = &_Class_PauseMenu;

void Class_InitPauseMenu()
{
    if (!Class_IsInitialized(Class_PauseMenu))
    {
        Class_InitUIObject();

        void *self = (void *)Class_PauseMenu;
        ClassCtorParams params = {
            .self = self,
            .super = Class_UIObject,
            .name = "PauseMenu",
            .instanceSize = sizeof(PauseMenu),
            .classSize = sizeof(PauseMenuClass)
        };
        Class_Constructor(params, PauseMenu_VM_Destructor);
    }
}

void ButtonExec_PauseRespawn(void *self, void *data)
{
    Scene *scene = data;
    LevelManager *manager = LevelScene_GetManager(scene);

    Scene_Respawn(scene);
    LevelManager_SetPause(manager, false);
}

void ButtonExec_PauseResume(void *self, void *data)
{
    Scene *scene = data;
    LevelManager *manager = LevelScene_GetManager(scene);
    LevelManager_SetPause(manager, false);
}

void ButtonExec_PauseOpenControls(void *self, void *data)
{
    Scene *scene = data;
    LevelCanvas *LevelCanvas = Object_Cast(
        LevelScene_GetCanvas(scene), Class_LevelCanvas
    );
    LevelCanvas_OpenControls(LevelCanvas);
}

void ButtonExec_PauseQuit(void *self, void *data)
{
    Scene *scene = data;
    Scene_Quit(scene);
}

void PauseMenu_Constructor(void *self, void *scene)
{
    UIObject_Constructor(self, scene);
    Object_SetClass(self, Class_PauseMenu);

    PauseMenu *menu = Object_Cast(self, Class_PauseMenu);
    AssetManager *assets = Scene_GetAssetManager(scene);
    TTF_Font *largeFont = AssetManager_GetLargeFont(assets);
    TTF_Font *font = AssetManager_GetNormalFont(assets);
    RE_Atlas *atlas = AssetManager_GetUIAtlas(assets);
    SDL_Color mainColor = AssetManager_GetNormalColor(assets);
    SDL_Color black = { 0, 0, 0, 255 };

    float buttonH = 55.0f;
    float topSkip = 100.0f;
    float sep = 10.0f;
    float panelW = 350.0f;
    float panelH = topSkip + 4.0f * buttonH + 3.0f * sep;

    UIRect rect;
    rect.m_anchorMin = Vec2_Set(0.5f, 0.5f);
    rect.m_anchorMax = Vec2_Set(0.5f, 0.5f);
    rect.m_offsetMin = Vec2_Set(-0.5f * panelW, -0.5f * panelH);
    rect.m_offsetMax = Vec2_Set(+0.5f * panelW, +0.5f * panelH);
    UIObject_SetLocalRect(menu, rect);

    Text *text = Scene_AllocateObject(scene, Class_Text);
    AssertNew(text);
    Text_Constructor(text, scene, "Pause", largeFont, mainColor);
    Text_SetAnchor(text, RE_ANCHOR_TOP | RE_ANCHOR_CENTER);

    rect.m_anchorMin = Vec2_Set(0.0f, 0.0f);
    rect.m_anchorMax = Vec2_Set(1.0f, 0.0f);
    rect.m_offsetMin = Vec2_Set(0.0f, 0.0f);
    rect.m_offsetMax = Vec2_Set(0.0f, buttonH);

    UIObject_SetLocalRect(text, rect);
    GameObject_SetParent(text, menu);
    
    RE_AtlasPart *buttonAtlas = RE_Atlas_GetPart(atlas, "Button");
    AssertNew(buttonAtlas);

    char *texts[4] = { u8"Continuer", u8"Recommencer", u8"Contrôles", u8"Quitter" };
    ButtonExec functions[4] = {
        ButtonExec_PauseResume, ButtonExec_PauseRespawn,
        ButtonExec_PauseOpenControls, ButtonExec_PauseQuit
    };
    float curY = topSkip;
    for (int i = 0; i < 4; ++i, curY += buttonH + sep)
    {
        Button *button = Scene_AllocateObject(scene, Class_Button);
        AssertNew(button);
        Button_Constructor(button, scene, buttonAtlas);
        Button_SetExec(button, functions[i], scene);

        rect.m_anchorMin = Vec2_Set(0.0f, 0.0f);
        rect.m_anchorMax = Vec2_Set(1.0f, 0.0f);
        rect.m_offsetMin = Vec2_Set(0.0f, curY);
        rect.m_offsetMax = Vec2_Set(0.0f, curY + buttonH);

        UIObject_SetLocalRect(button, rect);
        GameObject_SetParent(button, menu);

        UIBorders borders = { 25, 25, 25, 25 };
        Button_SetBorders(button, &borders);

        Button_SetTextUp(button, texts[i], font, mainColor);
        Button_SetTextHover(button, texts[i], font, black);
        Button_SetTextDown(button, texts[i], font, black);
    }
}

void PauseMenu_VM_Destructor(void *self)
{
    // Tous les enfants sont supprimés automatiquement par la scène
    //PauseMenu *menu = Object_Cast(self, Class_PauseMenu);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_PauseMenu);
}