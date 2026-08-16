#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

/**
 * main - entry point of shell
 * Return: 0 for success
 */
int main(void)
{
	char *command = NULL;
	size_t len = 0;
	ssize_t nread;
	pid_t pid;
	int status;
	int interactive;

	interactive = isatty(STDIN_FILENO);

	while (1)
	{
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
			exit(0);
		}

		if ((nread > 0) && (command[nread - 1] == '\n'))
			command[nread - 1] = '\0';
		if (command[0] == '\0')
			continue;

		pid = fork();
		if (pid == -1)
		{
			perror("./hsh");
			continue;
		}

		if (pid == 0)
		{
			char *args[2];
			args[0] = command;
			args[1] = NULL;

			execve(command, args, environ);
			perror(command);
			exit(127);
		}
		else
			waitpid(pid, &status, 0);
	}

	free(command);
	return (0);
}
