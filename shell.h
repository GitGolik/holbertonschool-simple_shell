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

#endif
