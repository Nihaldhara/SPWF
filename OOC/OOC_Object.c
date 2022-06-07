#include "OOC_Object.h"

#include <string.h>

void Object_VM_Destructor(void *self);
void Class_VM_Destructor(void *self);

static Class _Class_Object = { 0 };
static Class _Class_Class = { 0 };
const void *const Class_Object = &_Class_Object;
const void *const Class_Class = &_Class_Class;

#define STRUCT_OFFSET_OF(_struct_, _member_)  (size_t)&(((_struct_ *)0)->_member_)

void Class_InitObject()
{
    if (!Class_IsInitialized(Class_Object))
    {
        // Initialisation différente car c'est la classe dont toutes les autres classes héritent
        void *self = (void *)&_Class_Object;

        ((Object *)self)->m_magic = OBJECT_MAGIC;
        ((Object *)self)->m_clsDesc = self;

        ((Class *)self)->m_name = "Object";
        ((Class *)self)->m_instanceSize = sizeof(Object);
        ((Class *)self)->m_classSize = sizeof(Class);
        ((Class *)self)->m_super = self;
        ((Class *)self)->m_Destructor = Object_VM_Destructor;

        Class_InitClass();
    }
}

void Class_InitClass()
{
    if (!Class_IsInitialized(Class_Class))
    {
        Class_InitObject();

        void *self = (void *)Class_Class;
        ClassCtorParams params = {
            .self = self,
            .super = Class_Object,
            .name = "Class",
            .instanceSize = sizeof(Class),
            .classSize = sizeof(Class)
        };
        Class_Constructor(params, Class_VM_Destructor);
    }
}

static bool Object_Check(const void *self)
{
    return (self != NULL) && ((Object *)self)->m_magic == OBJECT_MAGIC;
}

void Class_Constructor(ClassCtorParams params, void(*Destructor)(void *))
{
    void *self = params.self;
    const Class *super = params.super;

    assert(self && super && Destructor);

    size_t classSize = params.classSize;
    size_t superSize = super->m_classSize;

    // La taille d'une classe dérivée est supérieure à celle de la classe de base
    assert(classSize >= superSize);

    // Copie la class mère
    // Permet des copier toutes les méthodes virtuelles ajoutées par héritage
#ifdef _WIN32
    memcpy_s(self, classSize, super, superSize);
#else
    memcpy(self, super, superSize);
#endif

    // Définit l'objet
    ((Object *)self)->m_magic = OBJECT_MAGIC;
    ((Object *)self)->m_clsDesc = self;

    // Définit la classe
    ((Class *)self)->m_name = params.name;
    ((Class *)self)->m_instanceSize = params.instanceSize;
    ((Class *)self)->m_classSize = classSize;
    ((Class *)self)->m_super = super;
    ((Class *)self)->m_Destructor = Destructor;
}

void Object_Constructor(void *self)
{
    assert(self);
    ((Object *)self)->m_magic = OBJECT_MAGIC;
    Object_SetClass(self, Class_Object);
}

void Object_VM_Destructor(void *self)
{
    Object *object = Object_Cast(self, Class_Object);
    object->m_magic = 0LL;
};

void Class_VM_Destructor(void *self)
{
    assert(false);
};

bool Class_IsInitialized(const void *clsDesc)
{
    if (!Object_Check(clsDesc) || !Object_IsClass(clsDesc))
        return false;

    // Vérification de la vtable
    const size_t classSize = ((Class *)clsDesc)->m_classSize;
    const size_t headerSize = STRUCT_OFFSET_OF(Class, m_Destructor);
    const size_t vtableSize = classSize - headerSize;
    const size_t ptrSize = sizeof(void *);

    if (vtableSize % ptrSize)
        return false;

    const size_t vmCount = vtableSize / ptrSize;
    for (size_t i = 0; i < vmCount; ++i)
    {
        void **ptr = (void **)((char *)clsDesc + headerSize + i * ptrSize);
        if (*ptr == NULL)
            return false;
    }

    return true;
}

bool Object_IsA(const void *self, const void *clsDesc)
{
    if (self)
    {
        assert(Object_Check(self));

        if (clsDesc == Class_Object)
        {
            // Tous les objets héritent de "Object"
            return true;
        }
        else
        {
            // On remonte la chaîne d'héritage de self pour trouver clsDesc
            const Class *cls = Object_GetClass(self);
            while (cls != clsDesc)
            {
                if (cls == Class_Object)
                {
                    // On vient de remonter la chaîne d'héritage sans trouver clsDesc
                    return false;
                }

                cls = Object_GetSuper(cls);
            }
            return true;
        }
    }

    // Pointeur NULL
    return false;
}

bool Object_IsClass(const void *self)
{
    assert(Object_Check(self));
    const Object *obj = self;
    return obj->m_clsDesc == self;
}
