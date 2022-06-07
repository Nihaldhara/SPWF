#include "Button.h"
#include "../../Scene/Scene.h"

// Object virtual methods
void Button_VM_Destructor(void *self);

// GameObject virtual methods
void Button_VM_Render(void *self);
void Button_VM_Update(void *self);

static ButtonClass _Class_Button = { 0 };
const void *const Class_Button = &_Class_Button;

void Class_InitButton()
{
    if (!Class_IsInitialized(Class_Button))
    {
        Class_InitUIObject();

        void *self = (void *)Class_Button;
        ClassCtorParams params = {
            .self = self,
            .super = Class_UIObject,
            .name = "Button",
            .instanceSize = sizeof(Button),
            .classSize = sizeof(ButtonClass)
        };
        Class_Constructor(params, Button_VM_Destructor);
        ((GameObjectClass *)self)->Render = Button_VM_Render;
        ((GameObjectClass *)self)->Update = Button_VM_Update;
    }
}

void Button_Constructor(void *self, void *scene, RE_AtlasPart *atlasPart)
{
    assert(self && scene && atlasPart);

    UIObject_Constructor(self, scene);
    Object_SetClass(self, Class_Button);

    Button *button = Object_Cast(self, Class_Button);

    button->m_atlasPart = atlasPart;
    button->m_exec = NULL;
    button->m_execData = NULL;
    button->m_onStateChanged = NULL;
    button->m_onStateChangedData = NULL;
    button->m_state = BUTTON_UP;
    button->m_prevState = BUTTON_UP;
    button->m_stateChanged = false;
    button->m_hasBorders = false;
    button->m_textAnchor = RE_ANCHOR_CENTER;
    button->m_textOffset = 0.0f;

    for (int i = 0; i < BUTTON_STATE_COUNT; ++i)
    {
        button->m_texts[i] = NULL;
    }

    UIRect rect = { 0 };
    rect.m_anchorMax.x = 1.0f;
    rect.m_anchorMax.y = 1.0f;
    UIObject_SetLocalRect(self, rect);

    SDL_Color gColor = { .r = 255, .g = 255, .b = 255, .a = 255 };
    UIObject_SetGizmosColor(self, gColor);
}

void Button_SetActive(void *self, bool isActive)
{
    Button *button = Object_Cast(self, Class_Button);
    if (isActive == (button->m_state != BUTTON_DISABLED))
    {
        // Le boutton est déjà dans l'état souhaité
        return;
    }

    if (isActive)
    {
        // Active le bouton
        button->m_state = BUTTON_UP;
    }
    else
    {
        // Désactive le bouton
        button->m_state = BUTTON_DISABLED;
        if (button->m_onStateChanged)
        {
            // Callback "OnStateChanged"
            (button->m_onStateChanged)(button, button->m_state, button->m_onStateChangedData);
        }

    }
}

void Button_SetBorders(void *self, UIBorders *borders)
{
    Button *button = Object_Cast(self, Class_Button);
    if (borders)
    {
        button->m_hasBorders = true;
        button->m_borders = *borders;
    }
    else
    {
        button->m_hasBorders = false;
    }
}

void Button_SetTextAnchor(void *self, int anchor, float offset)
{
    Button *button = Object_Cast(self, Class_Button);
    button->m_textAnchor =
        anchor & (RE_ANCHOR_LEFT | RE_ANCHOR_RIGHT | RE_ANCHOR_CENTER);
    button->m_textOffset = offset;
}

void Button_SetText(void *self, char *str, TTF_Font *font, SDL_Color color)
{
    Button_SetTextUp(self, str, font, color);
    Button_SetTextHover(self, str, font, color);
    Button_SetTextDown(self, str, font, color);
    Button_SetTextDisabled(self, str, font, color);
}

void Button_SetTextUp(void *self, char *str, TTF_Font *font, SDL_Color color)
{
    Button *button = Object_Cast(self, Class_Button);
    SDL_Renderer *renderer = Scene_GetRenderer(GameObject_GetScene(self));
    button->m_texts[BUTTON_UP] = RE_Text_New(renderer, font, str, color);
    AssertNew(button->m_texts[BUTTON_UP]);
}

void Button_SetTextHover(void *self, char *str, TTF_Font *font, SDL_Color color)
{
    Button *button = Object_Cast(self, Class_Button);
    SDL_Renderer *renderer = Scene_GetRenderer(GameObject_GetScene(self));
    button->m_texts[BUTTON_HOVER] = RE_Text_New(renderer, font, str, color);
    AssertNew(button->m_texts[BUTTON_HOVER]);
}

void Button_SetTextDown(void *self, char *str, TTF_Font *font, SDL_Color color)
{
    Button *button = Object_Cast(self, Class_Button);
    SDL_Renderer *renderer = Scene_GetRenderer(GameObject_GetScene(self));
    button->m_texts[BUTTON_DOWN] = RE_Text_New(renderer, font, str, color);
    AssertNew(button->m_texts[BUTTON_DOWN]);
}

