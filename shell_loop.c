#include "shell.h"

/**
 * wait_for_child - wait for child process and update status
 * @pid: child process id
 * @last_status: pointer to last command status
 *
 * Return: nothing
 */
void wait_for_child(pid_t pid, int *last_status)
{
	int status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		*last_status = WEXITSTATUS(status);
	else
		*last_status = 1;
}

/**
 * print_prompt - print shell prompt if interactive
 *
 * Return: nothing
 */
void print_prompt(void)
{
	printf("#cisfun$ ");
	fflush(stdout);
}

/**
 * read_command - read a line from stdin
 * @command: pointer to command buffer
 * @len: pointer to buffer length
 * @interactive: 1 if interactive mode
 *
 * Return: 1 on success, 0 on EOF
 */
int read_command(char **command, size_t *len, int interactive)
{
	ssize_t nread;

	nread = getline(command, len, stdin);
	if (nread == -1)
	{
		if (interactive)
			printf("\n");
		return (0);
	}

	if (nread > 0 && (*command)[nread - 1] == '\n')
		(*command)[nread - 1] = '\0';

	return (1);
}

/**
 * process_line - process a single command line
 * @command: command string to process
 * @args: array to store parsed arguments
 * @line_num: current line number
 * @last_status: pointer to last command status
 *
 * Return: -1 if exit, 0 otherwise
 */
int process_line(char *command, char **args, int line_num, int *last_status)
{
	trim(command);

	if (command[0] == '\0')
		return (0);

	if (parse_line(command, args) == 0)
		return (0);

	return (execute_command(args, line_num, last_status));
}

/**
 * shell_loop - run the main shell iteration
 * @command: command buffer pointer
 * @len: buffer length pointer
 * @args: arguments array
 * @line_num: current line number
 * @last_status: last command status
 * @interactive: interactive mode flag
 *
 * Return: 0 to continue, 1 to exit
 */
int shell_loop(char **command, size_t *len, char **args,
	       int line_num, int *last_status, int interactive)
{
	int result;

	if (interactive)
		print_prompt();

	if (read_command(command, len, interactive) == 0)
		return (1);

	result = process_line(*command, args, line_num, last_status);
	if (result == -1)
		return (1);

	return (0);
}
