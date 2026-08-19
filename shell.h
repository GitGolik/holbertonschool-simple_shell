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
char *find_in_path_dirs(char *cmd, char *path_copy);
char *find_path_absolute(char *cmd);
char *find_path(char *cmd);
void print_env(void);
void exec_cmd(char **args, int *last_status);
void handle_not_found(char *cmd, int line_num, int *last_status);
void parse_command(char *command, char **args, int *count);

#endif
