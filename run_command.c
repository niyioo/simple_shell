#include "shell.h"

/**
 * run_command - Executes the command
 * @command: Command to execute
 * @args: Arguments to the command
 */
void run_command(char *command, char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		/* Child process */
		if (execve(command, args, environ) == -1)
		{
			perror("Error");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		/* Forking error */
		perror("Error");
	}
	else
	{
		/* Parent process */
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		        }
}
