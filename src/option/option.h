#ifndef OPTION_H
#define OPTION_H

#include "hash_map.h"

///@file option.h
///@brief Contains functions to parse and related methods.

/// Struct used to contain/to point towards the others structs.
struct option
{
    struct option_c *c;
    struct option_O *O;
    struct option_filename *filename;
    struct gnu_option *gnu;
    char *error;
};

/// Struct used to detect if there is a '-c' option. If it does, it contains its
/// position in the input string and the command that follows the option.
struct option_c
{
    int state;
    char *command;
    int position;
};

/// Struct used as a list to detect if there is a '-O' or '+O option. If it
/// does, it contains its position in the input string and the shopt that
/// follows the option.
struct option_O
{
    int state;
    char *shopt;
    int position;
    struct option_O *next;
};

/// Struct used to detect if there is a file. If it does, it contains its
/// position in the input string and its name.
struct option_filename
{
    const char *filename;
    int position;
};

/// @enum e_gnu_option
/// @var DEBUGGER
/// Arrange  for  the debugger profile to be executed before the shell starts.
/// Turns on extended debugging mode.
/// @var DUMP_PO_STRINGS
/// Equivalent  to  -D,  but  the output is in the GNU gettext po (portable
/// object) file format.
/// @var DUMP_STRINGS
/// Equivalent to -D.
/// @var HELP
/// Display a usage message on standard output and exit successfully.
/// @var INIT_FILE
/// file
/// @var RCFILE
/// file /n Execute  commands from file instead of the system wide
/// initialization file /etc/bash.bashrc and the standard personal
/// initialization file ~/.bashr if the shell is interactive.
/// @var LOGIN
/// Equivalent to -l.
/// @var NOEDITING
/// Do not use the GNU readline library to read command lines when the shell is
/// interactive.
/// @var NOPROFILE
/// Do  not read either the system-wide startup file /etc/profile or any of the
/// personal initialization files ~/.bash_profile, ~/.bash_login, or ~/.profile.
/// By default, bash reads these files when it is invoked as a login shell.
/// @var NORC
/// Do not read and execute the system wide initialization file /etc/bash.bashrc
/// and the personal initialization file ~/.bashrc if the shell is interactive.
/// This option is on by default if the shell is invoked as sh.
/// @var POSIX
/// Change the behavior of bash where the default operation differs from the
/// POSIX standard to match the standard (posix mode). See SEE ALSO below for a
/// reference to a document that details how posix mode affects bash's behavior.
/// @var RESTRICTED
/// The shell becomes restricted.
/// @var VERBOSE
/// Equivalent to -v.
/// @var VERSION
/// Show version information for this instance of bash on the standard output
/// and exit successfully.
/// @var NO_OPTION
/// When there is no option in the input.
enum e_gnu_option
{
    DEBUGGER = 0,
    DUMP_PO_STRINGS,
    DUMP_STRINGS,
    HELP,
    INIT_FILE,
    RCFILE,
    LOGIN,
    NOEDITING,
    NOPROFILE,
    NORC,
    POSIX,
    RESTRICTED,
    VERBOSE,
    VERSION,
    NO_OPTION
};

/// Struct used to detect if there is a gnu long option. If it does, it contains
/// its position in the input string and an enum to be able to recognize the
/// specific gnu long option.
struct gnu_option
{
    enum e_gnu_option e_gnu;
    int position;
};

/// Main function that enables us to parse.
struct option *get_option(int argc, char *const *argv,
                          struct hash_map *variables);

/// Initializes the struct option.
struct option *option_init(void);

/// Creates the list for the struct option_O.
struct option_O *get_last_O(struct option *option);

/// Frees every memory allocated struct.
void free_option(struct option *option);

#endif /* ! OPTION_H */
