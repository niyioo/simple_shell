#include "shell.h"

/**
 * custom_env - prints the current environment
 * @command: command entered (unused)
 *
 * Return: void
 */
void custom_env(char **command _attribute_((unused)))
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		print_string(environ[i], STDOUT_FILENO);
		print_string("\n", STDOUT_FILENO);
	}
}

/**
 * custom_exit - exits the shell
 * @tokenized_command: command entered
 *
 * Return: void
 */
void custom_exit(char **tokenized_command)
{
	int num_tokens = 0, arg;

	for (; tokenized_command[num_tokens] != NULL; num_tokens++)
		;

	if (num_tokens == 1)
	{
		free(tokenized_command);
		free(line);
		free(commands);
		exit(status);
	}
	else if (num_tokens == 2)
	{
		arg = _atoi(tokenized_command[1]);
		if (arg == -1)
		{
			print_string(shell_name, STDERR_FILENO);
			print_string(": 1: exit: Illegal number: ", STDERR_FILENO);
			print_string(tokenized_command[1], STDERR_FILENO);
			print_string("\n", STDERR_FILENO);
			status = 2;
		}
		else
		{
			free(line);
			free(tokenized_command);
			free(commands);
			exit(arg);
		}
	}
	else
		print_string("$: exit doesn't take more than one argument\n", STDERR_FILENO);
}

/**
 * custom_getline - Read a line of input from stdin
 * Return: The line read
 */
char *custom_getline(void)
{
	static char *buffer;
	static int buffer_index;
	static int buffer_size;

	char *line = NULL;
	int line_index = 0;
	char current_char;

	while (1)
	{
		if (buffer_index >= buffer_size)
		{
			buffer = realloc(buffer, (buffer_size + READ_SIZE) * sizeof(char));
			if (!buffer)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
			buffer_size += read(STDIN_FILENO, buffer + buffer_size, READ_SIZE);
			if (buffer_size <= 0)
			{
				free(buffer);
				return (NULL);
			}
		}
		current_char = buffer[buffer_index++];
		if (current_char == '\n' || current_char == EOF)
			break;

		line = realloc(line, (line_index + 1) * sizeof(char));
		if (!line)
		{
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		line[line_index++] = current_char;
	}
	if (line)
		line[line_index] = '\0';
	return (line);
}

/**
 * builtin_setenv - Set or modify an environment variable
 * @command: Command arguments (setenv VARIABLE VALUE)
 */
void builtin_setenv(char **command)
{
	if (command[1] == NULL || command[2] == NULL)
	{
		fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		return;
	}

	if (setenv(command[1], command[2], 1) == -1)
		perror("setenv");
}

/**
 * builtin_unsetenv - Unset an environment variable
 * @command: Command arguments (unsetenv VARIABLE)
 */
void builtin_unsetenv(char **command)
{
	if (command[1] == NULL)
	{
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		return;
	}

	if (unsetenv(command[1]) == -1)
		perror("unsetenv");
}
