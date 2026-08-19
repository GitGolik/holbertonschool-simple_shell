#include "shell.h"

/**
 * run_command - fork and execute a command
 * @args: array of arguments
 * @last_status: pointer to last command status
 * Return: nothing
 */
void run_command(char **args, int *last_status)
{
	pid_t pid;
	int status;
	char *cmd_path;

	cmd_path = find_path(args[0]);
	if (cmd_path == NULL)
	{
		*last_status = 127;
		return;
	}
	args[0] = cmd_path;
	pid = fork();
	if (pid == -1)
	{
		perror("./hsh");
		free(cmd_path);
		*last_status = 1;
		return;
	}
	if (pid == 0)
	{
		execve(args[0], args, environ);
		perror("./hsh");
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		*last_status = WEXITSTATUS(status);
	else
		*last_status = 1;
	free(cmd_path);
}
