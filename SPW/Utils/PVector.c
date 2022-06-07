#include "PVector.h"

PVector *PVector_New(int capacity)
{
    PVector *pVector = (PVector *)calloc(1, sizeof(PVector));
    AssertNew(pVector);

    if (capacity < 2)
    {
        capacity = 2;
    }

    pVector->m_capacity = capacity;
    pVector->m_size = 0;

    pVector->m_array = (void **)calloc(capacity, sizeof(void *));
    AssertNew(pVector->m_array);

    return pVector;
}

void PVector_Delete(PVector *pVector)
{
    if (!pVector)
        return;

    free(pVector->m_array);
    free(pVector);
}

void PVector_PushBack(PVector *pVector, void *ptr)
{
    int size = pVector->m_size;
    int capacity = pVector->m_capacity;
    if (size >= capacity)
    {
        capacity = capacity << 1;
        if (capacity <= size)
        {
            capacity = size + 1;
        }

        void **newBlock = (void **)realloc(
            pVector->m_array, capacity * sizeof(void *)
        );
        AssertNew(newBlock);

        pVector->m_array = newBlock;
        pVector->m_capacity = capacity;
    }

    pVector->m_array[size] = ptr;
    pVector->m_size = size + 1;
}

void PVector_Pop(PVector *pVector, int index)
{
    int size = pVector->m_size;
    if (index >= size - 1)
    {
        if (size > 0)
            pVector->m_size--;
    }
    else
    {
        // Copie le dernier élément à l'indice donné
        pVector->m_array[index] = pVector->m_array[size - 1];
        pVector->m_size--;
    }
}

/*void PVector_PopElement(PVector *pVector, void *elt)
{
    int size = pVector->m_size;
    void **pArray = pVector->m_array;

    int i = 0;
    while (i < size)
    {
        if (pArray[i] == elt)
        {
            pArray[i] = pArray[size - 1];
            size--;
        }
        else
        {
            i++;
        }
    }
    pVector->m_size = size;
}*/

void **PVector_CopyArray(PVector *pVector)
{
    void **res = (void **)calloc(pVector->m_size, sizeof(void *));
    AssertNew(res);
    RE_Memcpy(
        res, pVector->m_size * sizeof(void *),
        pVector->m_array, pVector->m_size * sizeof(void *)
    );
    return res;
}
