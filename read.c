#include "shell.h"

/**
 * read_command - Reads a command from the user
 * @buffer: A buffer to store the command in
 *
 * Return: The number of characters read
 */
ssize_t read_command(char *buffer, size_t buffer_size)
{
	ssize_t read_count;

	read_count = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	buffer[read_count - 1] = '\0';

	return (read_count);
}
