#include "shell.h"

/**
 * main - Shell program
 * @argc: Number of arguments
 * @argv: Array of arguments
 *
 * Return: 0 on success
 */
int main(int argc __attribute__((unused)), char **argv)
{
	char **commands = NULL, *line = NULL, **current_command = NULL;
	int status = 0, i, type_command = 0, shell_status = 0;

	signal(SIGINT, handle_ctrl_c);
	shell_name = argv[0];
	while (1)
	{
		handle_non_interactive();
		print_string(" ($) ", STDOUT_FILENO);
		line = custom_getline();
		if (!line)
		{
			free(line);
			exit(shell_status);
		}
		custom_remove_newline(line);
		ignore_comment(line);
		commands = custom_tokenizer(line, ";");
		for (i = 0; commands[i] != NULL; i++)
		{
			current_command = custom_tokenizer(commands[i], " ");
			if (current_command[0] == NULL)
			{
				free(current_command);
				break;
			}
			type_command = determine_command_type(current_command[0]);
			initialize_command(current_command, type_command);
			if (_strcmp(current_command[0], "setenv") == 0)
				builtin_setenv(current_command);
			else if (_strcmp(current_command[0], "unsetenv") == 0)
				builtin_unsetenv(current_command);
			else if (_strcmp(current_command[0], "cd") == 0)
				builtin_cd(current_command);
			free(current_command);
		}
		free(commands);
	}
	free(line);
	return (status);
}
