#include "main.h"

/**
 * _strncpy - function that copies a string
 * @dest: a string that copied to
 * @src: a string copied from
 * @n: bytes to be copied from src
 * Return: a pointer to the resulting string dest
 */
char *_strncpy(char *dest, char *src, int n)
{
	int i = 0;

	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}
