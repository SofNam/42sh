#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_map.h"
#include "vector.h"

static void insertsort(struct vector *key_values)
{
    int j;
    struct pair_list *x = NULL;
    struct pair_list *p = NULL;
    for (size_t i = 1; i < key_values->size; i++)
    {
        x = key_values->data[i];
        j = i - 1;
        p = key_values->data[j];
        while (j >= 0 && strcmp(p->key, x->key) > 0)
        {
            key_values->data[j + 1] = p;
            j--;
            if (j >= 0)
                p = key_values->data[j];
        }
        key_values->data[j + 1] = x;
    }
}

static void print_sorted_key_values(struct vector *key_values, bool print)
{
    struct pair_list *p = NULL;
    if (print)
    {
        for (size_t i = 0; i < key_values->size; i++)
        {
            p = key_values->data[i];
            printf("alias %s='%s'\n", p->key, p->value);
        }
    }
    else
    {
        for (size_t i = 0; i < key_values->size; i++)
        {
            p = key_values->data[i];
            printf("%s='%s'\n", p->key, p->value);
        }
    }
    fflush(stdout);
}

void hash_map_dump(struct hash_map *hash, bool print)
{
    if (!hash || !hash->size)
        return;
    struct vector *keys_values = vector_create();
    for (size_t i = 0; i < hash->size; ++i)
    {
        if (!hash->data[i])
            continue;
        struct pair_list *p = hash->data[i];
        vector_add(keys_values, p);
        for (; p->next; p = p->next)
            vector_add(keys_values, p);
    }
    insertsort(keys_values);
    print_sorted_key_values(keys_values, print);
    vector_destroy(keys_values);
}

static void free_pair(struct pair_list *pair)
{
    free(pair->key);
    free(pair->value);
    free(pair);
}

bool hash_map_remove(struct hash_map *hash_map, char *key)
{
    if (hash_map == NULL || hash_map->size == 0)
        return false;
    size_t hkey = get_hashed_key(key, hash_map->size);
    if (hash_map->data[hkey] == NULL)
        return false;
    struct pair_list *pair = hash_map->data[hkey];
    if (strcmp(pair->key, key) == 0)
    {
        hash_map->data[hkey] = pair->next;
        free_pair(pair);
        return true;
    }
    for (struct pair_list *p = hash_map->data[hkey]; p->next; p = p->next)
    {
        if (strcmp(p->next->key, key) == 0)
        {
            pair = p->next;
            p->next = pair->next;
            free_pair(pair);
            return true;
        }
    }
    return false;
}
