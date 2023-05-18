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
	if (input == NULL || *input == '\0')
		return (NULL);

	char **arguments = NULL;
	char *token;
	int count = 0;

	char *input_copy = strdup(input);
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

	int i = 0, j = 0;
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
 * Handles special shell variables ($? and $$) in the given arguments.
 *
 * @param args The array of arguments to handle.
 */
void handle_special_variables(char *args[]) {
	int i = 0;
	while (args[i] != NULL) {
		if (strcmp(args[i], "$?") == 0) {
			// Replace $? with the exit status of the previous command
			// ... (implement code to retrieve exit status and replace args[i])
		} else if (strcmp(args[i], "$$") == 0) {
			// Replace $$ with process ID
			char pid_str[16];
			snprintf(pid_str, sizeof(pid_str), "%d", getpid());
			free(args[i]);
			args[i] = strdup(pid_str);
		}

		i++;
	}
}

/**
 * Replaces all occurrences of a substring with another substring in a given string.
 * The function dynamically allocates memory for the result string, which needs to be freed by the caller.
 *
 * @param string The original string.
 * @param pattern The substring to be replaced.
 * @param replacement The substring to replace occurrences of the pattern.
 * @return The result string with replaced substrings, or NULL on failure.
 */
char *replace_substring(const char *string, const char *pattern, const char *replacement)
{
	char *result;
	int string_length = strlen(string);
	int pattern_length = strlen(pattern);
	int replacement_length = strlen(replacement);
	int count = 0;
	const char *p;

	// Count the number of occurrences of the pattern
	for (p = string; (p = strstr(p, pattern)) != NULL; p++)
		count++;

	// Allocate memory for the result string
	result = (char *)malloc(string_length + count * (replacement_length - pattern_length) + 1);
	if (result == NULL)
	{
		fprintf(stderr, "Error: Failed to allocate memory\n");
		return NULL;
	}

	// Copy the original string, replacing occurrences of the pattern
	char *dest = result;
	const char *src = string;

	while (count > 0)
	{
		const char *match = strstr(src, pattern);
		if (match == NULL)
		{
			strcpy(dest, src);
			break;
		}
		int length = match - src;
		strncpy(dest, src, length);
		dest += length;
		strcpy(dest, replacement);
		dest += replacement_length;
		src = match + pattern_length;
		count--;
	}

	strcpy(dest, src);

	return result;
}
