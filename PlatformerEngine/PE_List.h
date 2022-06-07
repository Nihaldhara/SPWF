
#ifndef _PE_LIST_H_
#define _PE_LIST_H_

#include "PE_Settings.h"
#include "PE_Allocator.h"

typedef struct PE_ListNode_s PE_ListNode;

/// @brief A node of a PE_OldList.
struct PE_ListNode_s
{
    /// @protected
    /// @brief Pointer to the previous node. Set to the sentinel address if this node is the first.
    PE_ListNode *m_prev;

    /// @protected
    /// @brief Pointer to the next node. Set to the sentinel address if this node is the last.
    PE_ListNode *m_next;

    int m_capacity;
};

PE_ListNode *PE_ListNode_New(int capacity, PE_Allocator *allocator);
void PE_ListNode_Free(PE_ListNode *node, PE_Allocator *allocator);

PE_INLINE void PE_ListNode_SetData(PE_ListNode *node, void *data, int dataSize)
{
    assert((node != NULL) && (data != NULL));
    assert(dataSize <= node->m_capacity);

    PE_Memcpy((void *)(node + 1), node->m_capacity, data, dataSize);
}

PE_INLINE void *PE_ListNode_GetData(PE_ListNode *node)
{
    return (void *)(node + 1);
}

PE_INLINE PE_ListNode *PE_ListNode_GetNode(void *data)
{
    assert(data != NULL);
    return (PE_ListNode *)data - 1;
}

PE_INLINE void PE_ListNode_Init(PE_ListNode *node)
{
    node->m_next = node;
    node->m_prev = node;
}

typedef struct PE_List_s
{
    PE_ListNode m_sentinel;
    int m_nodeCount;
} PE_List;

PE_INLINE void PE_List_Init(PE_List *list)
{
    PE_ListNode_Init(&list->m_sentinel);
    list->m_nodeCount = 0;
}

PE_INLINE bool PE_List_IsEmpty(PE_List *list)
{
    return (list->m_nodeCount == 0);
}

/// @brief Appends a node to the list.
/// @param[in,out] list this.
/// @param[in,out] node the node to append.
void PE_List_PushBack(PE_List *list, PE_ListNode *node);

/// @brief Remove a node from the list.
/// @param[in,out] list this.
/// @param[in,out] node the node to remove.
void PE_List_Pop(PE_List *list, PE_ListNode *node);

PE_ListNode *PE_List_GetFirst(PE_List *list);

/// @brief Indicates if the list contains the node.
/// @param[in] list this.
/// @param[in] node a node.
/// @return true is the node is in the list, false otherwise.
bool PE_List_Contains(PE_List *list, PE_ListNode *node);

/// @brief Gets the number of elements in the list.
/// @param[in] list this.
/// @return The number of elements.
PE_INLINE int PE_List_GetNodeCount(PE_List *list)
{
    return list->m_nodeCount;
}

typedef struct PE_ListIterator_s
{
    PE_ListNode *m_sentinel;
    PE_ListNode *m_node;
} PE_ListIterator;

/// @brief Gets an iterator to enumerate the elements of the list.
/// @param list[in] this.
/// @param it[out] the iterator.
PE_INLINE void PE_List_GetIterator(PE_List *list, PE_ListIterator *it)
{
    it->m_sentinel = &list->m_sentinel;
    it->m_node = &list->m_sentinel;
}

/// @brief Advances the iterator to the next element of the collection.
/// @param[in,out] it this.
/// @return false if the iterator has passed the end of the collection, true otherwise.
PE_INLINE bool PE_ListIterator_MoveNext(PE_ListIterator *it)
{
    it->m_node = it->m_node->m_next;
    return (it->m_node) != (it->m_sentinel);
}

/// @brief Gets a pointer to the element in the collection at the current position of the iterator.
/// @param[in] it this.
/// @return The element in the collection at the current position of the iterator.
PE_INLINE void *PE_ListIterator_Current(PE_ListIterator *it)
{
    return (void *)(it->m_node + 1);
}

#endif