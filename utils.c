#include "shell.h"

/**
 * trim - remove leading and trailing spaces and tabs from a string
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
 * build_full_path - build full path from directory and command
 * @dir: directory path
 * @cmd: command name
 *
 * Return: allocated full path, or NULL on failure
 */
char *build_full_path(const char *dir, const char *cmd)
{
	char *full_path;
	size_t dir_len, cmd_len;

	dir_len = strlen(dir);
	cmd_len = strlen(cmd);
	full_path = malloc(dir_len + 1 + cmd_len + 1);
	if (full_path == NULL)
		return (NULL);

	strcpy(full_path, dir);
	strcat(full_path, "/");
	strcat(full_path, cmd);

	return (full_path);
}
