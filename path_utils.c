#include "shell.h"

/**
 * check_absolute_path - check if command is absolute or relative path
 * @cmd: command to check
 *
 * Return: allocated path if executable, NULL otherwise
 */
char *check_absolute_path(char *cmd)
{
	char *full_path;

	if (cmd[0] != '/' && cmd[0] != '.')
		return (NULL);

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
 * search_in_path - search for command in PATH directories
 * @path_copy: copy of PATH string (will be modified)
 * @cmd: command name to search
 *
 * Return: allocated full path if found, NULL otherwise
 */
char *search_in_path(char *path_copy, const char *cmd)
{
	char *dir, *full_path;

	dir = strtok(path_copy, ":");

	while (dir != NULL)
	{
		full_path = build_full_path(dir, cmd);
		if (full_path == NULL)
			return (NULL);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		dir = strtok(NULL, ":");
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
	char *path, *path_copy, *full_path;

	full_path = check_absolute_path(cmd);
	if (full_path != NULL)
		return (full_path);

	path = get_path_env();
	if (path == NULL)
		return (NULL);

	path_copy = strdup(path);
	if (path_copy == NULL)
		return (NULL);

	return (search_in_path(path_copy, cmd));
}
