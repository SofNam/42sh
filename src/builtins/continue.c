#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "builtins.h"
#include "err.h"
#include "is_number.h"
#include "token.h"
#include "xunistd.h"

extern struct global global;

int xcontinue(int argc, char **argv)
{
    global.continue_err = 0;
    if (argc == 1)
    {
        global.cont = 1;
        return 0;
    }
    if (argc == 2)
    {
        if (!is_number(argv[1]))
            errx(1, "continue: Illegal number");
        int n = atoi(argv[1]);
        if (n >= 1)
        {
            global.cont = n;
            return 0;
        }
        global.cont = 1;
        global.continue_err = -1;
        warnx("continue: %s: loop count out of range", argv[1]);
        return 1;
    }
    warnx("continue: too many arguments");
    global.cont = 1;
    global.continue_err = -1;
    return 1;
}
