
/// @file RE_HashTable.h

#ifndef _RE_HASH_TABLE_H_
#define _RE_HASH_TABLE_H_

#include "RE_Settings.h"

/// @brief Structure représentant un élément (couple clé-valeur) dans une table de hachage.
typedef struct RE_HTEntry_s
{
    /// @protected
    /// @brief Clé de l'élément.
    char *m_key;

    /// @protected
    /// @brief Valeur de l'élément.
    void *m_data;
} RE_HTEntry;

/// @brief Renvoie la clé de l'élément.
/// @param[in] entry l'élément d'une table de hachage.
/// @return La clé de l'élément.
RE_INLINE char *RE_HTEntry_GetKey(RE_HTEntry *entry)
{
    return entry->m_key;
}

/// @brief Renvoie la donnée de l'élément.
/// @param[in] entry l'élément d'une table de hachage.
/// @return La donnée de l'élément.
RE_INLINE void *RE_HTEntry_GetData(RE_HTEntry *entry)
{
    return entry->m_data;
}

/// @brief Structure représentant une table de hachage.
/// Les éléments présents dans la table sont de type (void *).
/// Il est à la charge de l'utilisateur de s'occuper de la mémoire associée à ces pointeurs.
/// La création, la suppression et la recherche d'un élément sont en temps constant
/// quand la taille de la table est inférieure à sa capacité maximale.
/// Si la taille devient suppérieure à la capacité, la table de hachage se redimensionne automatiquement
/// mais cette opération à une complexité proportionnelle à la taille de la table.
/// Les éléments sont toujours stockés de façon contigüe en mémoire.
typedef struct RE_HashTable_s
{
    /// @protected
    /// @brief Le nombre d'éléments dans la table de hachage.
    int m_size;

    /// @protected
    /// @brief La capacité de la table. Il s'agit du nombre maximal d'élément avant un redimensionnement.
    int m_capacity;

    /// @protected
    /// @brief La table de hachage
    /// le membre m_hashTable[hashValue] donne l'indice du premier élément dans le tableau m_entries
    /// dans la chaîne associée à hashValue.
    int *m_hashTable;

    /// @protected
    /// @brief Tableau représentant les chaînes de hachage.
    /// Le membre m_next[index] donne l'indice de l'élément suivant dans le tableau m_entries
    /// dans la chaîne associée à hashValue.
    int *m_next;

    /// @protected
    /// @brief Tableau des éléments de la table de hachage.
    /// Les éléments sont toujours stockés de manière contigüe en mémoire
    /// et indicés de 0 à (m_size - 1).
    RE_HTEntry *m_entries;
} RE_HashTable;

/// @brief Crée une nouvelle table de hachage.
/// @param[in] capacity la capacité initiale de la table (nombre maximal d'éléments avant un redimensionnement).
/// @return La table créée ou NULL en cas d'erreur.
RE_HashTable *RE_HashTable_New(int capacity);

/// @brief Détruit une table de hachage préalablement créée avec RE_HashTable_New().
/// @param[in,out] hashTable la table de hachage à détruire.
void RE_HashTable_Delete(RE_HashTable *hashTable);

/// @brief Recherche l'élément associé à un clé donnée dans la table de hachage.
/// @param[in] hashTable la table de hachage.
/// @param[in] key la clé recherchée.
/// @return L'indice de l'élément ou -1 si la clé n'existe pas dans la table.
/// L'indice correspond à la position de l'élément dans le tableau RE_HashTable_GetEntries().
int RE_HashTable_Find(RE_HashTable *hashTable, char *key);

/// @brief Renvoie la donnée associée à une clé dans une table de hachage.
/// @param[in] hashTable la table de hachage.
/// @param[in] key la clé recherchée.
/// @return Un pointeur vers la donnée ou NULL si la clé n'existe pas dans la table.
void *RE_HashTable_Get(RE_HashTable *hashTable, char *key);

/// @brief Définit la donnée associée à une clé dans une table de hachage.
/// Si la clé n'existe pas la table, cette fonction crée automatiquement une nouvelle entrée.
/// @param[in,out] hashTable la table de hachage.
/// @param[in] key la clé.
/// @param[in] data la donnée à définir.
void RE_HashTable_Set(RE_HashTable *hashTable, char *key, void *data);

/// @brief Supprime l'entrée associée à une clé dans une table de hachage.
/// Cette fonction supprime simplement le pointeur vers la donnée.
/// @param[in,out] hashTable la table de hachage.
/// @param[in] key la clé.
/// @return EXIT_SUCCESS ou EXIT_FAILURE.
int RE_HashTable_Remove(RE_HashTable *hashTable, char *key);

/// @brief Supprime les entrées d'une table de hachage.
/// Cette fonction supprime simplement les pointeurs vers les données.
/// @param[in,out] hashTable la table de hachage.
void RE_HashTable_Clear(RE_HashTable *hashTable);

/// @brief Renvoie le tableau contenant les entrées de la table de hachage.
/// Attention, ce tableau n'est accessible qu'en lecture et ne doit pas être modifié.
/// La taille de ce tableau est donnée par RE_HashTable_GetNbEntries().
/// @param[in] hashTable la table de hachage.
/// @return Le tableau contenant les entrées de la table de hachage.
RE_INLINE RE_HTEntry *RE_HashTable_GetEntries(RE_HashTable *hashTable)
{
    return hashTable->m_entries;
}

/// @brief Renvoie le nombre d'éléments d'une table de hachage.
/// Il s'agit de la taille du table renvoyé par RE_HashTable_GetEntries().
/// @param[in] hashTable la table de hachage.
/// @return Le nombre d'éléments de la table de hachage.
RE_INLINE int RE_HashTable_GetNbEntries(RE_HashTable *hashTable)
{
    return hashTable->m_size;
}

/// @brief Renvoie la capacité (nombre maximal d'éléments avant un redimensionnement) d'une table de hachage.
/// @param[in] hashTable la table de hachage.
/// @return La capacité de la table de hachage.
RE_INLINE int RE_HashTable_GetCapacity(RE_HashTable *hashTable)
{
    return hashTable->m_capacity;
}

/// @brief Affiche sur la sortie standard une table de hachage pour du debug.
/// @param[in] hashTable la table de hachage.
void RE_HashTable_Print(RE_HashTable *hashTable);

#endif