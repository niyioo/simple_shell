#include "shell.h"

/**
 * _strcmp - Compare two strings
 * @str1: First string to be compared
 * @str2: Second string to be compared
 *
 * Return: Difference between the two strings
 */
int _strcmp(char *str1, char *str2)
{
	int i = 0;

	while (str1[i] != '\0')
	{
		if (str1[i] != str2[i])
			break;
		i++;
	}

	return (str1[i] - str2[i]);
}

/**
 * _strcat - Concatenates two strings
 * @dest: String to be concatenated to
 * @src: String to concatenate
 *
 * Return: Address of the new string
 */
char *_strcat(char *dest, char *src)
{
	char *new_string = NULL;
	int dest_len = _strlen(dest);
	int src_len = _strlen(src);
	int i, j;

	new_string = malloc(sizeof(*new_string) * (dest_len + src_len + 1));
	if (new_string == NULL)
		return (NULL);

	for (i = 0; dest[i] != '\0'; i++)
		new_string[i] = dest[i];

	for (j = 0; src[j] != '\0'; j++)
		new_string[i + j] = src[j];

	new_string[i + j] = '\0';

	return (new_string);
}

/**
 * _strspn - Get length of a prefix substring
 * @str: String to be searched
 * @accept: String to be used
 *
 * Return: Number of bytes
 */
int _strspn(char *str, char *accept)
{
	int i = 0;
	int match = 0;

	while (str[i] != '\0')
	{
		if (_strchr(accept, str[i]) == NULL)
			break;
		match++;
		i++;
	}
	return (match);
}

/**
 * _strcspn - get length of a prefix substring
 * @str1: String to be searched
 * @str2: String to be used
 *
 * Return: Index at which a character in @str1 exists in @str2
 */
int _strcspn(char *str1, char *str2)
{
	int len = 0;
	int i;

	for (i = 0; str1[i] != '\0'; i++)
	{
		if (_strchr(str2, str1[i]) != NULL)
			break;
		len++;
	}
	return (len);
}

/**
 * _strchr - Locate character in string
 * @str: String to be searched
 * @ch: Character to be checked
 *
 * Return: Pointer to the first occurrence of @ch in @str, or NULL if not found
 */
char *_strchr(char *str, char ch)
{
	int i = 0;

	for (; str[i] != ch && str[i] != '\0'; i++)
		;

	if (str[i] == ch)
		return (str + i);
	else
		return (NULL);
}
