#ifndef RUN_COMMAND_H
#define RUN_COMMAND_H

#include <stddef.h>

/// @file run_command.h
/// @brief Contains definition of run_command for the execution of a command.

/// @brief
/// Fork and execute the file given as first argument using **execvp**.
/// @param argv
/// From the **execvp** man page:
/// The *const argv[] argument is an array of pointers to null-terminated
/// strings that represent the argument list available to the new program.
/// The first argument, by  convention,  should  point  to the filename
/// associated with the file being executed.
/// The array of pointers must be terminated by a null pointer.
/// @return exit status of the child process.
int run_command(int argc, char **argv);

#endif /* ! RUN_COMMAND_H */
