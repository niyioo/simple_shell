#include "shell.h"

/**
 * free_split - Free memory allocated for a string array.
 *
 * @arr: Array to free.
 */
void free_split(char **arr)
{
	if (arr == NULL)
		return;
	int i;

	for (i = 0; arr[i] != NULL; i++)
	{
		free(arr[i]);
	}
	free(arr);
}

/**
 * execute - Execute a given command with arguments.
 *
 * @args: Arguments to the command to execute.
 *
 * Return: 0 on success, -1 on failure.
 */
int execute(char **args)
{
	pid_t pid;
	int status;

	if (args[0][0] != '/')
	{
		char *full_path = _path(args);

		if (full_path == NULL)
		{
			print_error(args[0], "not found");
			return (-1);
		}
		args[0] = full_path;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("Error");
		return (-1);
	}
	else if (pid == 0)
	{
		if (execve(args[0], args, environ) == -1)
			perror("Error");
		exit(EXIT_FAILURE);
	}
	else
	{
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return (0);
}

/**
 * print_error - Print an error message.
 *
 * @cmd: Command name that caused the error.
 * @msg: Error message to print.
 */
void print_error(const char *cmd, const char *msg)
{
	fprintf(stderr, "%s: %s\n", cmd, msg);
}
