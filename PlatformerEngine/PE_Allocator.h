
#ifndef _PE_ALLOCATOR_H_
#define _PE_ALLOCATOR_H_

#include "PE_Settings.h"

#ifdef PE_DEFAULT_ALLOCATOR

typedef struct PE_Allocator_s
{
    int m_allocCount;
    int m_freeCount;
    int m_maxBlockSize;
} PE_Allocator;

PE_Allocator *PE_Allocator_New();
void *PE_Allocator_Allocate(PE_Allocator *allocator, int size);
void PE_Allocator_Deallocate(PE_Allocator *allocator, void *ptr, int size);
void PE_Allocator_Free(PE_Allocator *allocator);

#else //-------------------------------------------------------------------------------------------

typedef struct PE_Block_s PE_Block;

/// @brief Structure représentant un bloc de mémoire.
/// Quand un bloc est attribué à l'utilisateur, il peut y ecrire ce qu'il souhaite.
/// Quand le bloc est disponible dans l'allocateur, ces premiers octets servent à sauvegarder
/// l'adresse du prochain bloc disponible.
struct PE_Block_s
{
    /// @brief Quand le bloc est disponible dans l'allocateur,
    /// m_next contient l'adresse du prochain bloc disponible.
    PE_Block *m_next;
};

/// @brief Structure représentant une zone mémoire contenant plusieurs blocs.
/// La taille d'un chunk est fixe et définie dans le code source.
typedef struct PE_Chunk_s
{
    /// @brief La taille en octet des blocs.
    int m_blockSize;

    /// @brief Pointeur vers la zone de mémoire.
    /// Cette zone est découpée en plusieurs blocs de taille m_blockSize.
    /// Les blocs disponibles sont chaînés.
    void *m_data;
} PE_Chunk;

/// @brief Structure représentant un allocateur ne pouvant allouer qu'une seule taille fixée de mémoire.
typedef struct PE_FixedAllocator_s
{
    /// @brief Tableau contenant les adresse des différentes zones mémoires pouvant être attribuées à l'utilisateur.
    /// Chaque zone est elle-même découpée en plusieurs blocs de taille fixe.
    PE_Chunk *m_chunks;

    /// @brief Le nombre de zones mémoires de l'allocateur.
    int m_chunkCount;

    /// @brief Le nombre maximal de zones mémoires de l'allocateur avant un redimensionnement de m_chunks;
    int m_chunkCapacity;

    /// @brief Liste chaînée de tous les blocs disponibles dans l'allocateur.
    /// Ces blocs sont situés dans les différentes zones mémoires pointées dans m_chunks.
    PE_Block *m_freeList;

    /// @brief La taille des blocs alloués par l'allocateur.
    int m_blockSize;
} PE_FixedAllocator;

PE_FixedAllocator *PE_FixedAllocator_New(int blockSize);
void *PE_FixedAllocator_Allocate(PE_FixedAllocator *fixedAlloc);
void PE_FixedAllocator_Deallocate(PE_FixedAllocator *fixedAlloc, void *ptr);

typedef struct PE_Allocator_s
{
    PE_FixedAllocator **m_fixedAlloc;
    int m_blockSizeCount;
    int *m_sizeMap;
    int m_maxBlockSize;
} PE_Allocator;

PE_Allocator *PE_Allocator_New();
void *PE_Allocator_Allocate(PE_Allocator *allocator, int size);
void PE_Allocator_Deallocate(PE_Allocator *allocator, void *ptr, int size);
void PE_Allocator_Free(PE_Allocator *allocator);

#endif
#endif
