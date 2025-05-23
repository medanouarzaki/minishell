#ifndef MINISHELL_H
#define MINISHELL_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* builtins.c */
int minishell_cd(char **args);
int minishell_exit(char **args);
int minishell_help(char **args);

/* main.c */
int launch(char **args);
int execute(char **args);
char **split_line(char *line);
void minishell_loop(void);

#endif
