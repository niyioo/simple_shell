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
