#ifndef REDIR_H
#define REDIR_H

///@file redir.h
///@brief Contains definitions of redirection struct.

/// @brief enum of different types of redirections
enum type
{
    GREAT, // >
    LESS, // <
    DGREAT, // >>
    DLESS, // <<
    DLESSDASH, // <<-
    GREATAND, // >&
    LESSAND, // <&
    CLOBBER, // >|
    LESSGREAT // <>
};

struct redir
{
    int io_save_err;
    int io_save;
    int io;
    int fd;
    enum type type;
    char *word;
};

/// Allocates redir struct.
struct redir *redir_create(void);

/// Deallocate redir struct.
void redir_destroy(struct redir *redir);

/// Return corresponding redir operator
int which_redir_operator(const char *operator);

/// Return corresponding stream file descriptor
int io_from_redirop(int redirop);

#endif /* ! REDIR_H */
