#include "List.h"

ListNode *ListNode_New(int capacity)
{
    ListNode *node = (ListNode *)calloc(1, capacity + sizeof(ListNode));
    AssertNew(node);

    node->m_list = NULL;
    node->m_next = node;
    node->m_prev = node;
    node->m_capacity = capacity;

    return node;
}

void ListNode_Delete(ListNode *node)
{
    if (!node)
    {
        return;
    }

    List *list = node->m_list;
    if (list)
    {
        List_Pop(list, node);
    }

    free(node);
}

void List_PushBack(List *list, ListNode *node)
{
    assert(list && node && node->m_list == NULL);

    ListNode *sentinel = &list->m_sentinel;
    assert(node != sentinel);

    ListNode *last = sentinel->m_prev;

    node->m_list = list;
    node->m_prev = last;
    node->m_next = sentinel;

    last->m_next = node;
    sentinel->m_prev = node;

    ++list->m_nodeCount;
}

void List_Pop(List *list, ListNode *node)
{
    assert(list->m_nodeCount > 0);
    assert(node != &(list->m_sentinel));
    assert(node->m_list == list);

    ListNode *prev = node->m_prev;
    ListNode *next = node->m_next;

    prev->m_next = next;
    next->m_prev = prev;

    node->m_list = NULL;
    node->m_prev = node;
    node->m_next = node;

    list->m_nodeCount--;
}

ListNode *List_GetFirst(List *list)
{
    if (list->m_nodeCount > 0)
    {
        return list->m_sentinel.m_next;
    }
    else
    {
        return NULL;
    }
}

bool List_Contains(List *list, ListNode *node)
{
    ListNode *current;
    ListNode *sentinel;

    sentinel = &(list->m_sentinel);
    current = sentinel->m_next;
    while (current != sentinel)
    {
        if (current == node)
            return true;

        current = current->m_next;
    }

    return false;
}