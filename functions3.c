#include "shell.h"

/**
 * split_input - Split the input string into an array of arguments.
 *
 * @input: Input string to split.
 *
 * Return: Array of arguments.
 */

char **split_input(char *input)
{
	char **arguments = NULL;
	char *token;
	int count = 0;
	char *input_copy = strdup(input);
	int i = 0, j = 0;

	if (input == NULL || *input == '\0')
		return (NULL);

	if (input_copy == NULL)
		return (NULL);
	token = strtok(input_copy, DELIMITER);
	while (token != NULL)
	{
		count++;
		token = strtok(NULL, DELIMITER);
	}
	free(input_copy);
	arguments = malloc((count + 1) * sizeof(char *));
	if (arguments == NULL)
		return (NULL);

	input_copy = strdup(input);
	if (input_copy == NULL)
	{
		free(arguments);
		return (NULL);
	}
	token = strtok(input_copy, DELIMITER);
	while (token != NULL)
	{
		arguments[i] = strdup(token);
		if (arguments[i] == NULL)
		{
			for (j = 0; j < i; j++)
				free(arguments[j]);
			free(arguments);
			free(input_copy);
			return (NULL);
		}
		i++;
		token = strtok(NULL, DELIMITER);
	}
	arguments[i] = NULL;
	free(input_copy);
	if (strchr(arguments[0], '/') == NULL)
	{
		char *path = find_path();

		if (path != NULL)
		{
			char **path_dirs = split_path(path);

			if (path_dirs != NULL)
			{
				for (j = 0; path_dirs[j] != NULL; j++)
				{
					char *command_path = str_concat(path_dirs[j], arguments[0], '/');

					if (command_path != NULL && access(command_path, X_OK) == 0)
					{
						free(arguments[0]);
						arguments[0] = command_path;
						break;
					}
					free(command_path);
				}
				free_split(path_dirs);
			}
		}
	}
	return (arguments);
}

/**
 * prompt - Display the shell prompt.
 */
void prompt(void)
{
	printf("$ ");
	fflush(stdout);
}

/**
 * handle_special_variables - Handles special shell variables
 * ($? and $$) in the given arguments.
 *
 * @args: The array of arguments to handle.
 * @last_status: status
 */
void handle_special_variables(char *args[], int last_status)
{
	int i = 0;

	while (args[i] != NULL)
	{
		if (strcmp(args[i], "$?") == 0)
		{
			char status_str[16];

			snprintf(status_str, sizeof(status_str), "%d", last_status);
			strcpy(args[i], status_str);
		}
		else if (strcmp(args[i], "$$") == 0)
		{
			char pid_str[16];

			snprintf(pid_str, sizeof(pid_str), "%d", getpid());
			strcpy(args[i], pid_str);
		}
		else if (strcmp(args[i], "$PATH") == 0)
		{
			char *path_value = getenv("PATH");

			if (path_value != NULL)
			{
				strcpy(args[i], path_value);
			}
		}
		else if (strcmp(args[i], "\"$PATH\"") == 0)
		{
			char *path_value = getenv("PATH");

			if (path_value != NULL)
			{
				int len = strlen(path_value);
				char *quoted_path = malloc(len + 3);

				if (quoted_path != NULL)
				{
					snprintf(quoted_path, len + 3, "\"%s\"", path_value);
					strcpy(args[i], quoted_path);
					free(quoted_path);
				}
			}
		}
		i++;
	}
}

/**
 * replace_substring - Replaces all occurrences of a substring
 * with another substring in a given string.
 * The function dynamically allocates memory for the result string,
 * which needs to be freed by the caller.
 *
 * @string: The original string.
 * @pattern: The substring to be replaced.
 * @replacement: The substring to replace occurrences of the pattern.
 * Return: The result string with replaced substrings
 * or NULL on failure.
 */

char *replace_substring(const char *string, const char *pattern, const char *replacement)
{
	char *result;
	int string_length = strlen(string);
	int pattern_length = strlen(pattern);
	int replacement_length = strlen(replacement);
	int count = 0;
	const char *src;
	const char *p;
	char *dest;
	int length;

	for (p = string; (p = strstr(p, pattern)) != NULL; p++)
		count++;
	result = (char *)malloc(string_length + count *
			(replacement_length - pattern_length) + 1);
	if (result == NULL)
	{
		fprintf(stderr, "Error: Failed to allocate memory\n");
		return (NULL);
	}
	dest = result;
	src = string;

	while (count > 0)
	{
		const char *match = strstr(src, pattern);

		if (match == NULL)
		{
			strcpy(dest, src);
			break;
		}
		length = match - src;

		strncpy(dest, src, length);
		dest += length;
		strcpy(dest, replacement);
		dest += replacement_length;
		src = match + pattern_length;
		count--;
	}
	strcpy(dest, src);
	return (result);
}
