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
	pid_t pid;
	int status;
	int interactive;
	char *args[1024];
	int i;
	char *token;
	char *cmd_path;
	int line_num;
	int last_status;
	int j;

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
			for (j = 0; environ[j] != NULL; j++)
				printf("%s\n", environ[j]);
			last_status = 0;
			continue;
		}

		cmd_path = find_path(args[0]);
		if (cmd_path == NULL)
		{
			fprintf(stderr, "./hsh: %d: %s: not found\n", line_num, args[0]);
			last_status = 127;
			continue;
		}

		args[0] = cmd_path;

		pid = fork();

		if (pid == -1)
		{
			perror("./hsh");
			free(cmd_path);
			last_status = 1;
			continue;
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
				last_status = WEXITSTATUS(status);
			else
				last_status = 1;
		}

		free(cmd_path);
	}

	free(command);
	return (0);
}
