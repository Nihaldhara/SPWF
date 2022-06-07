#include "ObjectManager.h"

ObjectManager *ObjectManager_New(PE_World *world)
{
    ObjectManager *objManager = (ObjectManager *)calloc(1, sizeof(ObjectManager));
    AssertNew(objManager);

    objManager->m_world = world;

    List_Init(&objManager->m_objectList);

    objManager->m_toProcess = PVector_New(1 << 10);
    AssertNew(objManager->m_toProcess);

    objManager->m_visibleObjects = PVector_New(1 << 10);
    AssertNew(objManager->m_visibleObjects);

    return objManager;
}

void ObjectManager_Delete(void *self)
{
    if (!self)
    {
        return;
    }

    ObjectManager *objManager = self;

    PVector_Delete(objManager->m_visibleObjects);
    PVector_Delete(objManager->m_toProcess);

    ListIterator it = { 0 };
    List_GetIterator(&objManager->m_objectList, &it);
    bool hasNext = ListIterator_MoveNext(&it);
    while (hasNext)
    {
        GameObject *object = (GameObject *)ListIterator_Current(&it);
        ListNode *node = ListNode_GetNode((void *)object);

        // Avance l'itérateur pour pouvoir détruire l'objet
        hasNext = ListIterator_MoveNext(&it);

        // Libère la mémoire interne de l'objet
        Object_Destroy(object);

        // Supprime l'objet
        ListNode_Delete(node);
    }
}

void *ObjectManager_AllocateObject(void *self, int size)
{
    ObjectManager *objManager = self;

    // Allocation du noeud
    ListNode *node = ListNode_New(size);
    AssertNew(node);

    // Ajout à la liste des objets
    List_PushBack(&objManager->m_objectList, node);

    return ListNode_GetData(node);
}

void ObjectManager_ProcessObjects(void *self)
{
    ObjectManager *objManager = self;
    PVector *objVector = objManager->m_toProcess;
    int count = PVector_GetSize(objVector);
    void **objectArray = PVector_CopyArray(objVector);
    PVector_Clear(objVector);

    for (int i = 0; i < count; ++i)
    {
        GameObject *object = Object_Cast(objectArray[i], Class_GameObject);
        ListNode *node = ListNode_GetNode((void *)object);

        switch (object->m_state)
        {
        case GAME_OBJECT_TO_START:
            GameObject_Start(object);
            GameObject_OnEnable(object);
            object->m_state = GAME_OBJECT_RELAXED;
            break;

        case GAME_OBJECT_TO_DELETE:
            Object_Destroy(object);
            ListNode_Delete(node);
            break;

        case GAME_OBJECT_TO_ENABLE:
            object->m_enabled = true;
            object->m_state = GAME_OBJECT_RELAXED;
            GameObject_OnEnable(object);
            break;

        case GAME_OBJECT_TO_DISABLE:
            object->m_enabled = false;
            object->m_state = GAME_OBJECT_RELAXED;
            GameObject_OnDisable(object);
            break;

        default:
            break;
        }
    }
    free(objectArray);
}

void ObjectManager_RespawnObjects(void *self)
{
    ObjectManager *objManager = self;
    ListIterator it;

    // Réinitialise les objets avec le flag GAME_OBJECT_AUTO_RESPAWN
    List_GetIterator(&objManager->m_objectList, &it);
    while (ListIterator_MoveNext(&it))
    {
        GameObject *object = (GameObject *)ListIterator_Current(&it);

        if (object->m_flags & GAME_OBJECT_AUTO_RESPAWN)
        {
            // Réinitialise l'objet
            if (!object->m_enabled)
            {
                object->m_enabled = true;
                GameObject_OnEnable(object);
            }
            GameObject_OnRespawn(object);
        }
    }
}

void ObjectManager_DeleteObject(void *self, void *object)
{
    ObjectManager *objManager = self;
    GameObject *gameObject = Object_Cast(object, Class_GameObject);

    if (gameObject->m_state == GAME_OBJECT_TO_DELETE)
    {
        return;
    }
    if (gameObject->m_state == GAME_OBJECT_RELAXED)
    {
        PVector_PushBack(objManager->m_toProcess, object);
    }
    gameObject->m_state = GAME_OBJECT_TO_DELETE;

    GameObject_SetParent(object, NULL);
    int childCount = GameObject_GetChildCount(gameObject);
    void **children = GameObject_CopyChildren(gameObject);
    for (int i = 0; i < childCount; i++)
    {
        ObjectManager_DeleteObject(objManager, children[i]);
    }
    free(children);
}

