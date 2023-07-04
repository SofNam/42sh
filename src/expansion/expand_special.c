#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "builtins.h"
#include "dstring.h"
#include "hash_map.h"
#include "xmalloc.h"
#include "xstring.h"

#define RAND_MAX_VALUE 32767

extern struct global global;

char *expand_dollar(void)
{
    pid_t pid = getpid();
    char *buf = xcalloc(digit_count(pid) + 2, sizeof(char));
    sprintf(buf, "%d", pid);
    return buf;
}

char *expand_uid(void)
{
    pid_t uid = getuid();
    char *buf = xcalloc(digit_count(uid) + 2, sizeof(char));
    sprintf(buf, "%d", uid);
    return buf;
}

char *expand_qmark(void)
{
    char *buf = xcalloc(digit_count(global.last_return) + 2, sizeof(char));
    sprintf(buf, "%d", global.last_return);
    return buf;
}

char *expand_random(void)
{
    int rnd = rand() % RAND_MAX_VALUE;
    char *buf = xcalloc(digit_count(rnd) + 2, sizeof(char));
    sprintf(buf, "%d", rnd);
    return buf;
}

char *expand_all_parameters(struct hash_map *variables)
{
    struct string *string = string_create();
    size_t args = atol(hash_map_get(variables, "#"));
    char *arg = xcalloc(2, sizeof(char));
    for (size_t i = 1; i < args; i++)
    {
        arg = xrealloc(arg, (digit_count(i) + 2) * sizeof(char));
        sprintf(arg, "%zu", i);
        string_add(string, hash_map_get(variables, arg));
        string_add_char(string, ' ');
    }
    arg = xrealloc(arg, (digit_count(args) + 2) * sizeof(char));
    sprintf(arg, "%zu", args);
    string_add(string, hash_map_get(variables, arg));
    free(arg);
    char *res = xstrdup(string->str);
    string_destroy(string);
    return res;
}
