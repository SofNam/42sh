#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "builtins.h"
#include "err.h"
#include "is_number.h"
#include "token.h"
#include "xunistd.h"

extern struct global global;

int xbreak(int argc, char **argv)
{
    global.breaky_err = 0;
    if (argc == 1)
    {
        global.breaky = 1;
        return 0;
    }
    if (argc == 2)
    {
        if (!is_number(argv[1]))
            errx(1, "break: Illegal number");

        int n = atoi(argv[1]);
        if (n >= 1)
        {
            global.breaky = n;
            return 0;
        }
        global.breaky = 1;
        global.breaky_err = -1;
        warnx("break: %s: loop count out of range", argv[1]);
        return 1;
    }
    warnx("break: too many arguments");
    global.breaky = 1;
    global.breaky_err = -1;
    return 1;
}
