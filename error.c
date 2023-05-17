#include "shell.h"

/**
 * error_main - Entry point for the error program
 *
 * Return: Always returns 0
 */
int error_main(void)
{
	char *buffer = NULL;
	size_t bufsize = 0;
	ssize_t nread;
	pid_t pid;

	while (1)
	{
		/* Display prompt*/
		printf("$ ");

		/* Read command from standard input */
		nread = getline(&buffer, &bufsize, stdin);
		if (nread == -1)
		{
			/* End of file condition (Ctrl-D) */
			printf("\n");
			break;
		}

		/* Remove trailing newline character, if present */
		if (buffer[nread - 1] == '\n')
			buffer[nread - 1] = '\0';

		/* Fork a new process */
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			/* Child process */

			/* Execute the command */
			if (execl(buffer, buffer, NULL) == -1)
			{
				perror("execl");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			/* Parent process */

			/* Wait for child process to finish */
			if (waitpid(pid, NULL, 0) == -1)
			{
				perror("waitpid");
				exit(EXIT_FAILURE);
			}
		}
	}

	free(buffer);
	exit(EXIT_SUCCESS);
}
