#include "shell.h"

/**
 * execute_command - Executes a command with arguments.
 * If the command is "exit", the program terminates.
 * Otherwise, it forks a child process to execute the command.
 *
 * @command: The command to execute.
 * @args: The arguments for the command.
 * Return: 0 on success, -1 on failure.
 */

int execute_command(char *program_name, char *command, char *args[])
{
	int exit_status = 0;
	pid_t pid;
	int status;

	if (_strcmp(args[0], "exit") == 0)
	{
		if (args[1] != NULL)
		{
			exit_status = _atoi(args[1]);
			exit(exit_status);
		}
		else
		{
			exit(0);
		}
	}
	/* Execute the command and check for errors */
	pid = fork();
	if (pid == 0)
	{
		/* Child process */
		if (isatty(STDOUT_FILENO))
		{
			if (dup2(STDOUT_FILENO, STDERR_FILENO) == -1)
			{
				_printf("%s: Error: Failed to redirect standard output\n", program_name);
				exit(EXIT_FAILURE);
			}
		}
		if (execvp(command, args) == -1)
		{
			_printf("%s: %s: command not found\n", program_name, command);
			exit(EXIT_FAILURE);
		}
	}
	else if (pid < 0)
	{
		/* Forking error */
		_printf("%s: Error: Failed to create child process\n", program_name);
		exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process */
		do {
			/* Wait for the child process to finish */
			if (waitpid(pid, &status, 0) == -1)
			{
				_printf("%s: Error: Waiting for child process failed\n", program_name);
				exit(EXIT_FAILURE);
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));

		/* Check for command execution errors */
		if (WIFSIGNALED(status))
		{
			_printf("%s: %s: terminated by signal\n", program_name, command);
			exit_status = -1;
		}
		else
		{
			exit_status = WEXITSTATUS(status);
			if (exit_status != 0)
			{
				_printf("%s: %s: exited with status %d\n", program_name, command, exit_status);
			}
		}
	}
	return (exit_status);
}

/**
 * Main - function of the shell program.
 *
 * @argc: The number of command-line arguments.
 * @argv: The array of command-line arguments.
 * Return: The exit status of the program.
 */

