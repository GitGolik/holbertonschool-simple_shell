#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

/**
 * trim - remove space before and after the string
 * @str: string to modifie
 * Return: nothing
 */
void trim(char *str)
{
	char *start = str;
	char *end;
	size_t len;

	while (*start == ' ' || *start == '\t')
		start++;

	len = strlen(start);
	if (len == 0)
	{
		str[0] = '\0';
		return;
	}
	end = start + len - 1;
	while ((end > start) && ((*end == ' ') || (*end == '\t') || (*end == '\n')))
		end--;

	if (start != str)
	{
		memmove(str, start, end - start + 1);
		str[end - start + 1] = '\0';
	}
	else
	{
		*(end + 1) = '\0';
	}
}

/**
 * main - entry point of the shell
 * Return: 0 success
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

		pid = fork();

		if (pid == -1)
		{
			perror("./hsh");
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
		}
	}

	free(command);
	return (0);
}
