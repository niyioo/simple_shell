#include "shell.h"

/**
 * _getenv - gets an environment variable
 * @name: name of the environment variable to get
 *
 * Return: value of the environment variable
 */
char *_getenv(const char *name)
{
	int i = 0;
	size_t name_len = strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, name_len) == 0 && environ[i][name_len] == '=')
		{
			return environ[i] + name_len + 1;
		}
		i++;
	}

	return (NULL);
}

/**
 * find_path - looks for PATH in the environment
 *
 * Return: pointer to the PATH string in the environment or NULL if not found
 */
char *find_path(void)
{
	return _getenv("PATH");
}

/**
 * split_path - Split the PATH variable into an array of directories.
 *
 * @path: PATH string to split
 *
 * Return: Array of directory names.
 */
char **split_path(const char *path)
{
	// Check for NULL or empty path
	if (path == NULL || *path == '\0')
		return (NULL);

	char **directories = NULL;
	char *token;
	int count = 0;

	// Count the number of directories
	char *path_copy = strdup(path);
	if (path_copy == NULL)
		return (NULL);

	token = strtok(path_copy, ":");
	while (token != NULL)
	{
		count++;
		token = strtok(NULL, ":");
	}
	free(path_copy);

	// Allocate memory for the directory array
	directories = malloc((count + 1) * sizeof(char *));
	if (directories == NULL)
		return (NULL);

	// Split the path and store directories in the array
	int i = 0;
	path_copy = strdup(path);
	if (path_copy == NULL)
	{
		free(directories);
		return (NULL);
	}

	token = strtok(path_copy, ":");
	while (token != NULL)
	{
		directories[i] = strdup(token);
		if (directories[i] == NULL)
		{
			// Free previously allocated directories
			for (int j = 0; j < i; j++)
				free(directories[j]);
			free(directories);
			free(path_copy);
			return (NULL);
		}
		i++;
		token = strtok(NULL, ":");
	}
	directories[i] = NULL;
	free(path_copy);

	return (directories);
}

/**
 * _path - Get the full path of a command.
 *
 * @args: Arguments to the command.
 *
 * Return: Full path of the command on success, or NULL on failure.
 */
char *_path(char **args)
{
	char *path = find_path();  // Get the PATH variable
	if (path == NULL)
	{
		print_error(args[0], "command not found");
		return (NULL);
	}

	char **directories = split_path(path);  // Split the PATH into directories
	if (directories == NULL)
	{
		print_error(args[0], "failed to split PATH");
		return (NULL);
	}

	char *command = args[0];  // Get the command name

	// Check if the command exists in any of the directories
	for (int i = 0; directories[i] != NULL; i++)
	{
		char *full_path = str_concat(directories[i], command, '/');
		if (full_path == NULL)
		{
			print_error(args[0], "failed to concatenate path");
			free_split(directories);
			return NULL;
		}

		// Check if the command file exists
		if (access(full_path, X_OK) == 0)
		{
			free_split(directories);
			return full_path;
		}

		free(full_path);
	}

	print_error(args[0], "command not found");
	free_split(directories);
	return (NULL);
}

/**
 * str_concat - Concatenate two strings.
 *
 * @str1: First string.
 * @str2: Second string.
 * @delimiter: Delimiter character.
 *
 * Return: Concatenated string, or NULL on failure.
 */
char *str_concat(const char *str1, const char *str2, char delimiter)
{
	if (str1 == NULL || str2 == NULL)
		return (NULL);

	size_t str1_len = strlen(str1);
	size_t str2_len = strlen(str2);

	char *concatenated = malloc(str1_len + str2_len + 2);
	if (concatenated == NULL)
		return (NULL);

	// Copy str1 to the concatenated string
	strcpy(concatenated, str1);

	// Add the delimiter character
	concatenated[str1_len] = delimiter;

	// Copy str2 to the concatenated string
	strcpy(concatenated + str1_len + 1, str2);

	return (concatenated);
}

/**
 * free_split - Free memory allocated for a string array.
 *
 * @arr: Array to free.
 */
void free_split(char **arr)
{
	if (arr == NULL)
		return;

	for (int i = 0; arr[i] != NULL; i++)
		free(arr[i]);

	free(arr);
}

/**
 * execute - Execute a given command with arguments.
 *
 * @args: Arguments to the command to execute.
 *
 * Return: 0 on success, -1 on failure.
 */
int execute(char **args)
{
	pid_t pid;
	int status;

	if (args[0][0] != '/')
	{
		char *full_path = _path(args);
		if (full_path == NULL)
		{
			print_error(args[0], "not found");
			return -1;
		}
		args[0] = full_path;
	}

	pid = fork();
	if (pid < 0)
	{
		perror("Error");
		return -1;
	}
	else if (pid == 0)
	{
		if (execve(args[0], args, environ) == -1)
			perror("Error");
		exit(EXIT_FAILURE);
	}
	else
	{
		do
		{
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return (0);
}

/**
 * print_error - Print an error message.
 *
 * @cmd: Command name that caused the error.
 * @msg: Error message to print.
 */
void print_error(const char *cmd, const char *msg)
{
	fprintf(stderr, "%s: %s\n", cmd, msg);
}