int main(int argc, char *argv[])
{
	char command[MAX_COMMAND_LENGTH]; /* Buffer to hold the user's command */
	char *args[MAX_ARGUMENTS]; /* Array to store the parsed command arguments */
	char *delimiters = " \t\n"; /* Delimiters used to tokenize the command */
	int exit_shell = 0; /* Flag to determine whether to exit the shell */
	char *program_name = argv[0]; /* Name of the shell program */
	int last_status = 0;
	char *comment;
	char *token;
	int execute_next;

	if (argc > 1)
	{
		FILE *file = fopen(argv[1], "r"); /* Open the file in read mode */

		if (file == NULL)
		{
			_printf("Error: Failed to open file '%s'\n", argv[1]);
			exit(1);
		}

		while (fgets(command, sizeof(command), file) != NULL)
		{
			command[_strcspn(command, "\n")] = '\0'; /* Remove the trailing newline character */
			comment = _strchr(command, '#'); /* Check for comments */

			if (comment != NULL)
			{
				*comment = '\0'; /* Ignore everything after the comment character */
			}

			token = _strtok(command, ";"); /* Tokenize the command by semicolon */

			while (token != NULL)
			{
				char *expanded_token = expand_variables(token, last_status); /* Expand variables in the token */

				if (expanded_token == NULL)
				{
					_printf("Error: Failed to expand variables\n");
					token = _strtok(NULL, ";");
					continue;
				}

				execute_next = 1;

				if (_strstr(expanded_token, "&&") != NULL)
				{
					char **commands = split_logical_operators(expanded_token, "&&"); /* Split commands by logical AND */
					execute_next = execute_logical_operators(program_name, commands, 1); /* Execute commands with logical AND */
					free_split(commands);
				}
				else if (_strstr(expanded_token, "||") != NULL)
				{
					char **commands = split_logical_operators(expanded_token, "||"); /* Split commands by logical OR */
					execute_next = execute_logical_operators(program_name, commands, 0); /* Execute commands with logical OR */
					free_split(commands);
				}
				else
				{
					char *arg_token = _strtok(expanded_token, delimiters); /* Tokenize the expanded token by whitespace */
					int arg_count = 0;

					while (arg_token != NULL && arg_count < MAX_ARGUMENTS - 1)
					{
						args[arg_count] = arg_token; /* Store each argument in the args array */
						arg_count++;
						arg_token = _strtok(NULL, delimiters);
					}

					args[arg_count] = NULL; /* Null-terminate the args array */

					if (arg_count > 0)
					{
						handle_special_variables(args, last_status); /* Handle special variables */

						if (_strcmp(args[0], "alias") == 0)
							execute_alias(args[1], args[2]); /* Execute alias command */
						else if (_strcmp(args[0], "env") == 0)
							execute_env(); /* Execute env command */
						else if (_strcmp(args[0], "exit") == 0)
						{
							if (args[1] != NULL)
								execute_exit(args[1]); /* Execute exit command with an argument */
							else
								exit_shell = 1; /* Set exit_shell flag to exit the shell */
						}
						else if (_strcmp(args[0], "setenv") == 0)
							execute_setenv(args[1], args[2]); /* Execute setenv command */
						else if (_strcmp(args[0], "unsetenv") == 0)
							execute_unsetenv(args[1]); /* Execute unsetenv command */
						else if (_strcmp(args[0], "cd") == 0)
							execute_cd(args[1]); /* Execute cd command */
						else
						{
							if (execute_command(program_name, args[0], args) == -1)
								_printf("%s: command not found: %s\n", program_name, args[0]); /* Command not found */
						}
					}
				}

				free(expanded_token);
				token = _strtok(NULL, ";");

				if (execute_next == 0)
					break;
			}
		}

		fclose(file); /* Close the file */
	}
	else
	{
		while (!exit_shell)
		{
			_printf("$ ");
			fflush(stdout);

			if (fgets(command, sizeof(command), stdin) == NULL)
			{
				_printf("\n");
				exit(0);
			}

			command[_strcspn(command, "\n")] = '\0'; /* Remove the trailing newline character */
			comment = _strchr(command, '#'); /* Check for comments */

			if (comment != NULL)
			{
				*comment = '\0'; /* Ignore everything after the comment character */
			}

			token = _strtok(command, ";"); /* Tokenize the command by semicolon */

			while (token != NULL)
			{
				char *expanded_token = expand_variables(token, last_status); /* Expand variables in the token */

				if (expanded_token == NULL)
				{
					_printf("Error: Failed to expand variables\n");
					token = _strtok(NULL, ";");
					continue;
				}

				execute_next = 1;

				if (_strstr(expanded_token, "&&") != NULL)
				{
					char **commands = split_logical_operators(expanded_token, "&&"); /* Split commands by logical AND */
					execute_next = execute_logical_operators(program_name, commands, 1); /* Execute commands with logical AND */
					free_split(commands);
				}
				else if (_strstr(expanded_token, "||") != NULL)
				{
					char **commands = split_logical_operators(expanded_token, "||"); /* Split commands by logical OR */
					execute_next = execute_logical_operators(program_name, commands, 0); /* Execute commands with logical OR */
					free_split(commands);
				}
				else
				{
					char *arg_token = _strtok(expanded_token, delimiters); /* Tokenize the expanded token by whitespace */
					int arg_count = 0;

					while (arg_token != NULL && arg_count < MAX_ARGUMENTS - 1)
					{
						args[arg_count] = arg_token; /* Store each argument in the args array */
						arg_count++;
						arg_token = _strtok(NULL, delimiters);
					}

					args[arg_count] = NULL; /* Null-terminate the args array */

					if (arg_count > 0)
					{
						handle_special_variables(args, last_status); /* Handle special variables */

						if (_strcmp(args[0], "alias") == 0)
							execute_alias(args[1], args[2]); /* Execute alias command */
						else if (_strcmp(args[0], "env") == 0)
							execute_env(); /* Execute env command */
						else if (_strcmp(args[0], "exit") == 0)
						{
							if (args[1] != NULL)
								execute_exit(args[1]); /* Execute exit command with an argument */
							else
								exit_shell = 1; /* Set exit_shell flag to exit the shell */
						}
						else if (_strcmp(args[0], "setenv") == 0)
							execute_setenv(args[1], args[2]); /* Execute setenv command */
						else if (_strcmp(args[0], "unsetenv") == 0)
							execute_unsetenv(args[1]); /* Execute unsetenv command */
						else if (_strcmp(args[0], "cd") == 0)
							execute_cd(args[1]); /* Execute cd command */
						else
						{
							if (execute_command(program_name, args[0], args) == -1)
								_printf("%s: command not found: %s\n", program_name, args[0]); /* Command not found */
						}
					}
				}

				free(expanded_token);
				token = _strtok(NULL, ";");

				if (execute_next == 0)
					break;
			}
		}
	}

	return (0); /* Exit the program */
}
