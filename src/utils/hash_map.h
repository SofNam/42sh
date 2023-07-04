#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>

#define HASH_MAP_SIZE 512

/// @file hash_map.h
/// @brief Contains implementation of hash_map
/// and definition of related functions.

/// Linked list of key and value pairs.
struct pair_list
{
    char *key;
    char *value;
    struct pair_list *next;
};

/// Static implementation of hash_map with chaining.
struct hash_map
{
    struct pair_list **data;
    size_t size;
};

/// Return the hash value of a given string of size len.
size_t hash(char *str, const size_t len);

/// @brief Return the hash value of a given string of size len,
/// fitting the size of the given hash map.
size_t get_hashed_key(char *key, const size_t size);

/// Initialise a hash_map.
struct hash_map *hash_map_init(void);

/// Deallocate the space allocated to the given hash_map.
void hash_map_destroy(struct hash_map *hash_map);

/// @brief Insert a key value pair in the given hash_map.
/// If the key is already present, update its value.
bool hash_map_insert(struct hash_map *hash_map, char *key, char *value);

/// @brief Delete a key value pair in the given hash_map.
/// If the key isn't present or couldn't be removed, return false.
/// Otherwise return true.
bool hash_map_remove(struct hash_map *hash_map, char *key);

/// Print the content of the given hash_map.
void hash_map_dump(struct hash_map *hash_map, bool print);

/// Return the value of the given key, NULL if not found.
char *hash_map_get(struct hash_map *hash_map, char *key);

#endif /* ! HASH_MAP_H */
