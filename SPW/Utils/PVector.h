#ifndef _P_VECTOR_H_
#define _P_VECTOR_H_

#include "../Settings.h"

typedef struct PVector_s
{
    int m_size;
    int m_capacity;
    void **m_array;
} PVector;

PVector *PVector_New(int capacity);
void PVector_Delete(PVector *pVector);

void PVector_PushBack(PVector *pVector, void *ptr);

INLINE void PVector_PopBack(PVector *pVector)
{
    if (pVector->m_size > 0)
        pVector->m_size--;
}

void PVector_Pop(PVector *pVector, int index);

//void PVector_PopElement(PVector *pVector, void *elt);

INLINE void *PVector_Get(PVector *pVector, int index)
{
    return pVector->m_array[index];
}

INLINE void PVector_Set(PVector *pVector, int index, void *ptr)
{
    pVector->m_array[index] = ptr;
}

INLINE void PVector_Clear(PVector *pVector)
{
    pVector->m_size = 0;
}

INLINE void **PVector_ToArray(PVector *pVector)
{
    return pVector->m_array;
}

INLINE int PVector_GetSize(PVector *pVector)
{
    return pVector->m_size;
}

void **PVector_CopyArray(PVector *pVector);

INLINE int PVector_GetCapacity(PVector *pVector)
{
    return pVector->m_capacity;
}

#endif
