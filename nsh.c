#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "nsh.h"

#define NSH_BUF_SIZE 1024
#define NSH_TOK_SIZE 64
#define NSH_TOK_DELIM " \t\n"

size_t buf_size;
size_t tok_size;

int main()
{
    nsh_loop();
    return 0;
}

char *nsh_builtin_strs[] = {
    "help",
    "cd",
    "exit"
};

char *nsh_builtin_descs[] = {
    "prints information about the shell",
    "changes the present working directory to the passed argument",
    "exits the shell"
};

int (*nsh_builtin_funcs[])(char **) = {
    &nsh_help,
    &nsh_cd,
    &nsh_exit
};

size_t nsh_num_builtins()
{
    return (sizeof(nsh_builtin_strs) / sizeof(char *));
}

int nsh_cd(char **args)
{
    if (!args[1]) fprintf(stderr, "nsh: expected argument to \"cd\"\n");
    if (chdir(args[1])) perror("nsh");
    return 1;
}

int nsh_help(char **args)
{
    fprintf(stdout, "nsh: Shell developed by Nathan Abebe, 2024. Version 1.0.\n");
    fprintf(stdout, "Inspired by lsh: https://github.com/brenns10/lsh\n\n");
    fprintf(stdout, "The following builtin functions are supported:\n");

    size_t num_builtins = nsh_num_builtins();
    for (size_t i = 0; i < num_builtins; ++i)
    {
        fprintf(stdout, "%s%*s-     %s.\n", nsh_builtin_strs[i], 
                (int)(10 - strlen(nsh_builtin_strs[i])), "", nsh_builtin_descs[i]);
    }
    return 1;
}

int nsh_exit(char **args)
{
    return 0;
}

void nsh_loop()
{
    int status;
    char *line;
    char **args;
    
    do
    {
        printf("> ");

        line = nsh_read(stdin);
        args = nsh_tokenize(line);
        status = nsh_exec(args);

        free(line);
        free(args);
    } while (status);
}

char *nsh_read(FILE* in)
{
    buf_size = NSH_BUF_SIZE;
    char *buffer = malloc(buf_size * sizeof(char));
    if (!buffer) nsh_alloc_error(NULL, NULL);
    buffer[0] = '\0';
    
    size_t position = 0;
    char c;
    while (((c = fgetc(in)) != EOF) && (c != '\n'))
    {
        if ((position + 1) == buf_size)
        {
            buf_size *= 2;
            char *new_buffer = realloc(buffer, buf_size * sizeof(char));
            if (!new_buffer) nsh_alloc_error(buffer, NULL);
            buffer = new_buffer;
        }
        buffer[position++] = c;
        buffer[position] = '\0';
    }

    return buffer;
}

char **nsh_tokenize(char *line)
{
    tok_size = NSH_TOK_SIZE;
    char **tokens = malloc(tok_size * sizeof(char *));
    if (!tokens) nsh_alloc_error(line, NULL);

    char *line_copy = line;

    char *delim = NSH_TOK_DELIM;
    tokens[0] = strtok(line_copy, delim);
    size_t counter = 0;
    while (tokens[counter] != NULL)
    {
        if (counter == tok_size)
        {
            tok_size *= 2;
            char **new_tokens = realloc(tokens, tok_size * sizeof(char *));
            if (!new_tokens) nsh_alloc_error(line, tokens);
            tokens = new_tokens;
        }
        tokens[++counter] = strtok(NULL, delim);
    }
    
    return tokens;
}

int nsh_exec(char **args)
{
    if (args[0] == NULL) return 1;

    size_t num_builtins = nsh_num_builtins();
    for (size_t i = 0; i < num_builtins; ++i)
    {
        if (!strcmp(args[0], nsh_builtin_strs[i]))
        {
            return (*nsh_builtin_funcs[i])(args);
        }
    }

    return nsh_launch(args);
}

int nsh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    // child process
    {
        if (execvp(args[0], args) == -1)
        {
            perror("nsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    // error forking
    {
        perror("nsh");
    }
    else
    // parent process
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

void nsh_alloc_error(void *ptr1, void *ptr2)
{
    free(ptr1);
    free(ptr2);
    fprintf(stderr, "nsh: allocation error\n");
    exit(EXIT_FAILURE);
}