#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_

#include "../Settings.h"
#include "../GameObject/GameObject.h"
#include "List.h"
#include "PVector.h"

typedef struct ObjectManager_s
{
    PE_World *m_world;
    PVector *m_visibleObjects;
    List m_objectList;
    PVector *m_toProcess;
} ObjectManager;

ObjectManager *ObjectManager_New(PE_World *world);

void ObjectManager_Delete(void *self);

void *ObjectManager_AllocateObject(void *self, int size);

void ObjectManager_DeleteObject(void *self, void *object);

void ObjectManager_EnableObject(void *self, void *object);

void ObjectManager_DisableObject(void *self, void *object);

void ObjectManager_SetVisible(void *self, void *object);

void ObjectManager_SetToRespawn(void *self, void *object, bool toRespawn);

void ObjectManager_StartObject(void *self, void *object);

void ObjectManager_AddVisibleBodies(void *self, PE_AABB aabb);

INLINE int ObjectManager_GetVisibleCount(ObjectManager *self)
{
    return PVector_GetSize(self->m_visibleObjects);
}

GameObject **ObjectManager_GetVisibleObjects(void *self);

void ObjectManager_ClearVisibleObjects(void *self);

void ObjectManager_ProcessObjects(void *self);

void ObjectManager_RespawnObjects(void *self);

INLINE void ObjectManager_GetObjectIterator(ObjectManager *self, ListIterator *it)
{
    List_GetIterator(&self->m_objectList, it);
}

INLINE int ObjectManager_GetObjectCount(ObjectManager *self)
{
    return List_GetNodeCount(&self->m_objectList);
}

#endif
