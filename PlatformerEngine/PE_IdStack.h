#ifndef _PE_ID_STACK_H_
#define _PE_ID_STACK_H_

#include "PE_API.h"
#include "PE_Settings.h"

#include <stdlib.h>

typedef struct PE_IdStack_s
{
    PE_Id *m_array;
    int m_size;
    int m_capacity;
} PE_IdStack;

PE_IdStack *PE_IdStack_New();
void PE_IdStack_Free(PE_IdStack *stack);

void PE_IdStack_DoubleCapacity(PE_IdStack *stack);

PE_INLINE PE_Id PE_IdStack_Pop(PE_IdStack *stack)
{
    assert(stack->m_size > 0);

    --stack->m_size;
    return stack->m_array[stack->m_size];
}

PE_INLINE void PE_IdStack_PushBack(PE_IdStack *stack, PE_Id id)
{
    if (stack->m_size >= stack->m_capacity)
    {
        PE_IdStack_DoubleCapacity(stack);
    }
    stack->m_array[stack->m_size] = id;
    ++stack->m_size;
}

PE_INLINE void PE_IdStack_Clear(PE_IdStack *stack)
{
    stack->m_size = 0;
}

PE_INLINE int PE_IdStack_GetSize(PE_IdStack *stack)
{
    return stack->m_size;
}

PE_INLINE PE_Id *PE_IdStack_ToArray(PE_IdStack *stack)
{
    return stack->m_array;
}

#endif
