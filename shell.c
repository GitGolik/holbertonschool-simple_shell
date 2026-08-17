#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

/**
 * trim - remove leading and trailing spaces and newlines from a string
 * @str: string to modify (in-place)
 *
 * Return: nothing (modifies the string in-place)
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
 * get_path_env - get the value of PATH from the environment
 *
 * Return: value of PATH, or NULL if not found
 */
char *get_path_env(void)
{
	int i;
	char **env;

	env = environ;

	for (i = 0; env[i] != NULL; i++)
	{
		if (strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
	}

	return (NULL);
}

/**
 * find_path - search for an executable in the PATH
 * @cmd: command name (e.g., "ls")
 *
 * Return: full path allocated with malloc, or NULL if not found
 */
char *find_path(char *cmd)
{
	char *path, *path_copy, *dir, *full_path;
	size_t dir_len, cmd_len;

	if (cmd[0] == '/' || cmd[0] == '.')
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

	path = get_path_env();
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
 *
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
	int line_num;
	int last_status;

	interactive = isatty(STDIN_FILENO);
	line_num = 0;
	last_status = 0;

	while (1)
	{
		line_num++;

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
			exit(last_status);
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

		/* Built-in: exit */
		if (strcmp(args[0], "exit") == 0)
		{
			free(command);
			exit(last_status);
		}

		cmd_path = find_path(args[0]);
		if (cmd_path == NULL)
		{
			fprintf(stderr, "./hsh: %d: %s: not found\n", line_num, args[0]);
			last_status = 127;
			continue;
		}

		args[0] = cmd_path;

		pid = fork();

		if (pid == -1)
		{
			perror("./hsh");
			free(cmd_path);
			last_status = 1;
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
			/* Extraire le vrai code de retour */
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else
				last_status = 1;
		}

		free(cmd_path);
	}

	free(command);
	return (0);
}
