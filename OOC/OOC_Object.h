#ifndef _OOC_OBJECT_H_
#define _OOC_OBJECT_H_

#include "OOC_Settings.h"

#define OBJECT_MAGIC 0xEFFE165DABADEEEE

void Class_InitObject();
void Class_InitClass();
extern const void *const Class_Object;
extern const void *const Class_Class;

typedef struct Class_s Class;
typedef struct Object_s Object;

struct Object_s {
    unsigned long long m_magic;
    const Class *m_clsDesc;
};

struct Class_s
{
    const Object m_base;
    const char *m_name;
    size_t m_instanceSize;
    size_t m_classSize;
    const Class *m_super;

    // Virtual table (prolongée par l'héritage)
    void (*m_Destructor)(void *self);
};

void Object_Constructor(void *self);

typedef struct ClassCtorParams_s {
    void *self;
    const char *name;
    const void *super;
    size_t instanceSize;
    size_t classSize;
} ClassCtorParams;

void Class_Constructor(ClassCtorParams params, void (*Destructor)(void *));

bool Class_IsInitialized(const void * clsDesc);


/// @brief Ne fonctionne pas pour Class_Class, il faut utiliser Object_IsClass().
/// @param self 
/// @param clsDesc 
/// @return 
bool Object_IsA(const void *self, const void *clsDesc);

/// @brief Indique si l'objet est une classe.
/// @param self 
/// @return 
bool Object_IsClass(const void *self);

/// @brief Ne permet pas de caster un pointeur NULL.
/// En débug, vérifie que self n'est pas NULL, que sa classe est une classe dérivée de clsDesc.
/// En particulier, si clsDesc est NULL, le cast échoue.
/// @param self 
/// @param clsDesc 
/// @return 
OOC_INLINE void *Object_Cast(const void *self, const void *clsDesc)
{
    assert(Object_IsA(self, clsDesc));
    return (void *)self;
}

OOC_INLINE void Object_SetClass(void *self, const void *clsDesc)
{
    assert(Class_IsInitialized(clsDesc));
    Object *selfObject = (Object *)self;
    assert(selfObject && clsDesc);
    selfObject->m_clsDesc = (Class *)clsDesc;
}

OOC_INLINE size_t Object_SizeOf(const void *self)
{
    const Object *selfObject = (Object *)self;
    assert(selfObject && selfObject->m_clsDesc);
    return (selfObject->m_clsDesc)->m_instanceSize;
}

OOC_INLINE const void *Object_GetClass(const void *self)
{
    const Object *selfObject = (Object *)self;
    assert(selfObject && selfObject->m_clsDesc);
    return (void *)(selfObject->m_clsDesc);
}

OOC_INLINE const char *Object_GetClassName(const void *self)
{
    const Class *clsDesc = (const Class *)Object_GetClass(self);
    return clsDesc->m_name;
}

OOC_INLINE const void *Object_GetSuper(const void *self)
{
    const Object *selfObject = (Object *)self;
    assert(selfObject && selfObject->m_clsDesc);
    return (void *)(selfObject->m_clsDesc->m_super);
}

OOC_INLINE void *Object_Allocate(const void *clsDesc)
{
    assert(Class_IsInitialized(clsDesc));
    return calloc(1, Object_SizeOf(clsDesc));
}

OOC_INLINE void *Object_New()
{
    void *self = Object_Allocate(Class_Object);
    assert(self);
    Object_Constructor(self);
    return self;
}

OOC_INLINE void Object_Destroy(void *self)
{
    const Class *clsDesc = (Class *)Object_GetClass(self);
    Object *object = (Object *)Object_Cast(self, Class_Object);
    assert(clsDesc && clsDesc->m_Destructor);
    clsDesc->m_Destructor(object);
}

OOC_INLINE void Object_Delete(void *self)
{
    Object_Destroy(self);
    free(self);
}

OOC_INLINE void Object_SuperDestroy(void *self, const void *clsDesc)
{
    const Class *superClass = (Class *)Object_GetSuper(clsDesc);
    assert(superClass && superClass->m_Destructor);
    superClass->m_Destructor(self);
}

#endif
