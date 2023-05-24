#include "main.h"

/**
 * shell_env - prints environment ariables
 * @argv: not used here
 * @env: environment to be printed
 * Return: Nothing
 */
void shell_env(char __attribute__((__unused__)) **argv, char **env)
{
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
}

/**
 * exit_shell - exits process
 * @status: status of exit
 * Return: Nothing
 */
void exit_shell(int status)
{
	exit(status);
}

/**
 * change_directory - changes current directory
 * @argv: command from user
 * @envp: not used here
 * Return: Nothing
 */

void change_directory(char **argv, char __attribute__((__unused__)) **envp)
{
	char *new_dir = argv[1];

	if (new_dir == NULL || _strcmp(new_dir, "~") == 0)
		new_dir = getenv("HOME");
	else if (_strcmp(new_dir, "-") == 0)
		new_dir = getenv("OLDPWD");
	if (chdir(new_dir) == -1)
	{
		perror("cd");
	}
	else
	{
		setenv("OLDPWD", getenv("PWD"), 1);
		setenv("PWD", new_dir, 1);
	}
}

/**
 * shell_setenv - Initialize a new env variable, or modify an existing one
 * @argv: command from user
 * @envp: not used here
 * Return: Nothing
 */

void shell_setenv(char **argv, char __attribute__((__unused__)) **envp)
{
	char *msg = "Usage: setenv VARIABLE\n";
	char *msg2 = "FAILED: to set variable\n";
	size_t count = 30;

	if (argv[1] == NULL || argv[2] == NULL || argv[3] != NULL)
	{
		write(STDERR_FILENO, msg, count);
		return;
	}
	if (setenv(argv[1], argv[2], 1) != 0)
		write(STDERR_FILENO, msg2, count);
}

/**
 * shell_unsetenv - Remove an environment variable
 * @argv: command from user
 * @envp: not used here
 * Return: Nothing
 */

void shell_unsetenv(char **argv, char __attribute__((__unused__)) **envp)
{
	char *msg = "Usage: unsetenv VARIABLE\n";
	char *msg2 = "FAILED: to unset variable\n";
	size_t count = 30;

	if (argv[1] == NULL || argv[2] != NULL)
	{
		write(STDERR_FILENO, msg, count);
		return;
	}
	if (unsetenv(argv[1]) != 0)
		write(STDERR_FILENO, msg2, count);
}
