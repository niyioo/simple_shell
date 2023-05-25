#include "shell.h"

/**
 * custom_env - prints the current environment
 * @command: command entered (unused)
 *
 * Return: void
 */
void custom_env(char **command __attribute__((unused)))
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
 * custom_getline - Reads a line of input from the user
 *
 * Return: The line of input as a string, or NULL on failure or EOF
 */
char *custom_getline(void)
{
	static char buffer[BUFFER_SIZE];
	static size_t buffer_pos;
	static size_t buffer_size;
	static char line[BUFFER_SIZE];
	static size_t line_pos;
	char c;

	while (1)
	{
		if (buffer_pos >= buffer_size)
		{
			ssize_t read_count = read(STDIN_FILENO, buffer, BUFFER_SIZE);

			if (read_count <= 0)
			{
				if (line_pos > 0)
				{
					line[line_pos] = '\0';
					return (line);
				}
				return (NULL);
			}
			buffer_pos = 0;
			buffer_size = (size_t)read_count;
		}
		c = buffer[buffer_pos++];
		if (c == '\n')
		{
			line[line_pos] = '\0';
			return (line);
		}
		line[line_pos++] = c;
		if (line_pos >= BUFFER_SIZE - 1)
		{
			line[BUFFER_SIZE - 1] = '\0';
			return (line);
		}
	}
}

/**
 * builtin_setenv - Initialize or modify an environment variable
 * @command: The command and arguments passed (setenv VARIABLE VALUE)
 */
void builtin_setenv(char **command)
{
	if (command[1] == NULL || command[2] == NULL)
	{
		print_string("setenv: Invalid command syntax\n", STDERR_FILENO);
		return;
	}

	if (setenv(command[1], command[2], 1) == -1)
	{
		perror("setenv");
		return;
	}
}

/**
 * builtin_unsetenv - Remove an environment variable
 * @command: The command and arguments passed (unsetenv VARIABLE)
 */
void builtin_unsetenv(char **command)
{
	if (command[1] == NULL)
	{
		print_string("unsetenv: Invalid command syntax\n", STDERR_FILENO);
		return;
	}

	if (unsetenv(command[1]) == -1)
	{
		perror("unsetenv");
		return;
	}
}
