#include "shell.h"

/**
 * _printf - Custom implementation of printf function
 *
 * @format: Format string
 *
 * Return: Number of characters printed
 */
int _printf(const char *format, ...)
{
	va_list args;
	int count = 0;
	int written;
	char c, *s;

	va_start(args, format);

	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format == '%')
			{
				_putchar('%');
				count++;
			}
			else if (*format == 'c')
			{
				c = (char)va_arg(args, int);
				_putchar(c);
				count++;
			}
			else if (*format == 's')
			{
				s = va_arg(args, char *);
				if (s != NULL)
				{
					written = write(STDOUT_FILENO, s, _strlen(s));
					if (written == -1)
					{
						_perror("Error: ");
						count = -1;
						break;
					}
					count += written;
				}
			}
			else
			{
				_putchar('%');
				_putchar(*format);
				count += 2;
			}
		}
		else
		{
			_putchar(*format);
			count++;
		}
		format++;
	}

	va_end(args);

	return (count);
}

/**
 * _putchar - Custom implementation of putchar function
 *
 * @c: Character to print
 *
 * Return: On success, returns the character printed
 *         On error, returns EOF
 */
int _putchar(char c)
{
	return (write(STDOUT_FILENO, &c, 1));
}

/**
 * _perror - Custom implementation of perror function
 *
 * @message: Error message
 */
void _perror(const char *message)
{
	if (errno != 0)
		perror(message);
	else
		_printf("%s\n", message);
}

/**
 * _strspn - Gets the length of a prefix substring
 *
 * @s: Pointer to the string to search
 * @accept: Pointer to the string of characters to match against
 *
 * Return: The number of bytes in the initial segment of s
 * which consist only of bytes from accept
 */

unsigned int _strspn(char *s, char *accept)
{
	unsigned int count = 0;
	int match;

	while (*s)
	{
		for (match = 0; accept[match]; match++)
		{
			if (accept[match] == *s)
			{
				count++;
				break;
			}
			else if ((accept[match + 1]) == '\0')
				return (count);
		}
		s++;
	}
	return (count);
}

/**
 * _strcspn - Computes the segment of str1 consisting of characters not in str2
 * @str1: The string to be searched
 * @str2: The string to be used
 * Return: The index at which a character in str1 exists in str2
 */

int _strcspn(char *str1, char *str2)
{
	int len = 0;
	int exists = 0; /* Flag to track if a character in str1 exists in str2 */
	int j;

	while (str1[len] != '\0')
	{
		exists = 0; /* Reset the flag for each character in str1 */

		for (j = 0; str2[j] != '\0'; j++)
		{
			if (str1[len] == str2[j])
			{
				exists = 1; /* Set the flag if a match is found */
				break;
			}
		}

		if (exists)
			break;

		len++;
	}

	return (len);
}
