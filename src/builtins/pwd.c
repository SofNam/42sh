#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *gwd(void)
{
    char *cwd = getcwd(NULL, 0);
    if (cwd == NULL)
        err(1, "fail to get currrent working directory");
    return cwd;
}

int pwd(void)
{
    char *cwd = gwd();
    puts(cwd);
    free(cwd);
    return 0;
}
