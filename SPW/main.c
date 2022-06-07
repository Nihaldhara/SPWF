
//#define FULLSCREEN
//#define FHD

#define SKIP_MENU
#define DEFAUT_LEVEL 0

#include "Settings.h"

#include "Scene/Scene.h"
#include "Scene/LevelScene.h"
#include "Scene/TitleScene.h"

enum GameState_e
{
    GAME_MAIN_MENU,
    GAME_LEVEL
};

int main(int argc, char *argv[])
{
    SPW_Init();
    LevelData_Init();
    Progress_Init();

    SDL_Window  *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Crée la fenêtre et le moteur de rendu SDL
    int sdlFlags = 0;
#ifdef FULLSCREEN
    sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif

    window = SDL_CreateWindow(
        u8"Super Potoo World 2022", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, sdlFlags
    );

    if (!window)
    {
        printf("ERROR - Create window %s\n", SDL_GetError());
        assert(false); abort();
    }

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    if (!renderer)
    {
        printf("ERROR - Create renderer %s\n", SDL_GetError());
        assert(false); abort();
    }

    // Crée le temps global du jeu
    g_time = RE_Timer_New();
    AssertNew(g_time);

    // Lance le temps global
    RE_Timer_Start(g_time);

    // Niveau par défaut
    int levelIdx = DEFAUT_LEVEL;

    int state = GAME_MAIN_MENU;
#ifdef SKIP_MENU
    state = GAME_LEVEL;
#endif

    // Boucle de jeu
    Scene *scene = NULL;
    bool quitGame = false;
    while (!quitGame)
    {
        RE_Timer_SetTimeScale(g_time, 1.0f);
        if (scene)
        {
            Scene_Delete(scene);
            scene = NULL;
        }

        // Construction de la scène
        switch (state)
        {
        case GAME_LEVEL:
            scene = Object_Allocate(Class_LevelScene);
            AssertNew(scene);
            LevelScene_Constructor(scene, renderer, levelIdx);
            break;

        case GAME_MAIN_MENU:
        default:
            scene = Object_Allocate(Class_TitleScene);
            AssertNew(scene);
            TitleScene_Constructor(scene, renderer);
            break;
        }

        // Boucle de rendu
        bool quitScene = false;
        while (!quitScene)
        {
            // Met à jour le temps
            RE_Timer_Update(g_time);

            // Met à jour la scène
            quitScene = Scene_Update(scene);

            if (quitScene)
                break;

            // Efface le rendu précédent
            SDL_SetRenderDrawColor(renderer, (Uint8)50, (Uint8)50, (Uint8)70, (Uint8)255);
            SDL_RenderClear(renderer);

            // Dessine la scène
            Scene_Render(scene);

            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }

        switch (state)
        {
        case GAME_LEVEL:
            state = GAME_MAIN_MENU;
            break;

        case GAME_MAIN_MENU:
        default:
            levelIdx = TitleScene_GetSelectedLevel(scene);
            if (levelIdx < 0)
            {
                quitGame = true;
            }
            state = GAME_LEVEL;
            break;
        }
    }

    // Libération de la mémoire
    Scene_Delete(scene);
    scene = NULL;

    RE_Timer_Delete(g_time);
    g_time = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SPW_Quit();

    return EXIT_SUCCESS;
}
