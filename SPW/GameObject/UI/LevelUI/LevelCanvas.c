#include "LevelCanvas.h"
#include "../../../Scene/LevelScene.h"
#include "PauseControls.h"
#include "PauseMenu.h"
#include "SceneStatsUI.h"
#include "LevelHeader.h"

// Object virtual methods
void LevelCanvas_VM_Destructor(void *self);

// GameObject virtual methods
void LevelCanvas_VM_OnRespawn(void *self);
void LevelCanvas_VM_Render(void *self);

static LevelCanvasClass _Class_LevelCanvas = { 0 };
const void *const Class_LevelCanvas = &_Class_LevelCanvas;

void Class_InitLevelCanvas()
{
    if (!Class_IsInitialized(Class_LevelCanvas))
    {
        Class_InitCanvas();

        void *self = (void *)Class_LevelCanvas;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Canvas,
            .name = "LevelCanvas",
            .instanceSize = sizeof(LevelCanvas),
            .classSize = sizeof(LevelCanvasClass)
        };
        Class_Constructor(params, LevelCanvas_VM_Destructor);
        ((GameObjectClass *)self)->OnRespawn = LevelCanvas_VM_OnRespawn;
        ((GameObjectClass *)self)->Render    = LevelCanvas_VM_Render;
    }
}

void LevelCanvas_Constructor(void *self, void *scene)
{
    Canvas_Constructor(self, scene);
    Object_SetClass(self, Class_LevelCanvas);

    LevelCanvas *lvlCanvas = Object_Cast(self, Class_LevelCanvas);

    lvlCanvas->m_activeMenu = NULL;

    LevelHeader *lvlHeader = Scene_AllocateObject(scene, Class_LevelHeader);
    AssertNew(lvlHeader);

    LevelHeader_Constructor(lvlHeader, scene);
    GameObject_SetParent(lvlHeader, lvlCanvas);
}

void LevelCanvas_CloseActiveMenu(void *self)
{
    LevelCanvas *lvlCanvas = Object_Cast(self, Class_LevelCanvas);
    Scene *scene = GameObject_GetScene(self);

    if (lvlCanvas->m_activeMenu)
    {
        Scene_DeleteObject(scene, lvlCanvas->m_activeMenu);
    }

    lvlCanvas->m_activeMenu = NULL;
}

void LevelCanvas_ShowSceneStats(void *self, bool showInfo)
{
    LevelCanvas *lvlCanvas = Object_Cast(self, Class_LevelCanvas);
    Scene *scene = GameObject_GetScene(self);

    if ((showInfo == true) && (lvlCanvas->m_scnStatsUI == NULL))
    {
        SceneStatsUI *sceneStats = Scene_AllocateObject(scene, Class_SceneStatsUI);
        AssertNew(sceneStats);

        SceneStatsUI_Constructor(sceneStats, scene);
        GameObject_SetParent(sceneStats, lvlCanvas);
        lvlCanvas->m_scnStatsUI = Object_Cast(sceneStats, Class_UIObject);
    }
    else if ((showInfo == false) && (lvlCanvas->m_scnStatsUI != NULL))
    {
        Scene_DeleteObject(scene, lvlCanvas->m_scnStatsUI);
        lvlCanvas->m_scnStatsUI = NULL;
    }
}

void LevelCanvas_OpenPauseMenu(void *self)
{
    LevelCanvas *lvlCanvas = Object_Cast(self, Class_LevelCanvas);
    Scene *scene = GameObject_GetScene(self);

    // Ferme le menu actif (si existant)
    LevelCanvas_CloseActiveMenu(self);

    // Crée le nouveau menu
    PauseMenu *menu = Scene_AllocateObject(scene, Class_PauseMenu);
    AssertNew(menu);

    PauseMenu_Constructor(menu, scene);
    GameObject_SetParent(menu, lvlCanvas);
    lvlCanvas->m_activeMenu = Object_Cast(menu, Class_UIObject);
}

void LevelCanvas_OpenControls(void *self)
{
    LevelCanvas *lvlCanvas = Object_Cast(self, Class_LevelCanvas);
    Scene *scene = GameObject_GetScene(self);

    // Ferme le menu actif (si existant)
    LevelCanvas_CloseActiveMenu(self);

    // Crée le nouveau menu
    PauseControls *menu = Scene_AllocateObject(scene, Class_PauseControls);
    AssertNew(menu);

    PauseControls_Constructor(menu, scene);
    GameObject_SetParent(menu, lvlCanvas);
    lvlCanvas->m_activeMenu = Object_Cast(menu, Class_UIObject);
}

void LevelCanvas_VM_OnRespawn(void *self)
{
    // Ferme le menu actif (si existant)
    LevelCanvas_CloseActiveMenu(self);
}

void LevelCanvas_VM_Render(void *self)
{
    LevelCanvas *lvlCanvas = Object_Cast(self, Class_LevelCanvas);
    Scene *scene = GameObject_GetScene(self);
    LevelManager *manager = LevelScene_GetManager(scene);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);

    // Obscursit le fond quand le jeu est en pause
    if (LevelManager_IsGamePaused(manager))
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_RenderFillRect(renderer, NULL);
    }

    // Appelle la méthode "Render()" de la classe mère
    // Elle dessine tous les fils du canvas
    const GameObjectClass *clsDesc = Object_GetSuper(self);
    (clsDesc->Render)(self);
}

void LevelCanvas_VM_Destructor(void *self)
{
    // Tous les enfants sont supprimés automatiquement par la scène
    LevelCanvas *lvlCanvas = Object_Cast(self, Class_LevelCanvas);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_LevelCanvas);
}
