#include "shell.h"

/**
 * custom_strtok_r - Tokenizes a string
 * @str: String to be tokenized
 * @delim: Delimiter to be used to tokenize the string
 * @save_ptr: Pointer to be used to keep track of the next token
 *
 * Return: The next available token
 */
char *custom_strtok_r(char *str, char *delim, char **save_ptr)
{
	char *finish;

	if (str == NULL)
		str = *save_ptr;

	if (*str == '\0')
	{
		*save_ptr = str;
		return (NULL);
	}

	str += _strspn(str, delim);
	if (*str == '\0')
	{
		*save_ptr = str;
		return (NULL);
	}

	finish = str + _strcspn(str, delim);
	if (*finish == '\0')
	{
		*save_ptr = finish;
		return (str);
	}

	*finish = '\0';
	*save_ptr = finish + 1;
	return (str);
}

/**
 * custom_realloc - Reallocates a memory block
 * @ptr: Pointer to the memory previously allocated with a call to malloc
 * @old_size: Size of ptr
 * @new_size: Size of the new memory to be allocated
 *
 * Return: Pointer to the address of the new memory block
 */
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *temp_block;
	char *src, *dest;
	unsigned int i;

	if (new_size == old_size)
		return (ptr);

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	temp_block = malloc(new_size);
	if (temp_block != NULL)
	{
		src = (char *)ptr;
		dest = (char *)temp_block;

		for (i = 0; i < old_size && i < new_size; i++)
			dest[i] = src[i];

		free(ptr);
	}

	return (temp_block);
}

/**
 * _atoi - Converts a string to an integer
 * @s: The string to be converted
 *
 * Return: The converted integer
 */
int _atoi(char *s)
{
	int result = 0;
	int sign = 1;
	int i = 0;
	int digit;

	if (s[i] == '-')
	{
		sign = -1;
		i++;
	}

	while (s[i] != '\0')
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			digit = s[i] - '0';
			if (result > INT_MAX / 10 ||
					(result == INT_MAX / 10 && digit > INT_MAX % 10))
			{
				if (sign == -1)
					return (INT_MIN);
				else
					return (INT_MAX);
			}

			result = (result * 10 + digit);
		}
		else
		{
			break;
		}

		i++;
	}

	return (result * sign);
}

/**
 * handle_ctrl_c - Handles the signal raised by CTRL-C
 * @signum: Signal number
 * Return: void
 */

void handle_ctrl_c(int signum)
{
	if (signum == SIGINT)
		print_string("\n($) ", STDIN_FILENO);
}

/**
 * ignore_comment - ignores everything after a '#' character
 * @input: Input string to be processed
 *
 * Return: void
 */
void ignore_comment(char *input)
{
	int i = 0;

	if (input[i] == '#')
		input[i] = '\0';

	while (input[i] != '\0')
	{
		if (input[i] == '#' && input[i - 1] == ' ')
			break;
		i++;
	}

	input[i] = '\0';
}
