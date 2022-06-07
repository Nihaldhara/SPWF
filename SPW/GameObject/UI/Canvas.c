#include "Canvas.h"
#include "../../Scene/Scene.h"

// Object virtual methods
void Canvas_VM_Destructor(void *self);

// GameObject virtual methods
void Canvas_VM_DrawGizmos(void *self);
void Canvas_VM_Render(void *self);
void Canvas_VM_Update(void *self);

static CanvasClass _Class_Canvas = { 0 };
const void *const Class_Canvas = &_Class_Canvas;

void Class_InitCanvas()
{
    if (!Class_IsInitialized(Class_Canvas))
    {
        Class_InitUIObject();

        void *self = (void *)Class_Canvas;
        ClassCtorParams params = {
            .self = self,
            .super = Class_UIObject,
            .name = "Canvas",
            .instanceSize = sizeof(Canvas),
            .classSize = sizeof(CanvasClass)
        };
        Class_Constructor(params, Canvas_VM_Destructor);
        ((GameObjectClass *)self)->DrawGizmos = Canvas_VM_DrawGizmos;
        ((GameObjectClass *)self)->Render = Canvas_VM_Render;
        ((GameObjectClass *)self)->Update = Canvas_VM_Update;
    }
}

void Canvas_Constructor(void *self, void *scene)
{
    UIObject_Constructor(self, scene);
    Object_SetClass(self, Class_Canvas);

    Canvas *canvas = Object_Cast(self, Class_Canvas);

    canvas->m_objects = PVector_New(32);
    AssertNew(canvas->m_objects);

    UIObject_SetGizmosFillOpacity(canvas, 0);

    SDL_Renderer *renderer = Scene_GetRenderer(scene);
    int width, height;
    SDL_RenderGetLogicalSize(renderer, &width, &height);

    UIRect rect = { 0 };
    rect.m_offsetMin.x = 0;
    rect.m_offsetMin.y = 0;
    rect.m_offsetMax.x = (float)width;
    rect.m_offsetMax.y = (float)height;

    UIObject_SetLocalRect(canvas, rect);
}

void Canvas_VM_Destructor(void *self)
{
    Canvas *canvas = Object_Cast(self, Class_Canvas);

    PVector_Delete(canvas->m_objects);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_Canvas);
}

static void Canvas_FlattenChildren(PVector *pVector, void *parent)
{
    int childCount = GameObject_GetChildCount(parent);
    void **children = GameObject_GetChildren(parent);

    for (int i = 0; i < childCount; ++i)
    {
        PVector_PushBack(pVector, children[i]);
        if (GameObject_GetChildCount(children[i]) > 0)
        {
            Canvas_FlattenChildren(pVector, children[i]);
        }
    }
}

static int Canvas_CompareFct(const void *eltA, const void *eltB)
{
    GameObject *objectA = *(GameObject **)eltA;
    GameObject *objectB = *(GameObject **)eltB;
    return objectA->m_layer - objectB->m_layer;
}

void Canvas_VM_Render(void *self)
{
    Canvas *canvas = Object_Cast(self, Class_Canvas);
    PVector *objects = canvas->m_objects;

    PVector_Clear(objects);
    Canvas_FlattenChildren(objects, self);

    // Trie les objets selon leurs layers
    int size = PVector_GetSize(objects);
    void **data = PVector_ToArray(objects);
    qsort((void *)data, size, sizeof(void *), Canvas_CompareFct);

    // Calcule le rendu des objets
    for (int i = 0; i < size; ++i)
    {
        void *gameObject = data[i];
        if (GameObject_IsEnabled(gameObject))
        {
            GameObject_Render(gameObject);
        }
    }
}

void Canvas_VM_Update(void *self)
{
    Canvas *canvas = Object_Cast(self, Class_Canvas);
    Scene *scene = GameObject_GetScene(canvas);
    Scene_SetVisible(scene, canvas);
}

void Canvas_VM_DrawGizmos(void *self)
{
    Canvas *canvas = Object_Cast(self, Class_Canvas);
    PVector *objects = canvas->m_objects;

    int size = PVector_GetSize(objects);
    for (int i = 0; i < size; ++i)
    {
        void *gameObject = PVector_Get(objects, i);
        GameObject_DrawGizmos(gameObject);
    }
}