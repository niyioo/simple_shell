#include "shell.h"

/**
 * execute_pipeline - Executes a pipeline of commands
 * @program_name: The name of the program
 * @commands: An array of commands in the pipeline
 * @num_commands: The number of commands in the pipeline
 */

void execute_pipeline(char *program_name, char *commands[], int num_commands)
{
	int pipes[num_commands - 1][2];
	int i, j;

	for (i = 0; i < num_commands - 1; i++)
	{
		if (pipe(pipes[i]) == -1)
		{
			fprintf(stderr, "%s: Error: Failed to create pipe\n", program_name);
			exit(EXIT_FAILURE);
		}
	}

	for (i = 0; i < num_commands; i++)
	{
		pid_t pid = fork();

		if (pid == 0)
		{
			if (i > 0)
			{
				if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
				{
					fprintf(stderr, "%s: Error: Failed to connect input from previous command\n", program_name);
					exit(EXIT_FAILURE);
				}
			}

			if (i < num_commands - 1)
			{
				if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
				{
					fprintf(stderr, "%s: Error: Failed to connect output to next command\n", program_name);
					exit(EXIT_FAILURE);
				}
			}

			for (j = 0; j < num_commands - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			char *command = commands[i];
			char *args[MAX_ARGUMENTS];
			parse_arguments(command, args);
			execute_command(program_name, args[0], args);
			exit(EXIT_SUCCESS);
		}
		else if (pid < 0)
		{
			fprintf(stderr, "%s: Error: Failed to create child process\n", program_name);
			exit(EXIT_FAILURE);
		}
	}

	for (i = 0; i < num_commands - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	for (i = 0; i < num_commands; i++)
	{
		int status;
		wait(&status);
	}
}

/**
 * parse_arguments - Parse a command into arguments.
 *
 * @command: The command to parse.
 * @args: The array to store the parsed arguments.
 *
 */

void parse_arguments(char *command, char *args[])
{
	char *token = strtok(command, " ");
	int arg_count = 0;

	while (token != NULL && arg_count < MAX_ARGUMENTS - 1)
	{
		args[arg_count] = token;
		arg_count++;

		token = strtok(NULL, " ");
	}

	args[arg_count] = NULL;
}
