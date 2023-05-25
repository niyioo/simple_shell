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

/**
 * builtin_exit - Exits the shell
 * @command: The command and arguments passed (exit and optional exit status)
 */
void builtin_exit(char **command)
{
	int exit_status = 0;

	if (command[1] != NULL)
		exit_status = atoi(command[1]);

	exit(exit_status);
}
