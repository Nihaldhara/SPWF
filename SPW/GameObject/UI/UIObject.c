#include "UIObject.h"
#include "../../Scene/Scene.h"

// Object virtual methods
void UIObject_VM_Destructor(void *self);

// GameObject virtual methods
void UIObject_VM_DrawGizmos(void *self);

static UIObjectClass _Class_UIObject = { 0 };
const void *const Class_UIObject = &_Class_UIObject;

void Class_InitUIObject()
{
    if (!Class_IsInitialized(Class_UIObject))
    {
        Class_InitGameObject();

        void *self = (void *)Class_UIObject;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameObject,
            .name = "UIObject",
            .instanceSize = sizeof(UIObject),
            .classSize = sizeof(UIObjectClass)
        };
        Class_Constructor(params, UIObject_VM_Destructor);
        ((GameObjectClass *)self)->DrawGizmos = UIObject_VM_DrawGizmos;
    }
}

void UIObject_Constructor(void *self, void *scene)
{
    GameObject_Constructor(self, scene, LAYER_UI);
    Object_SetClass(self, Class_UIObject);

    UIObject *ui = Object_Cast(self, Class_UIObject);

    UIRect rect = { 0 };
    rect.m_anchorMax.x = 1.0f;
    rect.m_anchorMax.y = 1.0f;

    SDL_Color color = { .r = 255, .g = 200, .b = 0, .a = 255 };
    ui->m_gizmosColor = color;
    ui->m_gizmosAlpha = 32;

    ui->m_rect = rect;
}

SDL_FRect UIObject_GetCanvasRect(void *self)
{
    UIObject *ui = Object_Cast(self, Class_UIObject);
    void *parent = GameObject_GetParent(self);
    UIRect *rect = &ui->m_rect;

    if (Object_IsA(parent, Class_UIObject))
    {
        SDL_FRect pRect = UIObject_GetCanvasRect(parent);
        float lowerX = pRect.x + pRect.w * rect->m_anchorMin.x + rect->m_offsetMin.x;
        float lowerY = pRect.y + pRect.h * rect->m_anchorMin.y + rect->m_offsetMin.y;
        float upperX = pRect.x + pRect.w * rect->m_anchorMax.x + rect->m_offsetMax.x;
        float upperY = pRect.y + pRect.h * rect->m_anchorMax.y + rect->m_offsetMax.y;

        SDL_FRect res = {
            .x = lowerX,
            .y = lowerY,
            .w = fabsf(upperX - lowerX),
            .h = fabsf(upperY - lowerY)
        };
        return res;
    }
    else
    {
        SDL_FRect res = {
            .x = rect->m_offsetMin.x,
            .y = rect->m_offsetMin.y,
            .w = fabsf(rect->m_offsetMax.x - rect->m_offsetMin.x),
            .h = fabsf(rect->m_offsetMax.y - rect->m_offsetMin.y)
        };
        return res;
    }
}

void UIObject_VM_Destructor(void *self)
{
    UIObject *ui = Object_Cast(self, Class_UIObject);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_UIObject);
}

void UIObject_VM_DrawGizmos(void *self)
{
    UIObject *ui = Object_Cast(self, Class_UIObject);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);

    SDL_FRect rect = UIObject_GetCanvasRect(self);
    SDL_Color color = ui->m_gizmosColor;
    int alpha = ui->m_gizmosAlpha;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (alpha > 0)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
        SDL_RenderFillRectF(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRectF(renderer, &rect);

    int childCount = GameObject_GetChildCount(self);
    void **children = GameObject_GetChildren(self);
    for (int i = 0; i < childCount; i++)
    {
        GameObject_DrawGizmos(children[i]);
    }
}