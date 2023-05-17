#include "shell.h"

/**
 * betty_main - Entry point for the betty program
 *
 * Return: Always 0
 */
int betty_main(void)
{
	char buffer[BUFFER_SIZE];
	char *args[] = {NULL};
	int read_count;
	pid_t pid;

	while (1)
	{
		printf("$ ");

		read_count = read(STDIN_FILENO, buffer, BUFFER_SIZE);
		if (read_count == 0)
		{
			putchar('\n');
			break;
		}
		else if (read_count < 0)
		{
			perror("Error: ");
			exit(EXIT_FAILURE);
		}

		pid = fork();
		if (pid == -1)
		{
			perror("Error: ");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (execve(buffer, args, NULL) == -1)
			{
				perror("Error: ");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			wait(NULL);
		}
	}

	return (0);
}
