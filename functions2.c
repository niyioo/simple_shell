#include "shell.h"

/**
 * execute_cd - Execute the "cd" command to change the current directory.
 * @directory: Directory to change to. If NULL, changes to the home directory.
 *
 * Description: This function changes the current directory of the process. If
 * no argument is provided, it changes to the home directory. It also updates
 * the value of the "PWD" environment variable.
 */

void execute_cd(char *directory)
{
	if (directory == NULL)
	{
		directory = getenv("HOME");
	}
	else if (_strcmp(directory, "-") == 0)
	{
		directory = getenv("OLDPWD");
		_printf("%s\n", directory);
	}

	if (chdir(directory) != 0)
	{
		_printf("Error: Failed to change directory\n");
	}
	else
	{
		char cwd[MAX_COMMAND_LENGTH];
		getcwd(cwd, sizeof(cwd));
		setenv("PWD", cwd, 1);
		setenv("OLDPWD", cwd, 1);
	}
}

/**
 * execute_alias - Execute the "alias" command to handle aliases.
 * @name: Name of the alias. If NULL, prints all aliases.
 * @value: Value of the alias. If NULL, prints the value of the given alias.
 *
 * Description: This function handles the "alias" command. If no arguments are
 * provided, it prints all defined aliases. If a name is provided, it prints
 * the value of that alias. If both name and value are provided, it defines or
 * modifies an alias with the given name and value.
 */
void execute_alias(char *name, char *value)
{
	char **env;

	if (name == NULL)
	{
		for (env = environ; *env != NULL; env++)
		{
			char *alias = _strstr(*env, "alias");

			if (alias != NULL)
			{
				_printf("%s\n", alias);
			}
		}
	}
	else if (value == NULL)
	{
		for (env = environ; *env != NULL; env++)
		{
			char *alias = _strstr(*env, name);

			if (alias != NULL)
			{
				_printf("%s\n", alias);
				break;
			}
		}
	}
	else
	{
		char alias[MAX_COMMAND_LENGTH];

		_printf(alias, sizeof(alias), "alias %s='%s'", name, value);
		putenv(alias);
	}
}

/**
 * replace_variable - Replace variables in the input with their values.
 * @input: Input string containing variables to replace.
 *
 * Return: The output string with variables replaced.
 *
 * Description: This function replaces variables in the input string with their
 * respective values. It supports replacing "$?" with the exit status of the
 * last command and "$$" with the process ID.
 */
char *replace_variable(char *input)
{
	char *output = malloc(MAX_COMMAND_LENGTH);
	int index = 0;
	int length = _strlen(input);
	int i = 0;
	int status = 0;

	while (i < length)
	{
		if (input[i] == '$')
		{
			i++;
			if (i < length && input[i] == '?')
			{
				int status = 0;

				_printf("%d", status);
				index += _printf("%d", status);
			}
			else if (i < length && input[i] == '$')
			{
				pid_t pid = getpid();

				_printf("%d", pid);
				index += _printf("%d", status);
			}
		}
		else
		{
			output[index++] = input[i++];
		}
	}

	output[index] = '\0';
	return (output);
}

/**
 * replace_variables - Replaces variables in the argument array
 * with their corresponding values.
 *
 * @args: The argument array to replace variables in.
 */

void replace_variables(char *args[])
{
	int i;

	for (i = 0; args[i] != NULL; i++)
	{
		if (_strcmp(args[i], "$?") == 0)
		{
			int status = WEXITSTATUS(system(NULL));
			char exit_status[16];

			_printf(exit_status, "%d", status);
			free(args[i]);
			args[i] = _strdup(exit_status);
		}
		else if (_strcmp(args[i], "$$") == 0)
		{
			pid_t pid = getpid();
			char process_id[16];

			_printf(process_id, "%d", pid);
			free(args[i]);
			args[i] = _strdup(process_id);
		}
	}
}
