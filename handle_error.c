#include "shell.h"

/**
 * handle_not_found - print error when command not found
 * @cmd: command name
 * @line_num: line number
 * @last_status: pointer to status
 */
void handle_not_found(char *cmd, int line_num, int *last_status)
{
	fprintf(stderr, "./hsh: %d: %s: not found\n", line_num, cmd);
	*last_status = 127;
}
