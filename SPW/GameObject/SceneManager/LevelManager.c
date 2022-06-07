#include "LevelManager.h"
#include "../../Scene/LevelScene.h"
#include "../UI/LevelUI/LevelCanvas.h"

// Object virtual methods
void LevelManager_VM_Destructor(void *self);

// GameObject virtual methods
void LevelManager_VM_Update(void *self);

static LevelManagerClass _Class_LevelManager = { 0 };
const void *const Class_LevelManager = &_Class_LevelManager;

void Class_InitLevelManager()
{
    if (!Class_IsInitialized(Class_LevelManager))
    {
        Class_InitGameObject();

        void *self = (void *)Class_LevelManager;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameObject,
            .name = "LevelManager",
            .instanceSize = sizeof(LevelManager),
            .classSize = sizeof(LevelManagerClass)
        };
        Class_Constructor(params, LevelManager_VM_Destructor);
        ((GameObjectClass *)self)->Update = LevelManager_VM_Update;
    }
}

void LevelManager_Constructor(void *self, void *scene)
{
    GameObject_Constructor(self, scene, LAYER_FOREGROUND);
    Object_SetClass(self, Class_LevelManager);

    LevelManager *manager = Object_Cast(self, Class_LevelManager);

    manager->m_camIndex = 0;
    manager->m_paused = false;
    manager->m_showSceneStatsUI = false;
    manager->m_stepDelay = 0.0f;
}

void LevelManager_VM_Destructor(void *self)
{
    LevelManager *manager = Object_Cast(self, Class_LevelManager);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_LevelManager);
}

static void LevelManager_ChangePause(void *self)
{
    LevelManager *manager = Object_Cast(self, Class_LevelManager);

    Scene *scene = GameObject_GetScene(manager);
    InputManager *inputManager = Scene_GetInputManager(scene);
    LevelCanvas *lvlCanvas = LevelScene_GetCanvas(scene);

    manager->m_paused = !manager->m_paused;

    if (manager->m_paused)
    {
        RE_Timer_SetTimeScale(g_time, 0.0f);
        LevelCanvas_OpenPauseMenu(lvlCanvas);

        ControlsInput *controls = InputManager_GetControls(inputManager);
        InputGroup_SetEnabled(controls, false);
    }
    else
    {
        RE_Timer_SetTimeScale(g_time, 1.0f);
        LevelCanvas_CloseActiveMenu(lvlCanvas);

        ControlsInput *controls = InputManager_GetControls(inputManager);
        InputGroup_SetEnabled(controls, true);
    }
}

void LevelManager_SetPause(void *self, bool pauseStatus)
{
    LevelManager *manager = Object_Cast(self, Class_LevelManager);

    if (manager->m_paused != pauseStatus)
    {
        LevelManager_ChangePause(self);
    }
}

void LevelManager_VM_Update(void *self)
{
    LevelManager *lvlManager = Object_Cast(self, Class_LevelManager);
    Scene *scene = GameObject_GetScene(lvlManager);
    InputManager *inputManager = Scene_GetInputManager(scene);
    DebugInput *debug = InputManager_GetDebug(inputManager);
    AppInput *appInput = InputManager_GetApplication(inputManager);

    if (appInput->pausePressed)
    {
        LevelManager_ChangePause(self);
    }

    if (appInput->quitPressed)
    {
        Scene_Quit(scene);
    }

    if (debug->gizmosPressed)
    {
        bool drawn = Scene_AreGizmosDrawn(scene);
        Scene_SetDrawGizmos(scene, !drawn);
    }

    if (debug->bodyPressed)
    {
        bool drawn = Scene_AreBodiesDrawn(scene);
        Scene_SetDrawBodies(scene, !drawn);
    }

    if (debug->camPressed)
    {
        lvlManager->m_camIndex = (lvlManager->m_camIndex + 1) % 2;
        LevelScene_ChangeCamera(scene, lvlManager->m_camIndex);
    }

    if (debug->infoPressed)
    {
        LevelCanvas *lvlCanvas = LevelScene_GetCanvas(scene);
        lvlManager->m_showSceneStatsUI = !lvlManager->m_showSceneStatsUI;
        LevelCanvas_ShowSceneStats(lvlCanvas, lvlManager->m_showSceneStatsUI);
    }

    if (!lvlManager->m_paused)
    {
        int mode = Scene_GetFixedUpdateMode(scene);
        if (mode != FIXED_UPDATE_STEP_BY_STEP && debug->nextStepPressed)
        {
            mode = FIXED_UPDATE_STEP_BY_STEP;
            Scene_SetFixedUpdateMode(scene, mode);
        }
        else if (debug->quitStepPressed)
        {
            mode = FIXED_UPDATE_REALTIME;
            Scene_SetFixedUpdateMode(scene, mode);
        }

        // Mode pas-à-pas
        if (mode == FIXED_UPDATE_STEP_BY_STEP)
        {
            if (debug->nextStepPressed)
            {
                lvlManager->m_stepDelay = 0.5f;
                Scene_MakeStep(scene);
            }
            else if (debug->nextStepDown)
            {
                lvlManager->m_stepDelay -= RE_Timer_GetUnscaledDelta(g_time);
            }

            if (lvlManager->m_stepDelay < 0.0f)
            {
                Scene_MakeStep(scene);
                lvlManager->m_stepDelay = 1.0f / 10.0f;
            }
        }
    }
}