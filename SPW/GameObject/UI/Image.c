#include "Image.h"
#include "../../Scene/Scene.h"

// Object virtual methods
void Image_VM_Destructor(void *self);

// GameObject virtual methods
void Image_VM_Render(void *self);

static ImageClass _Class_Image = { 0 };
const void *const Class_Image = &_Class_Image;

void Class_InitImage()
{
    if (!Class_IsInitialized(Class_Image))
    {
        Class_InitUIObject();

        void *self = (void *)Class_Image;
        ClassCtorParams params = {
            .self = self,
            .super = Class_UIObject,
            .name = "Image",
            .instanceSize = sizeof(Image),
            .classSize = sizeof(ImageClass)
        };
        Class_Constructor(params, Image_VM_Destructor);
        ((GameObjectClass *)self)->Render = Image_VM_Render;
    }
}

void Image_Constructor(void *self, void *scene, RE_AtlasPart *part, int index)
{
    UIObject_Constructor(self, scene);
    Object_SetClass(self, Class_Image);

    Image *image = Object_Cast(self, Class_Image);
    image->m_hasBorders = false;
    image->m_texture = RE_AtlasPart_GetTexture(part);
    image->m_srcRect = RE_AtlasPart_GetSrcRect(part, index);

    UIRect rect = { 0 };
    rect.m_anchorMax.x = 1.0f;
    rect.m_anchorMax.y = 1.0f;
    UIObject_SetLocalRect(self, rect);

    SDL_Color gColor = { .r = 255, .g = 255, .b = 255, .a = 255 };
    UIObject_SetGizmosColor(self, gColor);

}

void Image_SetBorders(void *self, UIBorders *borders)
{
    Image *image = Object_Cast(self, Class_Image);
    if (borders)
    {
        image->m_hasBorders = true;
        image->m_borders = *borders;
    }
    else
    {
        image->m_hasBorders = false;
    }
}

void Image_VM_Destructor(void *self)
{
    Image *image = Object_Cast(self, Class_Image);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Image);
}

void Image_VM_Render(void *self)
{
    Image *image = Object_Cast(self, Class_Image);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = Scene_GetRenderer(scene);
    SDL_Rect *srcRect = image->m_srcRect;
    SDL_FRect dstRect = UIObject_GetCanvasRect(self);

    if (image->m_hasBorders)
    {
        UIBorders borders = image->m_borders;
        RE_RenderCopyBordersF(
            renderer, image->m_texture, srcRect, &dstRect, RE_ANCHOR_LEFT | RE_ANCHOR_TOP,
            borders.left, borders.right, borders.top, borders.bottom
        );
    }
    else
    {
        SDL_RenderCopyF(renderer, image->m_texture, srcRect, &dstRect);
    }
}