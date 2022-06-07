
#ifndef _RE_TEXT_H_
#define _RE_TEXT_H_

#include "RE_Settings.h"
#include "RE_Utils.h"


typedef struct RE_Text_s
{
    char *m_str;
    TTF_Font *m_font;
    SDL_Color m_color;
    SDL_Texture *m_texture;
    SDL_Renderer *m_renderer;
} RE_Text;

RE_Text *RE_Text_New(SDL_Renderer *renderer, TTF_Font *font, char *str, SDL_Color color);
void RE_Text_Delete(RE_Text *text);

RE_INLINE void RE_Text_SetFont(RE_Text *text, TTF_Font *font)
{
    text->m_font = font;
}

void RE_Text_SetString(RE_Text *text, char *str);
void RE_Text_SetColor(RE_Text *text, SDL_Color color);

RE_INLINE char *RE_Text_GetString(RE_Text *text)
{
    return text->m_str;
}

RE_INLINE SDL_Texture *RE_Text_GetTexture(RE_Text *text)
{
    assert(text && text->m_texture);
    return text->m_texture;
}

#endif