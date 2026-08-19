#include "shell.h"

/**
 * execute_builtin - execute built-in commands (exit, env)
 * @args: array of arguments
 * @last_status: pointer to last command status
 *
 * Return: 0 if not a builtin, -1 if exit, 1 if env executed
 */
int execute_builtin(char **args, int *last_status)
{
	int j;

	if (strcmp(args[0], "exit") == 0)
		return (-1);

	if (strcmp(args[0], "env") == 0)
	{
		for (j = 0; environ[j] != NULL; j++)
			printf("%s\n", environ[j]);
		*last_status = 0;
		return (1);
	}

	return (0);
}

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
 * exec_child - execute command in child process
 * @args: arguments array
 * @environ: environment variables
 *
 * Return: nothing (exits on failure)
 */
void exec_child(char **args, char **environ)
{
	execve(args[0], args, environ);
	perror("./hsh");
	exit(127);
}

/**
 * execute_external - execute an external command
 * @args: array of arguments (args[0] will be modified)
 * @line_num: current line number for error messages
 * @last_status: pointer to last command status
 *
 * Return: 0 on success
 */
int execute_external(char **args, int line_num, int *last_status)
{
	pid_t pid;
	char *cmd_path;

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
		exec_child(args, environ);

	wait_for_child(pid, last_status);
	free(cmd_path);
	return (0);
}

/**
 * execute_command - execute a single command
 * @args: array of arguments (NULL-terminated)
 * @line_num: current line number for error messages
 * @last_status: pointer to last command status
 *
 * Return: 0 on success, -1 on exit command
 */
int execute_command(char **args, int line_num, int *last_status)
{
	int result;

	result = execute_builtin(args, last_status);
	if (result != 0)
		return (result);

	return (execute_external(args, line_num, last_status));
}

/**
 * parse_line - tokenize a command line into arguments
 * @line: input line to parse
 * @args: array to store arguments (must hold at least 1024 pointers)
 *
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

/**
 * run_shell - main shell loop
 * Return: nothing (exits on exit command or EOF)
 */
void run_shell(void)
{
	char *command;
	size_t len;
	char *args[1024];
	int line_num;
	int last_status;
	int interactive;

	command = NULL;
	len = 0;
	line_num = 0;
	last_status = 0;
	interactive = isatty(STDIN_FILENO);

	while (1)
	{
		line_num++;
		if (shell_loop(&command, &len, args, line_num,
			       &last_status, interactive))
			break;
	}

	free(command);
	exit(last_status);
}
