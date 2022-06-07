#ifndef _TEXT_H_
#define _TEXT_H_

#include "UIObject.h"

void Class_InitText();

/// @brief Description de la classe "Text".
extern const void *const Class_Text;

typedef struct TextClass_s {
    const UIObjectClass base;
} TextClass;

typedef struct Text_s
{
    struct {
        UIObject base;
    } m_super;

    RE_Text *m_text;
    bool m_stretch;
    int m_anchor;
} Text;

void Text_Constructor(void *self, void *scene, char *str, TTF_Font *font, SDL_Color color);

INLINE void Text_SetString(void *self, char *str)
{
    Text *text = Object_Cast(self, Class_Text);
    RE_Text_SetString(text->m_text, str);
}

INLINE void Text_SetAnchor(void *self, int anchor)
{
    Text *text = Object_Cast(self, Class_Text);
    text->m_anchor = anchor;
    text->m_stretch = false;
}

INLINE void Text_Stretch(void *self)
{
    Text *text = Object_Cast(self, Class_Text);
    text->m_stretch = true;
}

INLINE void Text_GetNaturalSize(void *self, int *w, int *h)
{
    Text *text = Object_Cast(self, Class_Text);
    SDL_Texture *texture = RE_Text_GetTexture(text->m_text);
    SDL_QueryTexture(texture, NULL, NULL, w, h);
}

#endif
