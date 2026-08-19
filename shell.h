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
char *build_full_path(const char *dir, const char *cmd);

char *check_absolute_path(char *cmd);
char *search_in_path(char *path_copy, const char *cmd);
char *find_path(char *cmd);

void wait_for_child(pid_t pid, int *last_status);
void print_prompt(void);
int read_command(char **command, size_t *len, int interactive);

int execute_builtin(char **args, int *last_status);
int execute_external(char **args, int line_num, int *last_status);
int execute_command(char **args, int line_num, int *last_status);
int parse_line(char *line, char **args);
int shell_iter(char **command, size_t *len, char **args,
	       int line_num, int *last_status, int interactive);
void run_shell(void);

#endif
