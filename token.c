#include "shell.h"

/**
 * tokenize_input - Tokenizes the input string
 * @input: Input string
 * @tokens: Array of tokens
 *
 * Return: Number of tokens
 */
int tokenize_input(char *input, char **tokens)
{
	char *token;
	int i = 0;

	token = strtok(input, " \n");
	while (token != NULL)
	{
		tokens[i] = token;
		i++;
		token = strtok(NULL, " \n");
	}
	tokens[i] = NULL;

	   return (i);
}
