#ifndef __NSH_H__
#define __NSH_H__

#include <stdio.h>

extern size_t buf_size;
extern size_t tok_size;

int nsh_help(char **args);

int nsh_cd(char **args);

int nsh_exit(char **args);

size_t nsh_num_builtins();

void nsh_loop();

char *nsh_read(FILE* in);

char **nsh_tokenize(char *line);

int nsh_exec(char **args);

int nsh_launch(char **args);

void nsh_alloc_error(void *ptr1, void *ptr2);

void nsh_cd_error();

#endif