void ObjectManager_EnableObject(void *self, void *object)
{
    ObjectManager *objManager = self;
    GameObject *gameObject = Object_Cast(object, Class_GameObject);

    if ((gameObject->m_enabled == true) ||
        (gameObject->m_state == GAME_OBJECT_TO_ENABLE))
    {
        return;
    }
    if (gameObject->m_state == GAME_OBJECT_RELAXED)
    {
        PVector_PushBack(objManager->m_toProcess, object);
    }
    gameObject->m_state = GAME_OBJECT_TO_ENABLE;
}

void ObjectManager_DisableObject(void *self, void *object)
{
    ObjectManager *objManager = self;
    GameObject *gameObject = Object_Cast(object, Class_GameObject);

    if ((gameObject->m_enabled == false) ||
        (gameObject->m_state == GAME_OBJECT_TO_DISABLE))
    {
        return;
    }
    if (gameObject->m_state == GAME_OBJECT_RELAXED)
    {
        PVector_PushBack(objManager->m_toProcess, object);
    }
    gameObject->m_state = GAME_OBJECT_TO_DISABLE;

    int childCount = GameObject_GetChildCount(gameObject);
    void **children = GameObject_CopyChildren(gameObject);
    for (int i = 0; i < childCount; i++)
    {
        ObjectManager_DisableObject(objManager, children[i]);
    }
    free(children);
}

void ObjectManager_StartObject(void *self, void *object)
{
    ObjectManager *objManager = self;
    GameObject *gameObject = Object_Cast(object, Class_GameObject);

    assert(gameObject->m_state == GAME_OBJECT_TO_START);
    PVector_PushBack(objManager->m_toProcess, object);
}

void ObjectManager_SetToRespawn(void *self, void *object, bool toRespawn)
{
    GameObject *gameObject = Object_Cast(object, Class_GameObject);

    if (toRespawn)
    {
        gameObject->m_flags |= GAME_OBJECT_AUTO_RESPAWN;
    }
    else
    {
        gameObject->m_flags &= ~GAME_OBJECT_AUTO_RESPAWN;
    }
}

static bool ObjectManager_Callback(PE_Collider *collider, void *data)
{
    ObjectManager *objManager = (ObjectManager *)data;
    PE_Body *body = PE_Collider_GetBody(collider);
    GameObject *object = PE_Body_GetUserData(body);

    // Ajoute l'objet aux objects visibles
    ObjectManager_SetVisible(objManager, object);

    // On continue la recherche
    return true;
}

void ObjectManager_AddVisibleBodies(void *self, PE_AABB aabb)
{
    ObjectManager *objManager = self;
    PE_World *world = objManager->m_world;

    PE_World_OverlapAreaExpert(world, &aabb, ObjectManager_Callback, objManager);
}


void ObjectManager_SetVisible(void *self, void *object)
{
    ObjectManager *objManager = self;
    GameObject *gameObject = Object_Cast(object, Class_GameObject);

    bool isVisible = (gameObject->m_flags & GAME_OBJECT_VISIBLE) == GAME_OBJECT_VISIBLE;
    if (isVisible)
    {
        // L'objet est déjà visible
        return;
    }

    gameObject->m_flags |= GAME_OBJECT_VISIBLE;
    PVector_PushBack(objManager->m_visibleObjects, gameObject);
}

void ObjectManager_ClearVisibleObjects(void *self)
{
    ObjectManager *objManager = self;

    GameObject **objects = (GameObject **)PVector_ToArray(objManager->m_visibleObjects);
    int count = PVector_GetSize(objManager->m_visibleObjects);
    for (int i = 0; i < count; i++)
    {
        objects[i]->m_flags &= ~GAME_OBJECT_VISIBLE;
    }

    PVector_Clear(objManager->m_visibleObjects);
}

static int ObjectComparator(const void *eltA, const void *eltB)
{
    GameObject *objectA = *(GameObject **)eltA;
    GameObject *objectB = *(GameObject **)eltB;
    return objectA->m_layer - objectB->m_layer;
}

GameObject **ObjectManager_GetVisibleObjects(void *self)
{
    ObjectManager *objManager = self;
    GameObject **objects = (GameObject **)PVector_ToArray(objManager->m_visibleObjects);
    int count = PVector_GetSize(objManager->m_visibleObjects);

    qsort((void *)objects, count, sizeof(GameObject *), ObjectComparator);

    return objects;
}
