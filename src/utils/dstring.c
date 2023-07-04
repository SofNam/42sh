#include "dstring.h"

#include <stdlib.h>
#include <string.h>

#include "xmalloc.h"
#include "xstring.h"

struct string *string_create(void)
{
    struct string *string = xcalloc(1, sizeof(struct string));
    string->str = xcalloc(STRING_SIZE, sizeof(char));
    string->size = 0;
    string->capacity = STRING_SIZE;
    return string;
}

void string_resize(struct string *string)
{
    size_t old_capacity = string->capacity;
    string->capacity *= 2;
    string->str = xrealloc(string->str, string->capacity * sizeof(char));
    memset(string->str + old_capacity, 0, old_capacity);
}

void string_add(struct string *string, char *str)
{
    if (str == NULL)
        return;
    while (string->size + xstrlen(str) + 1 >= string->capacity)
        string_resize(string);
    for (size_t i = 0; str[i] != '\0'; i++)
        string->str[string->size++] = str[i];
}

void string_add_char(struct string *string, char c)
{
    if (string->size + 1 == string->capacity)
        string_resize(string);
    string->str[string->size++] = c;
}

void string_destroy(struct string *string)
{
    if (string)
    {
        free(string->str);
        free(string);
    }
}
