#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

void trim(char *str);
char *get_path_env(void);
char *find_path(char *cmd);
char *build_full_path(const char *dir, const char *cmd);

int execute_builtin(char **args, int *last_status);
int execute_external(char **args, int line_num, int *last_status);
int execute_command(char **args, int line_num, int *last_status);
int parse_line(char *line, char **args);
void run_shell(void);

#endif
