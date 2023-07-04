#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

/// @file file_handler.h
/// @brief Definition of functions to handle files.

/// @brief Return the content of a given file as one whole string.
char *file_to_string(const char *filename);

/// @brief Return the content of a given file descriptor as one whole string.
char *file_fd_to_string(int fd);

#endif /* ! FILE_HANDLER_H */
