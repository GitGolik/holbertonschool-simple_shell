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
	int line_num;
	int last_status;
	int arg_count;

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

		parse_command(command, args, &arg_count);

		if (arg_count == 0)
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
			handle_not_found(args[0], line_num, &last_status);
			continue;
		}

		exec_cmd(args, &last_status);
	}

	free(command);
	return (0);
}
