#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10

/**
 * split_input - Split the input string into an array of arguments.
 *
 * @input: Input string to split.
 *
 * Return: Array of arguments.
 */
char **split_input(char *input)
{
	// Check for NULL or empty input
	if (input == NULL || *input == '\0')
		return (NULL);

	char **arguments = NULL;
	char *token;
	int count = 0;

	// Count the number of arguments
	char *input_copy = strdup(input);
	if (input_copy == NULL)
		return (NULL);

	token = strtok(input_copy, " \n");
	while (token != NULL)
	{
		count++;
		token = strtok(NULL, " \n");
	}
	free(input_copy);

	// Allocate memory for the argument array
	arguments = malloc((count + 1) * sizeof(char *));
	if (arguments == NULL)
		return (NULL);

	// Split the input and store arguments in the array
	int i = 0;
	input_copy = strdup(input);
	if (input_copy == NULL)
	{
		free(arguments);
		return (NULL);
	}

	token = strtok(input_copy, " \n");
	while (token != NULL)
	{
		arguments[i] = strdup(token);
		if (arguments[i] == NULL)
		{
			// Free previously allocated arguments
			for (int j = 0; j < i; j++)
				free(arguments[j]);
			free(arguments);
			free(input_copy);
			return (NULL);
		}
		i++;
		token = strtok(NULL, " \n");
	}
	arguments[i] = NULL;
	free(input_copy);

	return (arguments);
}

/**
 * prompt - Display the shell prompt.
 */
void prompt(void)
{
	printf("$ ");
	fflush(stdout);
}

/**
 * execute_command - Execute a command.
 *
 * @command: Command to execute.
 * @args: Arguments for the command.
 *
 * Return: 0 on success, or -1 on failure.
 */
int execute_command(char *command, char **args)
{
	if (command == NULL)
	{
		return (-1);
	}

	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execvp(command, args) == -1)
		{
			perror ("Error executing command");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
	}
	return (0);
}
/**

  main - Entry point for the simple_shell program

Return: Always 0
*/
int main(void)
{
	char buffer[MAX_COMMAND_LENGTH];
	ssize_t read_count;
	// Check if the shell is running in interactive mode
	if (isatty(STDIN_FILENO))
	{
		while (1)
		{
			prompt();

			read_count = read_command(buffer, MAX_COMMAND_LENGTH);
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

			char **args = split_input(buffer);
			if (args == NULL)
			{
				fprintf(stderr, "Error splitting input\n");
				continue;
			}

			if (execute_command(args[0], args) == -1)
			{
				fprintf(stderr, "simple_shell: command not found: %s\n", buffer);
			}

			free_split(args);
		}
	}
	else
	{
		// Non-interactive mode
		while (fgets(buffer, MAX_COMMAND_LENGTH, stdin) != NULL)
		{
			char **args = split_input(buffer);
			if (args == NULL)
			{
				fprintf(stderr, "Error splitting input\n");
				continue;
			}

			if (execute_command(args[0], args) == -1)
			{
				fprintf(stderr, "simple_shell: command not found: %s\n", buffer);
			}

			free_split(args);
		}
	}

	return (0);
}
