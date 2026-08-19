#include "shell.h"

/**
 * find_in_path_dirs - search for cmd in PATH directories
 * @cmd: command name
 * @path_copy: copy of PATH to tokenize
 * Return: full path allocated with malloc, or NULL if not found
 */
char *find_in_path_dirs(char *cmd, char *path_copy)
{
	char *dir, *full_path;
	size_t dir_len, cmd_len;
	char *token;

	cmd_len = strlen(cmd);
	token = strtok(path_copy, ":");

	while (token != NULL)
	{
		dir = token;
		dir_len = strlen(dir);
		full_path = malloc(dir_len + 1 + cmd_len + 1);
		if (full_path == NULL)
			return (NULL);

		strcpy(full_path, dir);
		strcat(full_path, "/");
		strcat(full_path, cmd);

		if (access(full_path, X_OK) == 0)
			return (full_path);

		free(full_path);
		token = strtok(NULL, ":");
	}

	return (NULL);
}

/**
 * find_path_absolute - handle absolute/relative paths
 * @cmd: command path
 * Return: allocated copy of cmd, or NULL if not executable
 */
char *find_path_absolute(char *cmd)
{
	char *full_path;

	if (access(cmd, X_OK) == 0)
	{
		full_path = malloc(strlen(cmd) + 1);
		if (full_path)
			strcpy(full_path, cmd);
		return (full_path);
	}
	return (NULL);
}

/**
 * find_path - search for an executable in the PATH
 * @cmd: command name
 * Return: full path allocated with malloc, or NULL if not found
 */
char *find_path(char *cmd)
{
	char *path, *path_copy, *result;

	if (cmd[0] == '/' || cmd[0] == '.')
		return (find_path_absolute(cmd));

	path = get_path_env();
	if (path == NULL)
		return (NULL);

	path_copy = strdup(path);
	if (path_copy == NULL)
		return (NULL);

	result = find_in_path_dirs(cmd, path_copy);
	free(path_copy);

	return (result);
}
