#include "string_vector.h"

#include <stdlib.h>

#include "xmalloc.h"

struct string_vector *string_vector_create(void)
{
    struct string_vector *string_vector =
        xcalloc(1, sizeof(struct string_vector));
    string_vector->strings = xcalloc(VECTOR_SIZE, sizeof(void *));
    string_vector->size = 0;
    string_vector->capacity = VECTOR_SIZE;
    return string_vector;
}

void string_vector_resize(struct string_vector *string_vector)
{
    string_vector->capacity *= 2;
    string_vector->strings = xrealloc(string_vector->strings,
                                      string_vector->capacity * sizeof(void *));
}

void string_vector_add(struct string_vector *string_vector, char *string)
{
    if (string_vector->size == string_vector->capacity)
        string_vector_resize(string_vector);
    string_vector->strings[string_vector->size++] = string;
}

void string_vector_remove(struct string_vector *string_vector)
{
    if (string_vector->size > 0)
    {
        free(string_vector->strings[string_vector->size - 1]);
        string_vector->size--;
    }
}

void string_vector_destroy(struct string_vector *string_vector)
{
    if (string_vector)
    {
        for (size_t i = 0; i < string_vector->size; i++)
            free(string_vector->strings[i]);
        free(string_vector->strings);
        free(string_vector);
    }
}
