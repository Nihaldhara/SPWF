#include "Text.h"
#include "../../Scene/Scene.h"

// Object virtual methods
void Text_VM_Destructor(void *self);

// GameObject virtual methods
void Text_VM_Render(void *self);

static TextClass _Class_Text = { 0 };
const void *const Class_Text = &_Class_Text;

void Class_InitText()
{
    if (!Class_IsInitialized(Class_Text))
    {
        Class_InitUIObject();

        void *self = (void *)Class_Text;
        ClassCtorParams params = {
            .self = self,
            .super = Class_UIObject,
            .name = "Text",
            .instanceSize = sizeof(Text),
            .classSize = sizeof(TextClass)
        };
        Class_Constructor(params, Text_VM_Destructor);
        ((GameObjectClass *)self)->Render = Text_VM_Render;
    }
}

void Text_Constructor(void *self, void *scene, char *str, TTF_Font *font, SDL_Color color)
{
    assert(strlen(str) > 0 && font);

    UIObject_Constructor(self, scene);
    Object_SetClass(self, Class_Text);

    Text *text = Object_Cast(self, Class_Text);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);

    UIRect rect = { 0 };
    rect.m_anchorMax.x = 1.0f;
    rect.m_anchorMax.y = 1.0f;
    UIObject_SetLocalRect(self, rect);

    SDL_Color gColor = { .r = 0, .g = 200, .b = 255, .a = 255 };
    UIObject_SetGizmosColor(self, gColor);
    UIObject_SetGizmosFillOpacity(self, 64);

    text->m_text = RE_Text_New(renderer, font, str, color);
    AssertNew(text->m_text);
}

void Text_VM_Destructor(void *self)
{
    Text *text = Object_Cast(self, Class_Text);

    RE_Text_Delete(text->m_text);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Text);
}

void Text_VM_Render(void *self)
{
    Text *text = Object_Cast(self, Class_Text);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);
    SDL_FRect dstRect = UIObject_GetCanvasRect(self);
    SDL_Texture *texture = RE_Text_GetTexture(text->m_text);

    if (text->m_stretch)
    {
        SDL_RenderCopyF(renderer, texture, NULL, &dstRect);
    }
    else
    {
        int anchor = text->m_anchor;
        switch (anchor & 0x03)
        {
        case RE_ANCHOR_CENTER:
            dstRect.x += 0.5f * dstRect.w;
            break;
        case RE_ANCHOR_RIGHT:
            dstRect.x += dstRect.w;
            break;
        case RE_ANCHOR_LEFT:
        default:
            break;
        }
        switch (anchor & 0x30)
        {
        case RE_ANCHOR_MIDDLE:
            dstRect.y += 0.5f * dstRect.h;
            break;
        case RE_ANCHOR_BOTTOM:
            dstRect.y += dstRect.h;
            break;
        case RE_ANCHOR_TOP:
        default:
            break;
        }

        int w, h;
        Text_GetNaturalSize(self, &w, &h);
        dstRect.h = (float)h;
        dstRect.w = (float)w;

        RE_RenderCopyF(renderer, texture, NULL, &dstRect, anchor);
    }
}