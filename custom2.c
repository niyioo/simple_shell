#include "shell.h"

/**
 * _strchr - Locates a character in a string
 *
 * @s: Pointer to the string to search
 * @c: The character to search for
 *
 * Return: A pointer to the first occurrence of the
 * character c in the string s,
 * or NULL if the character is not found
 */
char *_strchr(char *s, char c)
{
	while (*s != '\0' && *s != c)
		s++;

	if (*s == c)
		return (s);
	else
		return (NULL);
}

/**
 * _strtok_r - Tokenizes a string into smaller strings based on a delimiter
 * @str: The string to be tokenized
 * @delim: The delimiter used to split the string
 * @saveptr: Pointer to a char pointer that stores the context between calls
 * Return: A pointer to the next token found in the string
 * or NULL if no more tokens are found
 */
char *_strtok_r(char *str, const char *delim, char **saveptr)
{
	char *token;
	char *start;
	const char *d;

	if (str != NULL)
	{
		start = str;
	}
	else if (*saveptr != NULL)
	{
		start = *saveptr;
	}
	else
	{
		return (NULL);
	}

	token = start;

	if (*token == '\0')
	{
		*saveptr = NULL;
		return (NULL);
	}

	/* Find the next occurrence of the delimiter */
	while (*start != '\0')
	{
		d = delim;
		while (*d != '\0')
		{
			if (*start == *d)
			{
				/* Found the delimiter, terminate the token and update saveptr */
				*start = '\0';
				*saveptr = start + 1;
				return (token);
			}
			d++;
		}
		start++;
	}

	/* No more delimiters found, return the remaining token */
	*saveptr = NULL;
	return (token);
}

/**
 * _realloc - Reallocates a memory block with a new size
 * @ptr: Pointer to the previously allocated memory block
 * @size: New size of the memory block
 * Return: Pointer to the address of the new memory block, or NULL if realloc fails
 */

void *_realloc(void *ptr, size_t size)
{
	void *new_ptr;

	/* If ptr is NULL, treat it as a call to malloc */
	if (ptr == NULL)
	{
		new_ptr = malloc(size);
		if (new_ptr == NULL)
		{
			/* Failed to allocate memory */
			return (NULL);
		}
		return (new_ptr);
	}

	/* If size is 0, treat it as a call to free */
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}

	/* Reallocate memory block with the new size */
	new_ptr = realloc(ptr, size);
	if (new_ptr == NULL)
	{
		/* Failed to reallocate memory */
		return (NULL);
	}

	return (new_ptr);
}

/**
 * _strdup - Duplicates a string
 * @str: The string to duplicate
 *
 * Return: Pointer to the newly duplicated string or NULL if allocation fails
 */

char *_strdup(const char *str)
{
	size_t len;
	char *dup;

	if (str == NULL)
		return NULL;

	len = strlen(str);
	dup = malloc((len + 1) * sizeof(char));

	if (dup == NULL)
		return (NULL);

	return (strcpy(dup, str));
}

/**
 * _strncmp - Compare two strings up to a specified length
 * @s1: The first string to compare
 * @s2: The second string to compare
 * @n: The maximum number of characters to compare
 *
 * Return: 0 if the strings are equal, negative if s1 is less than s2,
 *         or positive if s1 is greater than s2
 */

int _strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++, s1++, s2++)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		else if (*s1 == '\0')
			return (0);
	}
	return (0);
}
