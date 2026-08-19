#include "shell.h"

/**
 * main - simple shell entry point
 * Return: 0 on success
 */
int main(void)
{
	char *command = NULL;
	size_t len = 0;
	ssize_t nread;
	int interactive;
	char *args[1024];
	int i;
	char *token;
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

		if (strcmp(args[0], "exit") == 0)
		{
			free(command);
			exit(last_status);
		}

		if (strcmp(args[0], "env") == 0)
		{
			print_env();
			last_status = 0;
			continue;
		}

		if (find_path(args[0]) == NULL)
		{
			fprintf(stderr, "./hsh: %d: %s: not found\n",
				line_num, args[0]);
			last_status = 127;
			continue;
		}

		run_command(args, &last_status);
	}

	free(command);
	return (0);
}
