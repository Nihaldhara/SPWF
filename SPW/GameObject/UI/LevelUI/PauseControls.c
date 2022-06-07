#include "PauseControls.h"
#include "../../../Scene/LevelScene.h"
#include "../Button.h"
#include "../Image.h"
#include "../UIObject.h"
#include "../Text.h"
#include "LevelCanvas.h"

// Object virtual methods
void PauseControls_VM_Destructor(void *self);

// GameObject virtual methods

static PauseControlsClass _Class_PauseControls = { 0 };
const void *const Class_PauseControls = &_Class_PauseControls;

void Class_InitPauseControls()
{
    if (!Class_IsInitialized(Class_PauseControls))
    {
        Class_InitUIObject();

        void *self = (void *)Class_PauseControls;
        ClassCtorParams params = {
            .self = self,
            .super = Class_UIObject,
            .name = "PauseControls",
            .instanceSize = sizeof(PauseControls),
            .classSize = sizeof(PauseControlsClass)
        };
        Class_Constructor(params, PauseControls_VM_Destructor);
    }
}

void PauseControls_ExecGoToMenu(void *self, void *data)
{
    Scene *scene = data;
    LevelCanvas *LevelCanvas = Object_Cast(LevelScene_GetCanvas(scene), Class_LevelCanvas);
    LevelCanvas_OpenPauseMenu(LevelCanvas);
}

void PauseControls_Constructor(void *self, void *scene)
{
    UIObject_Constructor(self, scene);
    Object_SetClass(self, Class_PauseControls);

    PauseControls *controls = Object_Cast(self, Class_PauseControls);
    AssetManager *assets = Scene_GetAssetManager(scene);
    TTF_Font *largeFont = AssetManager_GetLargeFont(assets);
    TTF_Font *font = AssetManager_GetNormalFont(assets);
    SDL_Color mainColor = AssetManager_GetNormalColor(assets);
    SDL_Color black = { 0, 0, 0, 255 };

    int w, h;
    TTF_SizeUTF8(font, u8"00", &w, &h);
    float rowH = (float)h;
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
    UIObject_SetLocalRect(controls, rect);

    Text *text = Scene_AllocateObject(scene, Class_Text);
    AssertNew(text);
    Text_Constructor(text, scene, u8"Contrôles", largeFont, mainColor);
    Text_SetAnchor(text, RE_ANCHOR_TOP | RE_ANCHOR_CENTER);

    rect.m_anchorMin = Vec2_Set(0.0f, 0.0f);
    rect.m_anchorMax = Vec2_Set(1.0f, 0.0f);
    rect.m_offsetMin = Vec2_Set(0.0f, 0.0f);
    rect.m_offsetMax = Vec2_Set(0.0f, buttonH);

    UIObject_SetLocalRect(text, rect);
    GameObject_SetParent(text, controls);

    RE_Atlas *atlas = AssetManager_GetUIAtlas(assets);
    RE_AtlasPart *buttonAtlas = RE_Atlas_GetPart(atlas, "Button");
    AssertNew(buttonAtlas);

    float curY = topSkip;
    char *labels[2] = { u8"Déplacement", u8"Saut" };
    for (int i = 0; i < 2; ++i, curY += rowH + sep)
    {
        Text *labelText = Scene_AllocateObject(scene, Class_Text);
        AssertNew(labelText);
        Text_Constructor(labelText, scene, labels[i], font, mainColor);

        rect.m_anchorMin = Vec2_Set(0.0f, 0.0f);
        rect.m_anchorMax = Vec2_Set(1.0f, 0.0f);
        rect.m_offsetMin = Vec2_Set(0.0f, curY);
        rect.m_offsetMax = Vec2_Set(0.0f, curY + rowH);
        UIObject_SetLocalRect(labelText, rect);
        GameObject_SetParent(labelText, controls);

        Text_SetAnchor(labelText, RE_ANCHOR_TOP | RE_ANCHOR_LEFT);
    }

    {
        Button *button = Scene_AllocateObject(scene, Class_Button);
        AssertNew(button);
        Button_Constructor(button, scene, buttonAtlas);
        Button_SetExec(button, PauseControls_ExecGoToMenu, scene);

        rect.m_anchorMin = Vec2_Set(0.0f, 0.0f);
        rect.m_anchorMax = Vec2_Set(1.0f, 0.0f);
        rect.m_offsetMin = Vec2_Set(0.0f, curY);
        rect.m_offsetMax = Vec2_Set(0.0f, curY + buttonH);
        UIObject_SetLocalRect(button, rect);
        GameObject_SetParent(button, controls);

        UIBorders borders = { 25, 25, 25, 25 };
        Button_SetBorders(button, &borders);

        Button_SetTextUp(button, u8"Retour", font, mainColor);
        Button_SetTextHover(button, u8"Retour", font, black);
        Button_SetTextDown(button, u8"Retour", font, black);
    }
}

void PauseControls_VM_Destructor(void *self)
{
    // Tous les enfants sont supprimés automatiquement par la scène
    PauseControls *controls = Object_Cast(self, Class_PauseControls);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_PauseControls);
}