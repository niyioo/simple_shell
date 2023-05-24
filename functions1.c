#include "shell.h"

/**
 * execute_env - Execute the "env" command to
 * print the current environment variables.
 *
 * Return: void
 */

void execute_env(void)
{
	char **env;

	for (**env = environ; *env != NULL; env++)
	{
		printf("%s\n", *env);
	}
}

/**
 * custom_getline - Custom implementation of the
 * getline function to read input.
 * @buffer: Buffer to store the input.
 * @buffer_size: Size of the buffer.
 *
 * Return: Number of characters read.
 */
ssize_t custom_getline(char *buffer, size_t buffer_size)
{
	static char static_buffer[MAX_COMMAND_LENGTH];
	static size_t index;
	ssize_t read_count = 0;
	char c;

	while ((c = getchar()) != '\n')
	{
		if (read_count < buffer_size - 1)
		{
			static_buffer[index++] = c;
			read_count++;
		}
	}

	static_buffer[index] = '\0';
	strcpy(buffer, static_buffer);
	index = 0;

	return (read_count);
}

/**
 * execute_exit - Execute the "exit" command to exit the shell.
 * @status: Exit status as a string.
 */

void execute_exit(char *status)
{
	int exit_status = atoi(status);

	exit(exit_status);
}

/**
 * execute_setenv - Execute the "setenv" command to initialize
 * or modify an environment variable.
 * @variable: Name of the variable.
 * @value: Value of the variable.
 */
void execute_setenv(char *variable, char *value)
{
	if (setenv(variable, value, 1) != 0)
	{
		fprintf(stderr, "Error: Failed to set environment variable\n");
	}
}

/**
 * execute_unsetenv - Execute the "unsetenv" command to
 * remove an environment variable.
 * @variable: Name of the variable to unset.
 */
void execute_unsetenv(char *variable)
{
	if (unsetenv(variable) != 0)
	{
		fprintf(stderr, "Error: Failed to unset environment variable\n");
	}
}
