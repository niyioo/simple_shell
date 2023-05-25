#include "shell.h"

/**
 * determine_command_type - determines the type of the command
 * @command: command to be parsed
 *
 * Return: constant representing the type of the command
 */
int determine_command_type(char *command)
{
	int i;
	char *internal_commands[] = {"env", "exit", NULL};
	char *path = NULL;

	for (i = 0; command[i] != '\0'; i++)
	{
		if (command[i] == '/')
			return (EXTERNAL_COMMAND);
	}

	for (i = 0; internal_commands[i] != NULL; i++)
	{
		if (_strcmp(command, internal_commands[i]) == 0)
			return (INTERNAL_COMMAND);
	}

	/* search_command_path - checks if a command is found in the PATH */
	path = search_command_path(command);
	if (path != NULL)
	{
		free(path);
		return (PATH_COMMAND);
	}

	return (INVALID_COMMAND);
}

/**
 * execute_command - executes a command based on its type
 * @tokenized_command: tokenized form of the command (ls -l == {ls, -l, NULL})
 * @command_type: type of the command
 *
 * Return: void
 */
void execute_command(char **tokenized_command, int command_type)
{
	void (*func)(char **command);

	if (command_type == EXTERNAL_COMMAND)
	{
		if (execve(tokenized_command[0], tokenized_command, NULL) == -1)
		{
			perror(_getenv("PWD"));
			exit(2);
		}
	}
	if (command_type == PATH_COMMAND)
	{
		if (execve(search_command_path(tokenized_command[0]),
					tokenized_command, NULL) == -1)
		{
			perror(_getenv("PWD"));
			exit(2);
		}
	}
	if (command_type == INTERNAL_COMMAND)
	{
		func = get_internal_func(tokenized_command[0]);
		func(tokenized_command);
	}
	if (command_type == INVALID_COMMAND)
	{
		print_string(shell_name, STDERR_FILENO);
		print_string(": 1: ", STDERR_FILENO);
		print_string(tokenized_command[0], STDERR_FILENO);
		print_string(": not found\n", STDERR_FILENO);
		status = 127;
	}
}

/**
 * get_internal_func - retrieves a function based on
 * the command given and a mapping
 * @command: string to check against the mapping
 *
 * Return: pointer to the proper function, or NULL on failure
 */
void (*get_internal_func(char *command))(char **)
{
	int i;

	function_map mapping[] = {
		{"env", custom_env}, {"exit", builtin_exit}
	};

	for (i = 0; i < 2; i++)
	{
		if (_strcmp(command, mapping[i].command_name) == 0)
			return (mapping[i].func);
	}
	return (NULL);
}

/**
 * search_command_path - checks if a command is found in the PATH
 * @command: command to be searched
 *
 * Return: path where the command is found, NULL if not found
 */
char *search_command_path(char *command)
{
	char **path_dirs = NULL;
	char *path_copy, *full_path, *temp_dir;
	char *path = _getenv("PATH");
	int i;

	if (path == NULL || _strlen(path) == 0)
		return (NULL);

	path_copy = malloc(sizeof(char) * (_strlen(path) + 1));
	_strcpy(path, path_copy);

	path_dirs = custom_tokenizer(path_copy, ":");
	for (i = 0; path_dirs[i] != NULL; i++)
	{
		temp_dir = _strcat(path_dirs[i], "/");
		full_path = _strcat(temp_dir, command);

		if (access(full_path, F_OK) == 0)
		{
			free(temp_dir);
			free(path_copy);
			free(path_dirs);
			return (full_path);
		}

		free(full_path);
		free(temp_dir);
	}

	free(path_copy);
	free(path_dirs);
	return (NULL);
}

/**
 * _getenv - gets the value of an environment variable
 * @name: name of the environment variable
 *
 * Return: the value of the variable as a string, or NULL if not found
 */
char *_getenv(char *name)
{
	char **environment;
	char *pair;
	char *name_copy;

	for (environment = environ; *environment != NULL; environment++)
	{
		for (pair = *environment, name_copy = name;
				*pair == *name_copy; pair++, name_copy++)
		{
			if (*pair == '=')
				break;
		}
		if ((*pair == '=') && (*name_copy == '\0'))
			return (pair + 1);
	}

	return (NULL);
}
