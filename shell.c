#include "shell.h"

/**
 * execute_command - execute a single command
 * @args: array of arguments (NULL-terminated)
 * @line_num: current line number for error messages
 * @last_status: pointer to last command status
 * Return: 0 on success, 1 on failure
 */
int execute_command(char **args, int line_num, int *last_status)
{
	pid_t pid;
	int status;
	char *cmd_path;
	int j;

	if (strcmp(args[0], "exit") == 0)
		return (-1);
	if (strcmp(args[0], "env") == 0)
	{
		for (j = 0; environ[j] != NULL; j++)
			printf("%s\n", environ[j]);
		*last_status = 0;
		return (0);
	}
	cmd_path = find_path(args[0]);
	if (cmd_path == NULL)
	{
		fprintf(stderr, "./hsh: %d: %s: not found\n", line_num, args[0]);
		*last_status = 127;
		return (0);
	}
	args[0] = cmd_path;
	pid = fork();
	if (pid == -1)
	{
		perror("./hsh");
		free(cmd_path);
		*last_status = 1;
		return (0);
	}
	if (pid == 0)
	{
		execve(args[0], args, environ);
		perror("./hsh");
		exit(127);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			*last_status = WEXITSTATUS(status);
		else
			*last_status = 1;
	}
	free(cmd_path);
	return (0);
}

/**
 * parse_line - tokenize a command line into arguments
 * @line: input line to parse
 * @args: array to store arguments (must hold at least 1024 pointers)
 * Return: number of arguments parsed
 */
int parse_line(char *line, char **args)
{
	int i;
	char *token;

	i = 0;
	token = strtok(line, " \t");
	while (token != NULL && i < 1023)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, " \t");
	}
	args[i] = NULL;

	return (i);
}

/**
 * run_shell - main shell loop
 * Return: nothing (exits on exit command or EOF)
 */
void run_shell(void)
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
		arg_count = parse_line(command, args);
		if (arg_count == 0)
			continue;
		if (execute_command(args, line_num, &last_status) == -1)
		{
			free(command);
			exit(last_status);
		}
	}
	free(command);
}
