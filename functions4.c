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
	args[arg_count] = NULL;
}

/**
 * split_logical_operators - Splits a string into an array
 * of tokens based on the given delimiter.
 *
 * @string: The string to split.
 * @delimiter: The delimiter to split the string by.
 * Return: The array of tokens.
 */

char **split_logical_operators(const char *string, const char *delimiter)
{
	char **commands = (char **)malloc(MAX_PIPELINE_COMMANDS * sizeof(char *));

	if (commands == NULL)
	{
		return (NULL);
	}
	int command_count = 0;
	char *token = strtok((char *)string, delimiter);

	while (token != NULL && command_count < MAX_PIPELINE_COMMANDS)
	{
		commands[command_count] = strdup(token);
		command_count++;
		token = strtok(NULL, delimiter);
	}
	commands[command_count] = NULL;

	return (commands);
}

/**
 * execute_logical_operators - Executes a series of commands
 * based on the logical operators (&& or ||).
 *
 * @program_name: The name of the shell program.
 * @commands: The array of commands.
 * @and_operator: 1 if the logical operator is &&, 0 if it is ||.
 * Return: 1 if the commands should continue executing, 0 otherwise.
 */

int execute_logical_operators(char *program_name, char **commands, int and_operator)
{
	int command_index = 0;
	int exit_status = 0;
	int last_status;

	while (commands[command_index] != NULL)
	{
		char *command = commands[command_index];
		char *expanded_command = expand_variables(command, last_status);

		if (expanded_command == NULL)
		{
			fprintf(stderr, "Error: Failed to expand variables\n");
			return (1);
		}
		char **split_args = split_input(expanded_command);

		if (split_args == NULL)
		{
			fprintf(stderr, "Error splitting input\n");
			free(expanded_command);
			return (1);
		}
		int status = execute_command(program_name, split_args[0], split_args);

		free_split(split_args);
		if (and_operator && status != 0)
		{
			free(expanded_command);
			return (status);
		}
		else if (!and_operator && status == 0)
		{
			free(expanded_command);
			return (0);
		}
		free(expanded_command);
		command_index++;
	}
	return (exit_status);
}

/**
 * expand_variables - Expands variables in the given command
 * by replacing them with their values.
 *
 * @command: The command to expand variables in.
 * @last_status: status
 * Return: The expanded command.
 */

char *expand_variables(const char *command, int last_status)
{
	const char *pattern = "$?";
	char *expanded_command = strdup(command);
	char *match = strstr(expanded_command, pattern);
	char status[MAX_COMMAND_LENGTH];

	while (match != NULL)
	{
		snprintf(status, sizeof(status), "%d", last_status);
		expanded_command = replace_substring(expanded_command, pattern, status);
		match = strstr(expanded_command, pattern);
	}

	return (expanded_command);
}
