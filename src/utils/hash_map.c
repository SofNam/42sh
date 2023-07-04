#include "hash_map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xmalloc.h"
#include "xstring.h"

size_t get_hashed_key(char *key, const size_t size)
{
    return hash(key, xstrlen(key)) % size;
}

struct hash_map *hash_map_init(void)
{
    struct hash_map *hash_map = xcalloc(1, sizeof(struct hash_map));
    hash_map->data = xcalloc(HASH_MAP_SIZE, sizeof(struct pair_list *));
    for (size_t i = 0; i < HASH_MAP_SIZE; ++i)
        hash_map->data[i] = NULL;
    *(hash_map->data) = NULL;
    hash_map->size = HASH_MAP_SIZE;
    return hash_map;
}

static void pair_list_destroy(struct pair_list *pair)
{
    if (pair != NULL)
    {
        pair_list_destroy(pair->next);
        free(pair->key);
        free(pair->value);
        free(pair);
    }
}

void hash_map_destroy(struct hash_map *hash_map)
{
    if (hash_map != NULL)
    {
        for (size_t i = 0; i < hash_map->size; ++i)
            pair_list_destroy(hash_map->data[i]);
        free(hash_map->data);
    }
    free(hash_map);
}

static struct pair_list *hash_map_contains(struct hash_map *hash_map, char *key,
                                           size_t hkey)
{
    for (struct pair_list *p = hash_map->data[hkey]; p != NULL; p = p->next)
    {
        if (p->key == key)
            return p;
    }
    return NULL;
}

static struct pair_list *init_pair(char *key, char *value)
{
    struct pair_list *pair = xcalloc(1, sizeof(struct pair_list));
    pair->key = key;
    pair->value = value;
    return pair;
}

bool hash_map_insert(struct hash_map *hash_map, char *key, char *value)
{
    if (!hash_map || !hash_map->size)
        return false;
    size_t hkey = get_hashed_key(key, hash_map->size);
    struct pair_list *p = NULL;
    if ((p = hash_map_contains(hash_map, key, hkey)))
    {
        p->value = value;
        return true;
    }
    p = init_pair(key, value);
    // chain
    p->next = hash_map->data[hkey];
    hash_map->data[hkey] = p;
    return true;
}

char *hash_map_get(struct hash_map *hash_map, char *key)
{
    if (hash_map == NULL)
        return NULL;
    size_t hkey = get_hashed_key(key, hash_map->size);
    struct pair_list *pair = hash_map->data[hkey];
    for (; pair != NULL; pair = pair->next)
    {
        if (strcmp(pair->key, key) == 0)
            return pair->value;
    }
    return NULL;
}
