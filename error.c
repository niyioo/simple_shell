#include "shell.h"

/**
 * error_main - Entry point for the error program
 *
 * Return: Always returns 0
 */

int error_main(void)
{
	char *buffer = NULL;
	size_t bufsize = 0;
	ssize_t nread;
	pid_t pid;

	while (1)
	{
		_printf("$ ");
		nread = getline(&buffer, &bufsize, stdin);
		if (nread == -1)
		{
			_printf("\n");
			break;
		}
		if (buffer[nread - 1] == '\n')
			buffer[nread - 1] = '\0';
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (execl(buffer, buffer, NULL) == -1)
			{
				perror("execl");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if (waitpid(pid, NULL, 0) == -1)
			{
				perror("waitpid");
				exit(EXIT_FAILURE);
			}
		}
	}
	free(buffer);
	exit(EXIT_SUCCESS);
}

/**
 * _strstr - Locates a substring within a string
 * @haystack: The string to search in
 * @needle: The substring to search for
 *
 * Return: Pointer to the first occurrence of the substring, or NULL if not found
 */
char *_strstr(const char *haystack, const char *needle)
{
	if (*needle == '\0')
		return (char *)haystack;

	while (*haystack != '\0')
	{
		const char *h = haystack;
		const char *n = needle;

		while (*h == *n && *n != '\0')
		{
			h++;
			n++;
		}

		if (*n == '\0')
			return (char *)haystack;

		haystack++;
	}

	return (NULL);
}

/**
 * _strncpy - Copies a string up to n characters
 * @dest: The destination string
 * @src: The source string
 * @n: The maximum number of characters to copy
 *
 * Return: Pointer to the destination string
 */
char *_strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];

	for (; i < n; i++)
		dest[i] = '\0';

	return dest;
}

/**
 * _strtok - Extracts tokens from a string
 * @str: The string to tokenize
 * @delim: The delimiter characters
 *
 * Return: Pointer to the next token
 * or NULL if no more tokens are found
 */

char *_strtok(char *str, const char *delim)
{
	static char *next_token = NULL;
	bool delimiter_found = false;
	char *token_start;

	if (str != NULL)
	{
		next_token = str;
	}

	while (*next_token != '\0' && strchr(delim, *next_token) != NULL)
	{
		next_token++;
	}

	if (*next_token == '\0')
	{
		return (NULL);
	}

	token_start = next_token;

	while (*next_token != '\0' && strchr(delim, *next_token) == NULL)
	{
		next_token++;
	}

	if (*next_token != '\0')
	{
		*next_token++ = '\0';
		delimiter_found = true;
	}

	if (delimiter_found)
	{
		while (*next_token != '\0' && strchr(delim, *next_token) != NULL)
		{
			next_token++;
		}
	}

	return (token_start);
}
