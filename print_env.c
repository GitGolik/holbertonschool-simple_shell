#include "shell.h"

/**
 * print_env - print the current environment
 */
void print_env(void)
{
	int j;

	for (j = 0; environ[j] != NULL; j++)
		printf("%s\n", environ[j]);
}