void Button_SetTextDisabled(void *self, char *str, TTF_Font *font, SDL_Color color)
{
    Button *button = Object_Cast(self, Class_Button);
    SDL_Renderer *renderer = Scene_GetRenderer(GameObject_GetScene(self));
    button->m_texts[BUTTON_DISABLED] = RE_Text_New(renderer, font, str, color);
    AssertNew(button->m_texts[BUTTON_DISABLED]);
}

void Button_VM_Destructor(void *self)
{
    Button *button = Object_Cast(self, Class_Button);

    for (int i = 0; i < BUTTON_STATE_COUNT; ++i)
    {
        RE_Text_Delete(button->m_texts[i]);
    }

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Button);
}

void Button_VM_Update(void *self)
{
    Button *button = Object_Cast(self, Class_Button);
    Scene *scene = GameObject_GetScene(self);
    InputManager *inputManager = Scene_GetInputManager(scene);
    MouseInput *mouse = InputManager_GetMouse(inputManager);

    if (button->m_state == BUTTON_DISABLED)
    {
        button->m_stateChanged = false;
        button->m_prevState = BUTTON_DISABLED;
        return;
    }

    int state = button->m_state;
    SDL_FPoint mousePos = {
        .x = mouse->viewPos.x,
        .y = mouse->viewPos.y
    };
    SDL_FRect rect = UIObject_GetCanvasRect(self);

    if (RE_FRect_Contains(&rect, &mousePos))
    {
        if (mouse->leftDown)
        {
            button->m_state = BUTTON_DOWN;
        }
        else if (mouse->leftReleased)
        {
            if (button->m_exec)
            {
                // Exécute l'action associée au bouton
                (button->m_exec)(button, button->m_execData);

                // Evite le déclanchement de plusieurs boutons 
                mouse->leftReleased = false;
            }

            button->m_state = BUTTON_UP;
        }
        else
        {
            button->m_state = BUTTON_HOVER;
        }
    }
    else
    {
        button->m_state = BUTTON_UP;
    }

    if (state != button->m_state)
    {
        button->m_stateChanged = true;
        button->m_prevState = state;
        if (button->m_onStateChanged)
        {
            // Callback "OnStateChanged"
            (button->m_onStateChanged)(button, button->m_state, button->m_onStateChangedData);
        }
    }
    else
    {
        button->m_stateChanged = false;
    }
}

void Button_VM_Render(void *self)
{
    Button *button = Object_Cast(self, Class_Button);
    RE_AtlasPart *atlasPart = button->m_atlasPart;
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);
    SDL_Texture *texture = RE_AtlasPart_GetTexture(atlasPart);

    SDL_FRect dstRect = UIObject_GetCanvasRect(self);
    SDL_Rect *srcRect = NULL;

    switch (button->m_state)
    {
    case BUTTON_UP:
        srcRect = RE_AtlasPart_GetSrcRect(atlasPart, 0);
        break;
    case BUTTON_HOVER:
        srcRect = RE_AtlasPart_GetSrcRect(atlasPart, 1);
        break;
    case BUTTON_DOWN:
        srcRect = RE_AtlasPart_GetSrcRect(atlasPart, 2);
        break;
    case BUTTON_DISABLED:
    default:
        srcRect = RE_AtlasPart_GetSrcRect(atlasPart, 3);
        break;
    }

    if (button->m_hasBorders)
    {
        UIBorders borders = button->m_borders;
        RE_RenderCopyBordersF(
            renderer, texture, srcRect, &dstRect, RE_ANCHOR_LEFT | RE_ANCHOR_TOP,
            borders.left, borders.right, borders.top, borders.bottom
        );
    }
    else
    {
        SDL_RenderCopyF(renderer, texture, srcRect, &dstRect);
    }

    RE_Text *text = button->m_texts[button->m_state];
    if (text)
    {
        int w, h;
        SDL_Texture *textTexture = RE_Text_GetTexture(text);
        SDL_QueryTexture(textTexture, NULL, NULL, &w, &h);
        
        switch (button->m_textAnchor)
        {
        case RE_ANCHOR_LEFT:
            dstRect.x += button->m_textOffset;
            break;
        case RE_ANCHOR_RIGHT:
            dstRect.x += dstRect.w - button->m_textOffset;
            break;
        case RE_ANCHOR_CENTER:
        default:
            dstRect.x += 0.5f * dstRect.w;
            break;
        }
        dstRect.y += 0.5f * dstRect.h;

        dstRect.w = (float)w;
        dstRect.h = (float)h;

        RE_RenderCopyF(renderer, textTexture, NULL, &dstRect, RE_ANCHOR_MIDDLE | button->m_textAnchor);
    }
}
