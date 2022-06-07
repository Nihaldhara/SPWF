
#ifndef _LIST_H_
#define _LIST_H_

#include "../Settings.h"

typedef struct List_s List;
typedef struct ListNode_s ListNode;

/// @brief A node of a List.
struct ListNode_s
{
    /// @brief Pointeur vers la liste parente.
    List *m_list;

    /// @protected
    /// @brief Pointer to the previous node. Set to the sentinel address if this node is the first.
    ListNode *m_prev;

    /// @protected
    /// @brief Pointer to the next node. Set to the sentinel address if this node is the last.
    ListNode *m_next;

    int m_capacity;
};

ListNode *ListNode_New(int capacity);
void ListNode_Delete(ListNode *node);

INLINE void ListNode_SetData(ListNode *node, void *data, int dataSize)
{
    assert(node != NULL);
    assert(dataSize <= node->m_capacity);

    PE_Memcpy((void *)(node + 1), node->m_capacity, data, dataSize);
}

INLINE void *ListNode_GetData(ListNode *node)
{
    return (void *)(node + 1);
}

INLINE ListNode *ListNode_GetNode(void *data)
{
    assert(data != NULL);
    return (ListNode *)data - 1;
}

INLINE List *ListNode_GetList(ListNode *node)
{
    assert(node != NULL);
    return node->m_list;
}

typedef struct List_s
{
    ListNode m_sentinel;
    int m_nodeCount;
} List;

INLINE void List_Init(List *list)
{
    ListNode *sentinel = &(list->m_sentinel);

    sentinel->m_next = sentinel;
    sentinel->m_prev = sentinel;
    sentinel->m_capacity = 0;
    sentinel->m_list = list;

    list->m_nodeCount = 0;
}

INLINE bool List_IsEmpty(List *list)
{
    return (list->m_nodeCount == 0);
}

/// @brief Appends a node to the list.
/// @param[in,out] list this.
/// @param[in,out] node the node to append.
void List_PushBack(List *list, ListNode *node);

/// @brief Remove a node from the list.
/// @param[in,out] list this.
/// @param[in,out] node the node to remove.
void List_Pop(List *list, ListNode *node);

ListNode *List_GetFirst(List *list);

/// @brief Indicates if the list contains the node.
/// @param[in] list this.
/// @param[in] node a node.
/// @return true is the node is in the list, false otherwise.
bool List_Contains(List *list, ListNode *node);

/// @brief Gets the number of elements in the list.
/// @param[in] list this.
/// @return The number of elements.
INLINE int List_GetNodeCount(List *list)
{
    return list->m_nodeCount;
}

typedef struct ListIterator_s
{
    ListNode *m_sentinel;
    ListNode *m_node;
} ListIterator;

/// @brief Gets an iterator to enumerate the elements of the list.
/// @param list[in] this.
/// @param it[out] the iterator.
INLINE void List_GetIterator(List *list, ListIterator *it)
{
    it->m_sentinel = &list->m_sentinel;
    it->m_node = &list->m_sentinel;
}

/// @brief Advances the iterator to the next element of the collection.
/// @param[in,out] it this.
/// @return false if the iterator has passed the end of the collection, true otherwise.
INLINE bool ListIterator_MoveNext(ListIterator *it)
{
    it->m_node = it->m_node->m_next;
    return (it->m_node) != (it->m_sentinel);
}

/// @brief Gets a pointer to the element in the collection at the current position of the iterator.
/// @param[in] it this.
/// @return The element in the collection at the current position of the iterator.
INLINE void *ListIterator_Current(ListIterator *it)
{
    return (void *)(it->m_node + 1);
}

#endif