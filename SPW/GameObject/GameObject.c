#include "GameObject.h"
#include "../Scene/Scene.h"

// Object virtual methods
void GameObject_VM_Destructor(void *self);

// GameObject virtual methods
void GameObject_VM_DrawGizmos(void *self);
void GameObject_VM_FixedUpdate(void *self);
void GameObject_VM_OnDisable(void *self);
void GameObject_VM_OnEnable(void *self);
void GameObject_VM_OnRespawn(void *self);
void GameObject_VM_Render(void *self);
void GameObject_VM_Start(void *self);
void GameObject_VM_Update(void *self);

static GameObjectClass _Class_GameObject = { 0 };
const void *const Class_GameObject = &_Class_GameObject;

void Class_InitGameObject()
{
    if (!Class_IsInitialized(Class_GameObject))
    {
        Class_InitObject();

        void *self = (void *)Class_GameObject;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Object,
            .name = "GameObject",
            .instanceSize = sizeof(GameObject),
            .classSize = sizeof(GameObjectClass)
        };
        Class_Constructor(params, GameObject_VM_Destructor);
        ((GameObjectClass *)self)->DrawGizmos = GameObject_VM_DrawGizmos;
        ((GameObjectClass *)self)->FixedUpdate = GameObject_VM_FixedUpdate;
        ((GameObjectClass *)self)->OnDisable = GameObject_VM_OnDisable;
        ((GameObjectClass *)self)->OnEnable = GameObject_VM_OnEnable;
        ((GameObjectClass *)self)->OnRespawn = GameObject_VM_OnRespawn;
        ((GameObjectClass *)self)->Render = GameObject_VM_Render;
        ((GameObjectClass *)self)->Start = GameObject_VM_Start;
        ((GameObjectClass *)self)->Update = GameObject_VM_Update;
    }
}

void GameObject_Constructor(void *self, void *scene, int layer)
{
    Object_Constructor(self);
    Object_SetClass(self, Class_GameObject);

    GameObject *object = Object_Cast(self, Class_GameObject);

    object->m_scene = Object_Cast(scene, Class_Scene);
    object->m_layer = layer;
    object->m_flags = 0;
    object->m_state = GAME_OBJECT_TO_START;
    object->m_enabled = true;

    object->m_parent = NULL;
    object->m_children = PVector_New(2);
    AssertNew(object->m_children);

    ObjectManager *objManager = Scene_GetObjectManager(scene);
    ObjectManager_StartObject(objManager, object);
}

static void GameObject_AddChild(void *self, void *child)
{
    assert(self && child);
    GameObject *object = Object_Cast(self, Class_GameObject);
    GameObject *childObj = Object_Cast(child, Class_GameObject);

    // Vérifie que child n'est pas déjà parmi les enfants
    PVector *children = object->m_children;
    int childCount = PVector_GetSize(children);
    for (int i = 0; i < childCount; i++)
    {
        if (child == PVector_Get(children, i))
        {
            return;
        }
    }

    // Ajoute child à la liste des enfants
    PVector_PushBack(children, child);

    // Définit le parent de child
    childObj->m_parent = object;
}

static void GameObject_RemoveChild(void* self, void* child)
{
    assert(self && child);
    GameObject *object = Object_Cast(self, Class_GameObject);
    GameObject *childObj = Object_Cast(child, Class_GameObject);

    PVector *children = object->m_children;
    int childCount = PVector_GetSize(children);
    bool found = false;
    for (int i = 0; i < childCount; i++)
    {
        if (child == PVector_Get(children, i))
        {
            PVector_Pop(children, i);
            found = true;
            break;
        }
    }
    if (!found)
    {
        assert(false);
        return;
    }

    childObj->m_parent = NULL;
}

void GameObject_SetParent(void *self, void *parent)
{
    assert(self);
    GameObject *object = Object_Cast(self, Class_GameObject);

    if (object->m_parent)
    {
        GameObject_RemoveChild(object->m_parent, self);
    }

    if (parent)
    {
        GameObject_AddChild(parent, self);
    }
}

int GameObject_GetChildCount(void *self)
{
    GameObject *object = Object_Cast(self, Class_GameObject);
    return PVector_GetSize(object->m_children);
}

void **GameObject_GetChildren(void *self)
{
    GameObject *object = Object_Cast(self, Class_GameObject);
    return PVector_ToArray(object->m_children);
}

void **GameObject_CopyChildren(void *self)
{
    GameObject *object = Object_Cast(self, Class_GameObject);
    return PVector_CopyArray(object->m_children);
}

void GameObject_VM_Destructor(void *self)
{
    GameObject *object = Object_Cast(self, Class_GameObject);

    // C'est la scene qui s'occupe de supprimer les objets fils
    PVector_Delete(object->m_children);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_GameObject);
}

void GameObject_VM_Update(void *self)
{
}

void GameObject_VM_FixedUpdate(void *self)
{
}

void GameObject_VM_Render(void *self)
{
}

void GameObject_VM_DrawGizmos(void *self)
{
}

void GameObject_VM_OnEnable(void *self)
{
}

void GameObject_VM_OnDisable(void *self)
{
}

void GameObject_VM_OnRespawn(void *self)
{
}

void GameObject_VM_Start(void *self)
{
}