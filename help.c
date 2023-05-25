#include "shell.h"

/**
 * print_string - Print a string to a stream
 * @str: String to be printed
 * @stream: Stream
 *
 * Return: void
 */
void print_string(char *str, int stream)
{
	int i = 0;

	for (; str[i] != '\0'; i++)
		write(stream, &str[i], 1);
}

/**
 * custom_tokenizer - Tokenize input string and store in an array
 * @input: Input string
 * @delimiter: Delimiter to be used
 *
 * Return: Array of tokens
 */
char **custom_tokenizer(char *input, char *delimiter)
{
	int num_tokens = 0;
	char **tokens = NULL;
	char *token = NULL;
	char *save_ptr = NULL;

	token = custom_strtok_r(input, delimiter, &save_ptr);

	while (token != NULL)
	{
	tokens = custom_realloc(tokens, sizeof(*tokens) * num_tokens,
			sizeof(*tokens) * (num_tokens + 1));
		tokens[num_tokens] = token;
		token = custom_strtok_r(NULL, delimiter, &save_ptr);
		num_tokens++;
	}

	tokens = custom_realloc(tokens, sizeof(*tokens) * num_tokens,
			sizeof(*tokens) * (num_tokens + 1));
	tokens[num_tokens] = NULL;

	return (tokens);
}

/**
 * custom_remove_newline - Removes newline character from a string
 * @string: String to be processed
 *
 * Return: void
 */
void custom_remove_newline(char *string)
{
	int index = 0;

	while (string[index] != '\0')
	{
		if (string[index] == '\n')
			break;
		index++;
	}
	string[index] = '\0';
}

/**
 * _strcpy - Copies a string
 * @src: Source string
 * @dest: Destination string
 *
 * Return: void
 */
void _strcpy(char *src, char *dest)
{
	int index = 0;

	while (src[index] != '\0')
	{
		dest[index] = src[index];
		index++;
	}
	dest[index] = '\0';
}

/**
 * _strlen - Calculates the length of a string
 * @str: String to be counted
 *
 * Return: Length of the string
 */
int _strlen(char *str)
{
	int length = 0;

	if (str == NULL)
		return (length);

	while (str[length] != '\0')
		length++;

	return (length);
}
