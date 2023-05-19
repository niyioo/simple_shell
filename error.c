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
		printf("$ ");
		nread = getline(&buffer, &bufsize, stdin);
		if (nread == -1)
		{
			printf("\n");
			break;
		}
		if (buffer[nread - 1] == '\n')
			buffer[nread - 1] = '\0';
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (execl(buffer, buffer, NULL) == -1)
			{
				perror("execl");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
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
