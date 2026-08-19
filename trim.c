#include "shell.h"

/**
 * trim - remove leading and trailing spaces and newlines from a string
 * @str: string to modify (in-place)
 * Return: nothing (modifies the string in-place)
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
	while (end > start && (*end == ' ' || *end == '\t' || *end == '\n'))
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
