#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

/**
 * trim - remove space before and after the string
 * @str: string to modifie
 * Return: nothing
 */
void trim(char *str)
{
	char *start = str;
	char *end;
	size_t len;

	while (*start == ' ' || *start == '\t')
		start++;

	len = strlen(start);
	if (len == 0)
	{
		str[0] = '\0';
		return;
	}

	end = start + len - 1;
	while (end > start && (*end == ' ' || *end == '\t' || *end == '\n'))
		end--;

	if (start != str)
	{
		memmove(str, start, end - start + 1);
		str[end - start + 1] = '\0';
	}
	else
	{
		*(end + 1) = '\0';
	}
}

/**
 * find_path - search the PATH
 * @cmd: name of command
 * Return: path of malloc or NULL if not found
 */
char *find_path(char *cmd)
{
	char *path, *path_copy, *dir, *full_path;
	size_t path_len, dir_len, cmd_len;

	if (cmd[0] == '/')
	{
		if (access(cmd, X_OK) == 0)
		{
			full_path = malloc(strlen(cmd) + 1);
			if (full_path)
				strcpy(full_path, cmd);
			return (full_path);
		}
		return (NULL);
	}

	path = getenv("PATH");
	if (path == NULL)
		return (NULL);

	path_copy = strdup(path);
	if (path_copy == NULL)
		return (NULL);

	cmd_len = strlen(cmd);
	dir = strtok(path_copy, ":");

	while (dir != NULL)
	{
		dir_len = strlen(dir);
		full_path = malloc(dir_len + 1 + cmd_len + 1);
		if (full_path == NULL)
		{
			free(path_copy);
			return (NULL);
		}

		strcpy(full_path, dir);
		strcat(full_path, "/");
		strcat(full_path, cmd);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

/**
 * main - simple shell entry point
 * Return: 0 on success
 */
int main(void)
{
	char *command = NULL;
	size_t len = 0;
	ssize_t nread;
	pid_t pid;
	int status;
	int interactive;
	char *args[1024];
	int i;
	char *token;
	char *cmd_path;

	interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
		{
			printf("#cisfun$ ");
			fflush(stdout);
		}

		nread = getline(&command, &len, stdin);

		if (nread == -1)
		{
			if (interactive)
				printf("\n");
			free(command);
			exit(0);
		}

		if (nread > 0 && command[nread - 1] == '\n')
			command[nread - 1] = '\0';

		trim(command);

		if (command[0] == '\0')
			continue;

		i = 0;
		token = strtok(command, " \t");
		while (token != NULL && i < 1023)
		{
			args[i] = token;
			i++;
			token = strtok(NULL, " \t");
		}
		args[i] = NULL;

		if (i == 0)
			continue;

		cmd_path = find_path(args[0]);
		if (cmd_path == NULL)
		{
			perror("./hsh");
			continue;
		}

		args[0] = cmd_path;

		pid = fork();

		if (pid == -1)
		{
			perror("./hsh");
			free(cmd_path);
			continue;
		}

		if (pid == 0)
		{
			execve(args[0], args, environ);

			perror("./hsh");
			exit(127);
		}
		else
		{
			waitpid(pid, &status, 0);
		}

		free(cmd_path);
	}

	free(command);
	return (0);
}
