#include "main.h"

/**
 * _strcat - function that concatenates two strings
 * @dest: a string src to be added to
 * @src: a string to be appended
 * Return: a pointer to the resulting string dest
 */
char *_strcat(char *dest, const char *src)
{
	char *final_str = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++))
		;
	return (final_str);
}

/**
 * _strcpy -  copies the string pointed to by src to dest
 * @dest: string to be copied to
 * @src: string copied from
 * Return: pointer to dest
 */
char *_strcpy(char *dest, char *src)
{
	char *copied = dest;

	while ((*dest++ = *src++))
		;
	return (copied);
}

/**
 * _strcmp - function that copies a string
 * @s1: a string to be compared
 * @s2: a string to be compared
 * Return: lessthan greaterthan or equal to zero
 */
int _strcmp(char *s1, char *s2)
{
	int i = 0, val;

	while ((s1[i] == s2[i]) && s1[i] != '\0')
	{
		i++;
	}
	val = s1[i] - s2[i];
	return (val);
}

/**
 * _atoi - function that converts string to integer
 * @str: a string to be converted
 * Return: the converte value or 0 if not converted
 */
int _atoi(char *str)
{
	int result = 0;
	int sign = 1;
	int i = 0;

	if (str[0] == '-')
	{
		sign = -1;
		i++;
	}

	for (; str[i] != '\0'; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
			result = result * 10 + (str[i] - '0');
		else
			return (0);
	}

	return (result * sign);
}

/**
 * exec_builtin - execute custome commands
 * @builtins: array of built_cmd
 * @argv: command from user
 * @envp: environment variable
 * Return: Nothing
 */

void exec_builtin(builtin_cmd *builtins, char **argv, char **envp)
{
	int i = 0, status;

	while (builtins[i].name != NULL)
	{
		if (_strcmp(argv[0], builtins[i].name) == 0)
		{
			if (builtins[i].func_str != NULL)
				builtins[i].func_str(argv, envp);
			else if (builtins[i].func_int != NULL)
			{
				if (argv[1] != NULL && argv[2] != NULL)
				{
					printf("Usage: exit [status]\n");
					return;
				}
				else if (argv[1])
					status = _atoi(argv[1]);
				else
					status = 0;
				builtins[i].func_int(status);
			}
			return;
		}
		i++;
	}
}
