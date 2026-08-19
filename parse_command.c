#include "shell.h"

/**
 * parse_command - tokenize command into arguments
 * @command: input string
 * @args: output array
 * @count: number of arguments
 */
void parse_command(char *command, char **args, int *count)
{
	char *token;
	int i;

	i = 0;
	token = strtok(command, " \t");
	while (token != NULL && i < 1023)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, " \t");
	}
	args[i] = NULL;
	*count = i;
}
