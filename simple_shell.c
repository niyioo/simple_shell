#include "shell.h"

/**
 * Executes a command with arguments.
 * If the command is "exit", the program terminates.
 * Otherwise, it forks a child process to execute the command.
 *
 * @param command The command to execute.
 * @param args The arguments for the command.
 * @return 0 on success, -1 on failure.
 */

int execute_command(char *program_name, char *command, char *args[]) {
	// ... (existing code)

	int exit_status = 0;

	if (strcmp(args[0], "exit") == 0) {
		if (args[1] != NULL) {
			execute_exit(args[1]);
		} else {
			exit(0);
		}
	}

	// Execute the command and check for errors
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0) {
		// Child process
		if (isatty(STDOUT_FILENO)) {
			if (dup2(STDOUT_FILENO, STDERR_FILENO) == -1) {
				fprintf(stderr, "%s: Error: Failed to redirect standard output\n", program_name);
				exit(EXIT_FAILURE);
			}
		}

		if (execvp(command, args) == -1) {
			fprintf(stderr, "%s: %s: command not found\n", program_name, command);
			exit(EXIT_FAILURE);
		}
	} else if (pid < 0) {
		// Forking error
		fprintf(stderr, "%s: Error: Failed to create child process\n", program_name);
		exit(EXIT_FAILURE);
	} else {
		// Parent process
		do {
			// Wait for the child process to finish
			if (waitpid(pid, &status, 0) == -1) {
				fprintf(stderr, "%s: Error: Waiting for child process failed\n", program_name);
				exit(EXIT_FAILURE);
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));

		// Check for command execution errors
		if (WIFSIGNALED(status)) {
			fprintf(stderr, "%s: %s: terminated by signal\n", program_name, command);
			exit_status = -1;
		} else {
			exit_status = WEXITSTATUS(status);
			if (exit_status != 0) {
				fprintf(stderr, "%s: %s: exited with status %d\n", program_name, command, exit_status);
			}
		}
	}

	return exit_status;
}

/**
 * Main - function of the shell program.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return The exit status of the program.
 */

int main(int argc, char *argv[]) {
	char command[MAX_COMMAND_LENGTH];
	char *args[MAX_ARGUMENTS];
	char *delimiters = " \t\n";
	int exit_shell = 0;
	char *program_name = argv[0];
	int last_status;

	if (argc > 1) {
		// ... (existing file execution logic)
	}

	while (!exit_shell) {
		printf("$ ");
		fflush(stdout);

		if (fgets(command, sizeof(command), stdin) == NULL) {
			printf("\n");
			exit(0);
		}

		command[strcspn(command, "\n")] = '\0';

		// Remove comments
		char *comment = strchr(command, '#');
		if (comment != NULL) {
			*comment = '\0';
		}

		// Split commands by ;
		char *token = strtok(command, ";");
		while (token != NULL) {
			// Handle variable replacement
			char *expanded_token = expand_variables(token, last_status);
			if (expanded_token == NULL) {
				fprintf(stderr, "Error: Failed to expand variables\n");
				token = strtok(NULL, ";");
				continue;
			}

			// Handle logical operators
			int execute_next = 1;
			if (strstr(expanded_token, "&&") != NULL) {
				char **commands = split_logical_operators(expanded_token, "&&");
				execute_next = execute_logical_operators(program_name, commands, 1);
				free_split(commands);
			} else if (strstr(expanded_token, "||") != NULL) {
				char **commands = split_logical_operators(expanded_token, "||");
				execute_next = execute_logical_operators(program_name, commands, 0);
				free_split(commands);
			} else {
				// Split commands by whitespace
				char *arg_token = strtok(expanded_token, delimiters);
				int arg_count = 0;

				while (arg_token != NULL && arg_count < MAX_ARGUMENTS - 1) {
					args[arg_count] = arg_token;
					arg_count++;
					arg_token = strtok(NULL, delimiters);
				}

				args[arg_count] = NULL;

				if (arg_count > 0) {
					// Handle special variables
					handle_special_variables(args);

					if (strcmp(args[0], "alias") == 0) {
						execute_alias(args[1], args [2]);
					} else if (strcmp(args[0], "env") == 0) {
						execute_env();
					} else if (strcmp(args[0], "exit") == 0) {
						if (args[1] != NULL) {
							execute_exit(args[1]);
						} else {
							exit_shell = 1;
						}
					} else if (strcmp(args[0], "setenv") == 0) {
						execute_setenv(args[1], args[2]);
					} else if (strcmp(args[0], "unsetenv") == 0) {
						execute_unsetenv(args[1]);
					} else if (strcmp(args[0], "cd") == 0) {
						execute_cd(args[1]);
					} else {
						if (execute_command(program_name, args[0], args) == -1) {
							fprintf(stderr, "%s: command not found: %s\n", program_name, args[0]);
						}
					}
				}
			}

			free(expanded_token);

			token = strtok(NULL, ";");
			if (execute_next == 0) {
				break; // Skip execution of subsequent commands after failed logical operator check
			}
		}
	}

	return 0;
}
