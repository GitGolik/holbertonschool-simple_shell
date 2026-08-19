#include "shell.h"

/**
 * get_path_env - get the value of PATH from environment
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
