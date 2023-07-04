#include "vector.h"

#include <stdlib.h>

#include "xmalloc.h"

struct vector *vector_create(void)
{
    struct vector *vector = xcalloc(1, sizeof(struct vector));
    vector->data = xcalloc(VECTOR_SIZE, sizeof(void *));
    vector->size = 0;
    vector->capacity = VECTOR_SIZE;
    return vector;
}

void vector_resize(struct vector *vector)
{
    vector->capacity *= 2;
    vector->data = xrealloc(vector->data, vector->capacity * sizeof(void *));
}

void vector_add(struct vector *vector, void *datum)
{
    if (vector->size == vector->capacity)
        vector_resize(vector);
    vector->data[vector->size++] = datum;
}

void vector_destroy(struct vector *vector)
{
    if (vector)
    {
        free(vector->data);
        free(vector);
    }
}
