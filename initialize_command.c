#include "shell.h"

/**
 * initialize_command - starts executing everything
 * @current_command: current command token
 * @type_command: type of command
 *
 * Return: void
 */
void initialize_command(char **current_command, int type_command)
{
	pid_t pid;

	if (type_command == EXTERNAL_COMMAND || type_command == PATH_COMMAND)
	{
		pid = fork();
		if (pid == 0)
			execute_command(current_command, type_command);
		else
		{
			waitpid(pid, &status, 0);
			status >>= 8;
		}
	}
	else
		execute_command(current_command, type_command);
}
