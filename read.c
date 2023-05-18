#include "shell.h"

/**
 * read_command - Reads a command from the user
 * @buffer: A buffer to store the command in
 *
 * Return: The number of characters read
 */

ssize_t read_command(char *buffer, size_t buffer_size)
{
	if (fgets(buffer, buffer_size, stdin) != NULL) {
		size_t length = strlen(buffer);

		if (length > 0 && buffer[length - 1] == '\n') {
			buffer[length - 1] = '\0';
			return (length - 1);
		}

		return (length);
	}

	if (feof(stdin))
	{
		return (0);
	}
	else
	{
		return (-1);
	}
}
