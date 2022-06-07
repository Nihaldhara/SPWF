#include "StartScreen.h"
#include "../../../Scene/TitleScene.h"
#include "../Button.h"
#include "../Image.h"
#include "../UIObject.h"
#include "../Text.h"

// Object virtual methods
void StartScreen_VM_Destructor(void *self);

// GameObject virtual methods

static StartScreenClass _Class_StartScreen = { 0 };
const void *const Class_StartScreen = &_Class_StartScreen;

void Class_InitStartScreen()
{
    if (!Class_IsInitialized(Class_StartScreen))
    {
        Class_InitUIObject();

        void *self = (void *)Class_StartScreen;
        ClassCtorParams params = {
            .self = self,
            .super = Class_UIObject,
            .name = "StartScreen",
            .instanceSize = sizeof(StartScreen),
            .classSize = sizeof(StartScreenClass)
        };
        Class_Constructor(params, StartScreen_VM_Destructor);
    }
}

void StartScreen_ExecContinue(void *self, void *data)
{
    TitleScene *scene = data;
    TitleCanvas *ttlCanvas = TitleScene_GetCanvas(scene);

    TitleCanvas_OpenLevelSelection(ttlCanvas);
}

void StartScreen_ExecQuit(void *self, void *data)
{
    Scene *scene = data;
    Scene_Quit(scene);
}

void StartScreen_Constructor(void *self, void *scene)
{
    UIObject_Constructor(self, scene);
    Object_SetClass(self, Class_StartScreen);

    StartScreen *menu = Object_Cast(self, Class_StartScreen);
    AssetManager *assets = Scene_GetAssetManager(scene);
    TTF_Font *largeFont = AssetManager_GetLargeFont(assets);
    TTF_Font *font = AssetManager_GetNormalFont(assets);
    RE_Atlas *atlas = AssetManager_GetUIAtlas(assets);
    RE_Atlas *logoAtlas = AssetManager_GetLogoAtlas(assets);
    SDL_Color mainColor = AssetManager_GetNormalColor(assets);
    SDL_Color black = { 0, 0, 0, 255 };

    int w, h;
    TTF_SizeUTF8(font, u8"00", &w, &h);
    float buttonH = 55.0f;
    float topSkip = 400.0f;
    float sep = 10.0f;
    float panelW = 350.0f;
    float panelH = topSkip + 2.0f * buttonH + 1.0f * sep;

    UIRect rect;
    rect.m_anchorMin = Vec2_Set(0.5f, 0.5f);
    rect.m_anchorMax = Vec2_Set(0.5f, 0.5f);
    rect.m_offsetMin = Vec2_Set(-0.5f * panelW, -0.5f * panelH);
    rect.m_offsetMax = Vec2_Set(+0.5f * panelW, +0.5f * panelH);
    UIObject_SetLocalRect(menu, rect);

    // Logo
    Image *logo = Scene_AllocateObject(scene, Class_Image);
    AssertNew(logo);
    RE_AtlasPart *logoPart = RE_Atlas_GetPart(logoAtlas, "Logo");
    Image_Constructor(logo, scene, logoPart, 0);
    float ratio = Image_GetNaturalRatio(logo);
    float logoW = 700.0f;

    rect.m_anchorMin = Vec2_Set(0.5f, 0.0f);
    rect.m_anchorMax = Vec2_Set(0.5f, 0.0f);
    rect.m_offsetMin = Vec2_Set(-0.5f * logoW, 0.0f);
    rect.m_offsetMax = Vec2_Set(+0.5f * logoW, logoW / ratio);

    UIObject_SetLocalRect(logo, rect);
    GameObject_SetParent(logo, menu);

    RE_AtlasPart *buttonAtlas = RE_Atlas_GetPart(atlas, "Button");
    AssertNew(buttonAtlas);

    char *texts[2] = { u8"Démarrer", u8"Quitter" };
    ButtonExec functions[2] = {
        StartScreen_ExecContinue, StartScreen_ExecQuit
    };
    float curY = topSkip;
    for (int i = 0; i < 2; ++i, curY += buttonH + sep)
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

void StartScreen_VM_Destructor(void *self)
{
    // Tous les enfants sont supprimés automatiquement par la scène
    StartScreen *menu = Object_Cast(self, Class_StartScreen);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_StartScreen);
}