#include "shell.h"

/**
 * _atoi - functions that convert a string to an integer
 * @s: Input string to convert
 *
 * Return: the converted integer
 */

int _atoi(char *s)
{
	int i = 0;
	int sign = 1;
	int num = 0;

	if (s[0] == '-')
	{
		sign = -1;
		i++;
	}

	while (s[i] != '\0')
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			if (num > INT_MAX / 10 || (num == INT_MAX / 10 && s[i] - '0' > 7))
			{
				if (sign == 1)
					return (INT_MAX);
				else
					return (INT_MIN);
			}
			num = num * 10 + (s[i] - '0');
		}
		else
		{
			break;
		}
		i++;
	}

	return (num * sign);
}

/**
 * _strlen - function that returns the length of a string
 * @s: Input
 *
 * Return: length
 */

int _strlen(const char *s)
{
	int length = 0;

	while (*s != '\0')
	{
		length++;
		s++;
	}
	return (length);
}

/**
 * *_strcpy - function that copies string pointed to by src
 * @dest: Input
 * @src: Input
 *
 * Return: dest
 */

char *_strcpy(char *dest, const char *src)
{
	char *dest_start = dest;

	while (*src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';

	return (dest_start);
}

/**
 * _strcmp - function that compares two strings
 * @s1: String 1
 * @s2: String 2
 *
 * Return: comparison
 */

int _strcmp(char *s1, char *s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			return (0);
		s1++;
		s2++;
	}

	return (*s1 - *s2);
}

/**
 * _strcat - concatenates two strings.
 * @dest: string to be concatenated to.
 * @src: string to be appended to dest.
 *
 * Return: pointer to resulting string dest.
 */

char *_strcat(char *dest, char *src)
{
	char *dest_end = dest;

	while (*dest_end != '\0')
		dest_end++;

	while (*src != '\0')
	{
		*dest_end = *src;
		dest_end++;
		src++;
	}

	*dest_end = '\0';

	return (dest);
}
