#ifndef _BASE_BUTTON_H_
#define _BASE_BUTTON_H_

#include "UIObject.h"

void Class_InitButton();

/// @brief Description de la classe "Button".
extern const void *const Class_Button;

typedef struct ButtonClass_s {
    const UIObjectClass base;
} ButtonClass;

typedef enum ButtonState_e
{
    BUTTON_UP,
    BUTTON_HOVER,
    BUTTON_DOWN,
    BUTTON_DISABLED,
    //
    BUTTON_STATE_COUNT
} ButtonState;

typedef void (*ButtonExec)(void *self, void *data);
typedef void (*ButtonOnStateChanged)(void *self, int state, void *data);

typedef struct Button_s
{
    struct {
        UIObject base;
    } m_super;

    RE_AtlasPart *m_atlasPart;

    int m_state;
    int m_prevState;
    bool m_stateChanged;

    bool m_hasBorders;
    UIBorders m_borders;

    int m_textAnchor;
    float m_textOffset;

    RE_Text *m_texts[BUTTON_STATE_COUNT];

    // Callbacks
    ButtonExec m_exec;
    void *m_execData;
    ButtonOnStateChanged m_onStateChanged;
    void *m_onStateChangedData;
} Button;

void Button_Constructor(void *self, void *scene, RE_AtlasPart *atlasPart);

INLINE void Button_SetExec(void *self, ButtonExec exec, void *data)
{
    Button *button = Object_Cast(self, Class_Button);
    button->m_exec = exec;
    button->m_execData = data;
}

void Button_SetActive(void *self, bool isActive);

INLINE void Button_SetOnStateChanged(void *self, ButtonOnStateChanged onStateChanged, void *data)
{
    Button *button = Object_Cast(self, Class_Button);
    button->m_onStateChanged = onStateChanged;
    button->m_onStateChangedData = data;
}

void Button_SetBorders(void *self, UIBorders *borders);
void Button_SetTextAnchor(void *self, int anchor, float offset);

void Button_SetText(void *self, char *str, TTF_Font *font, SDL_Color color);

void Button_SetTextUp(void *self, char *str, TTF_Font *font, SDL_Color color);
void Button_SetTextHover(void *self, char *str, TTF_Font *font, SDL_Color color);
void Button_SetTextDown(void *self, char *str, TTF_Font *font, SDL_Color color);
void Button_SetTextDisabled(void *self, char *str, TTF_Font *font, SDL_Color color);

#endif
