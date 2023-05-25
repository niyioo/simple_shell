#include "shell.h"

/**
 * initialize_command - starts executing everything
 * @current_command: current command token
 * @type_command: type of command
 *
 * Return: void
 */
void initialize_command(char **current_command, int type_command)
{
	pid_t pid;

	if (type_command == EXTERNAL_COMMAND || type_command == PATH_COMMAND)
	{
		pid = fork();
		if (pid == 0)
			execute_command(current_command, type_command);
		else
		{
			waitpid(pid, &status, 0);
			status >>= 8;
		}
	}
	else
		execute_command(current_command, type_command);
}

/**
 * builtin_exit - Exits the shell
 * @command: The command and arguments passed (exit and optional exit status)
 */
void builtin_exit(char **command)
{
	int exit_status = 0;

	if (command[1] != NULL)
		exit_status = atoi(command[1]);

	exit(exit_status);
}

/**
 * _snprintf - Writes formatted data to a string
 * @str: Destination string
 * @size: Maximum number of characters to write
 * @format: Format string
 * @...: Additional arguments
 *
 * Return: Number of characters written excluding the null byte
 *         or a negative value if an error occurred
 */
int _snprintf(char *str, size_t size, const char *format, ...)
{
	va_list args;
	int result;

	va_start(args, format);
	result = vsnprintf(str, size, format, args);
	va_end(args);

	if (result < 0 || result >= (int)size)
	{
		return (-1);
	}

	return (result);
}

/**
 * _strstr - Locates a substring
 * @haystack: The string to search in
 * @needle: The substring to find
 *
 * Return: Pointer to the beginning of the found substring or NULL if not found
 */
char *_strstr(const char *haystack, const char *needle)
{
	const char *h, *n;

	if (*needle == '\0')
		return ((char *)haystack);

	while (*haystack != '\0')
	{
		h = haystack;
		n = needle;

		while (*n != '\0' && *h == *n)
		{
			h++;
			n++;
		}

		if (*n == '\0')
			return ((char *)haystack);

		haystack++;
	}

	return (NULL);
}

/**
 * execute_alias - Executes the alias command
 * @name: The alias name
 * @value: The alias value
 */
void execute_alias(char *name, char *value)
{
	char **env, *alias;
	int result;

	if (name == NULL)
	{
		for (env = environ; *env != NULL; env++)
		{
			alias = _strstr(*env, "alias");

			if (alias != NULL)
			{
				print_string(alias, STDERR_FILENO);
				print_string("\n", STDERR_FILENO);
			}
		}
	}
	else if (value == NULL)
	{
		for (env = environ; *env != NULL; env++)
		{
			alias = _strstr(*env, name);

			if (alias != NULL)
			{
				print_string(alias, STDERR_FILENO);
				print_string("\n", STDERR_FILENO);
				break;
			}
		}
	}
	else
	{
		char alias[MAX_COMMAND_LENGTH];

		result = _snprintf(alias, sizeof(alias), "alias %s='%s'", name, value);

		if (result > 0 && result < MAX_COMMAND_LENGTH)
			putenv(alias);
	}
}
