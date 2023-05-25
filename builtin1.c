#include "shell.h"

/**
 * builtin_cd - Change the current directory
 * @command: Command arguments (cd [DIRECTORY])
 */
void builtin_cd(char **command)
{
	char *path;

	if (command[1] == NULL || strcmp(command[1], "~") == 0)
		path = getenv("HOME");
	else if (strcmp(command[1], "-") == 0)
		path = getenv("OLDPWD");
	else
		path = command[1];

	if (path == NULL)
	{
		fprintf(stderr, "cd: no HOME or OLDPWD variable set\n");
		return;
	}

	if (chdir(path) == -1)
	{
		perror("cd");
		return;
	}

	setenv("OLDPWD", getenv("PWD"), 1);
	setenv("PWD", getcwd(NULL, 0), 1);
}

/**
 * handle_logical_operators - Handles shell logical operators (&& and ||)
 * @command: The current command
 * @prev_status: The exit status of the previous command
 *
 * Return: 1 if the command should be executed, 0 if it should be skipped
 */
int handle_logical_operators(char **command, int prev_status)
{
	if (strcmp(command[0], "&&") == 0)
	{
		if (prev_status != 0)
			return (0);
	}
	else if (strcmp(command[0], "||") == 0)
	{
		if (prev_status == 0)
			return (0);
	}
	return (1);
}

/**
 * replace_variables - Replaces variables in the command with their values
 * @command: The current command
 */
void replace_variables(char **command)
{
	int i = 0;

	while (command[i] != NULL)
	{
		if (command[i][0] == '$')
		{
			char *var_name = command[i] + 1;
			char *var_value = get_variable_value(var_name);

			if (var_value != NULL)
			{
				free(command[i]);
				command[i] = strdup(var_value);
			}
		}
		i++;
	}
}

/**
 * set_special_variable - Set the value of a special variable
 * @var_name: Name of the variable
 * @value: Value to set
 */
void set_special_variable(const char *var_name, int value)
{
	char str_value[12];

	print_string("%d", value);

	setenv(var_name, str_value, 1);
}

/**
 * get_variable_value - Retrieves the value of a variable given its name.
 * @var_name: The name of the variable to retrieve the value for.
 *
 * Return: A pointer to the value of the variable if found, otherwise NULL.
 */
char *get_variable_value(const char *var_name)
{
	if (strcmp(var_name, "var1") == 0)
	{
		return ("Value of var1");
	}
	else if (strcmp(var_name, "var2") == 0)
	{
		return ("Value of var2");
	}
	else
	{
		return (NULL);
	}
}
