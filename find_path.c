#include "shell.c"

/**
 * find_path - search for an executable in the PATH
 * @cmd: command name
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
