#include "shell.h"

/**
 * main - shell program
 * @argc: arguments
 * @argv: arguments
 *
 * Return: 0
 */
int main(int argc __attribute__((unused)), char **argv)
{
	char **commands = NULL;
	char *line = NULL;
	int status = 0;
	char **current_command = NULL;
	int i, type_command = 0;
	size_t n = 0;

	signal(SIGINT, handle_ctrl_c);
	shell_name = argv[0];

	while (1)
	{
		handle_non_interactive();
		print_string(" ($) ", STDOUT_FILENO);
		if (getline(&line, &n, stdin) == -1)
		{
			free(line);
			exit(status);
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
			free(current_command);
		}
		free(commands);
	}
	free(line);
	return (status);
}
