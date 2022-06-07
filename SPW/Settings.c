#include "Settings.h"

#include "Scene/Scene.h"
#  include "Scene/LevelScene.h"
#  include "Scene/TitleScene.h"

#include "Input/InputGroup.h"
#  include "Input/AppInput.h"
#  include "Input/ControlsInput.h"
#  include "Input/DebugInput.h"
#  include "Input/MouseInput.h"

#include "GameObject/GameObject.h"
#  include "GameObject/Background.h"
#  include "GameObject/Camera/Camera.h"
#    include "GameObject/Camera/MainCamera.h"
#    include "GameObject/Camera/UserCamera.h"
#    include "GameObject/Camera/TitleCamera.h"
#  include "GameObject/GameBody.h"
#    include "GameObject/Collectable/Collectable.h"
#      include "GameObject/Collectable/Firefly.h"
#      include "GameObject/Collectable/Heart.h"
#    include "GameObject/Enemy/Enemy.h"
#      include "GameObject/Enemy/Nut.h"
#    include "GameObject/Player.h"
#    include "GameObject/Terrain/StaticMap.h"
#    include "GameObject/Terrain/Bonus.h"
#    include "GameObject/Terrain/Brick.h"
#    include "GameObject/Terrain/Checkpoint.h"
#    include "GameObject/Terrain/LevelEnd.h"
#    include "GameObject/Terrain/MovingPlatform.h"
#  include "GameObject/UI/UIObject.h"
#    include "GameObject/UI/Canvas.h"
#       include "GameObject/UI/LevelUI/LevelCanvas.h"
#       include "GameObject/UI/TitleUI/TitleCanvas.h"
#    include "GameObject/UI/Text.h"
#    include "GameObject/UI/Button.h"
#    include "GameObject/UI/Image.h"
#    include "GameObject/UI/LevelUI/SceneStatsUI.h"
#    include "GameObject/UI/LevelUI/LevelHeader.h"
#    include "GameObject/UI/LevelUI/PauseMenu.h"
#    include "GameObject/UI/LevelUI/PauseControls.h"
#    include "GameObject/UI/TitleUI/StartScreen.h"
#    include "GameObject/UI/TitleUI/LevelSelection.h"
#  include "GameObject/SceneManager/LevelManager.h"
#  include "GameObject/SceneManager/TitleManager.h"

const InitFct SPW_Initializers[] = {
    Class_InitObject, Class_InitClass,
    Class_InitScene, Class_InitLevelScene, Class_InitTitleScene,
    Class_InitInputGroup, Class_InitAppInput, Class_InitControlsInput,
    Class_InitDebugInput, Class_InitMouseInput,
    Class_InitGameObject, Class_InitGameBody,
    Class_InitPlayer, Class_InitCanvas, Class_InitLevelCanvas, Class_InitBackground,
    Class_InitCamera, Class_InitMainCamera, Class_InitUserCamera, Class_InitTitleCamera,
    Class_InitStaticMap, Class_InitBonus, Class_InitBrick, Class_InitMovingPlatform,
    Class_InitCheckpoint, Class_InitLevelEnd,
    Class_InitCollectable, Class_InitFirefly, Class_InitHeart,
    Class_InitEnemy, Class_InitNut,
    Class_InitUIObject, Class_InitText, Class_InitButton, Class_InitImage,
    Class_InitLevelHeader, Class_InitSceneStatsUI, Class_InitPauseMenu, Class_InitPauseControls,
    Class_InitTitleCanvas, Class_InitStartScreen, Class_InitLevelSelection,
    Class_InitLevelManager, Class_InitTitleManager
};

void SPW_Init()
{
    int exitStatus = RE_Init(SDL_INIT_VIDEO, IMG_INIT_PNG);
    if (exitStatus != EXIT_SUCCESS)
    {
        assert(false);
        abort();
    }

    if (TTF_Init() == -1)
    {
        printf("ERROR - TTF_Init %s\n", IMG_GetError());
        assert(false);
        abort();
    }

    // Initialise les classes du jeu
    int initCount = sizeof(SPW_Initializers) / sizeof(InitFct);
    for (int i = 0; i < initCount; ++i)
    {
        (SPW_Initializers[i])();
    }
}

void SPW_Quit()
{
    TTF_Quit();
    RE_Quit();
}
