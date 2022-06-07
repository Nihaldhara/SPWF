#include "TitleCanvas.h"
#include "../../../Scene/TitleScene.h"
#include "StartScreen.h"
#include "LevelSelection.h"

// Object virtual methods
void TitleCanvas_VM_Destructor(void *self);

// GameObject virtual methods
void TitleCanvas_VM_Render(void *self);

static TitleCanvasClass _Class_TitleCanvas = { 0 };
const void *const Class_TitleCanvas = &_Class_TitleCanvas;

void Class_InitTitleCanvas()
{
    if (!Class_IsInitialized(Class_TitleCanvas))
    {
        Class_InitCanvas();

        void *self = (void *)Class_TitleCanvas;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Canvas,
            .name = "TitleCanvas",
            .instanceSize = sizeof(TitleCanvas),
            .classSize = sizeof(TitleCanvasClass)
        };
        Class_Constructor(params, TitleCanvas_VM_Destructor);
        ((GameObjectClass *)self)->Render = TitleCanvas_VM_Render;
    }
}

void TitleCanvas_Constructor(void *self, void *scene)
{
    Canvas_Constructor(self, scene);
    Object_SetClass(self, Class_TitleCanvas);

    TitleCanvas *ttlCanvas = Object_Cast(self, Class_TitleCanvas);

    StartScreen *menu = Scene_AllocateObject(scene, Class_StartScreen);
    AssertNew(menu);
    StartScreen_Constructor(menu, scene);
    GameObject_SetParent(menu, ttlCanvas);

    ttlCanvas->m_activeMenu = Object_Cast(menu, Class_UIObject);
}

static void TitleCanvas_CloseActiveMenu(void *self)
{
    TitleCanvas *ttlCanvas = Object_Cast(self, Class_TitleCanvas);
    Scene *scene = GameObject_GetScene(self);

    if (ttlCanvas->m_activeMenu)
    {
        Scene_DeleteObject(scene, ttlCanvas->m_activeMenu);
    }

    ttlCanvas->m_activeMenu = NULL;
}

void TitleCanvas_OpenLevelSelection(void *self)
{
    TitleCanvas *ttlCanvas = Object_Cast(self, Class_TitleCanvas);
    Scene *scene = GameObject_GetScene(self);

    // Ferme le menu actif (si existant)
    TitleCanvas_CloseActiveMenu(ttlCanvas);

    // Crée le nouveau menu
    LevelSelection *lvlSelect = Scene_AllocateObject(scene, Class_LevelSelection);
    AssertNew(lvlSelect);
    LevelSelection_Constructor(lvlSelect, scene);
    GameObject_SetParent(lvlSelect, ttlCanvas);

    ttlCanvas->m_activeMenu = Object_Cast(lvlSelect, Class_UIObject);
}

void TitleCanvas_VM_Render(void *self)
{
    TitleCanvas *ttlCanvas = Object_Cast(self, Class_TitleCanvas);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);

    // Obscursit le fond
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(renderer, NULL);

    // Appelle la méthode "Render()" de la classe mère
    // Elle dessine tous les fils du canvas
    const GameObjectClass *clsDesc = Object_GetSuper(self);
    (clsDesc->Render)(self);
}

void TitleCanvas_VM_Destructor(void *self)
{
    // Tous les enfants sont supprimés automatiquement par la scène
    TitleCanvas *ttlCanvas = Object_Cast(self, Class_TitleCanvas);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_TitleCanvas);
}
