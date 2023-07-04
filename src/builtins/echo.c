#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static void print_escape_char(char c)
{
    if (c == 'a')
        printf("\a");
    else if (c == 'b')
        printf("\b");
    else if (c == 'f')
        printf("\f");
    else if (c == 'n')
        printf("\n");
    else if (c == 'r')
        printf("\r");
    else if (c == 't')
        printf("\t");
    else if (c == 'v')
        printf("\v");
    else if (c == '\\')
        printf("\\");
    else
        putchar(c);
}

static void echo_word_no_escape(char *word)
{
    printf("%s", word);
}

static void echo_word_escape(char *word)
{
    for (size_t i = 0; i < strlen(word); i++)
    {
        if (word[i] == '\\' && word[i + 1] != '\0')
            print_escape_char(word[++i]);
        else
            putchar(word[i]);
    }
}

static void echo_word(char *word, const bool escape)
{
    if (escape)
        echo_word_escape(word);
    else
        echo_word_no_escape(word);
}

static void filter_argv(char *arg, bool *read_option, bool *trailing_newline,
                        bool *escape)
{
    if (arg[0] != '-' || strlen(arg) == 1 || !*read_option)
    {
        *read_option = false;
        echo_word(arg, *escape);
        return;
    }
    for (size_t j = 1; arg[j] != '\0'; j++)
    {
        if (arg[j] == 'n')
            *trailing_newline = false;
        else if (arg[j] == 'e')
            *escape = true;
        else if (arg[j] == 'E')
            *escape = false;
        else
            echo_word(arg, *escape);
    }
}

int echo(int argc, char **argv)
{
    bool read_option = true;
    bool trailing_newline = true;
    bool escape = false;
    for (int i = 1; i < argc - 1; i++)
    {
        filter_argv(argv[i], &read_option, &trailing_newline, &escape);
        putchar(' ');
    }
    if (argc > 1)
        filter_argv(argv[argc - 1], &read_option, &trailing_newline, &escape);
    if (trailing_newline)
        putchar('\n');
    return 0;
}
