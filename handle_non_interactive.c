#include "shell.h"

/**
 * handle_non_interactive - handles non-interactive mode
 *
 * Return: void
 */
void handle_non_interactive(void)
{
	char **current_command = NULL;
	int i, command_type = 0;
	size_t buffer_size = 0;

	if (!(isatty(STDIN_FILENO)))
	{
		while (getline(&line, &buffer_size, stdin) != -1)
		{
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
				command_type = determine_command_type(current_command[0]);
				initialize_command(current_command, command_type);
				free(current_command);
			}
			free(commands);
		}
		free(line);
		exit(status);
	}
}
