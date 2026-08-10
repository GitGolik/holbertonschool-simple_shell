#include "shell.h"

/**
 * execute_command - executes one command without arguments
 * @command: absolute path of the command to execute
 * @program_name: shell program name used for error messages
 *
 * Return: 0 on success, -1 on failure
 */
int execute_command(char *command, char *program_name)
{
	pid_t child;
	int status;
	char *args[2];

	child = fork();
	if (child == -1)
	{
		perror(program_name);
		return (-1);
	}

	if (child == 0)
	{
		args[0] = command;
		args[1] = NULL;
		execve(command, args, environ);
		perror(program_name);
		_exit(127);
	}

	if (waitpid(child, &status, 0) == -1)
	{
		perror(program_name);
		return (-1);
	}

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));

	return (-1);
}

/**
 * main - runs a minimal UNIX command line interpreter
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t size = 0;
	ssize_t read_count;
	int interactive;
	int last_status = 0;

	(void)argc;
	interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
		{
			printf("#cisfun$ ");
			fflush(stdout);
		}

		read_count = getline(&line, &size, stdin);
		if (read_count == -1)
			break;

		if (read_count > 0 && line[read_count - 1] == '\n')
			line[read_count - 1] = '\0';

		if (line[0] == '\0')
			continue;

		last_status = execute_command(line, argv[0]);
	}

	free(line);
	return (last_status);
}
