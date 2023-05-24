#include "main.h"
#define ARGS 10
#define MAX_SIZE 200

/**
 * main - main function
 * @argc: number of arguments passed
 * @av: array of arguments passed
 * @envp: array of environment variables
 * Return: Always zero
 */

int main(int __attribute__((__unused__)) argc, char __attribute__((__unused__))
		**av, char **envp)
{
	char *buffer = NULL, *cmd[ARGS];
	size_t bufsize = 0;
	int check = 0;
	ssize_t line_size;
	builtin_cmd builtins[] = {
		{"cd", change_directory, NULL},
		{"env", shell_env, NULL},
		{"exit", NULL, exit_shell},
		{"setenv", shell_setenv, NULL},
		{"unsetenv", shell_unsetenv, NULL}
	};

	while (1)
	{
		fflush(stdout);
		line_size = _getline(&buffer, &bufsize, STDIN_FILENO);
		if (line_size == -1)
			break;
		check = 0;
		split_string(buffer, cmd, ";");
		start_shell(cmd, envp, builtins, check);
	}
	free(buffer);
	return (0);
}

/**
 * start_shell - start shell process
 * @cmd: command to execute
 * @envp: environment variable
 * @builtins: array of values
 * @check: flag
 * Return - Nothing
 */
void start_shell(char **cmd, char **envp, struct builtin_cmd *builtins,
		int check)
{
	char *argv[ARGS], *arr_path;
	pid_t child_pid;
	int status, res, i, j;

	for (i = 0; cmd[i] != NULL; i++)
	{
		split_string(cmd[i], argv, " ");
		for (j = 0; j < 5; j++)
		{
			if (_strcmp(argv[0], builtins[j].name) == 0)
			{
				exec_builtin(builtins, argv, envp);
				check = 1;
				res = 2;
				break;
			}
		}
		if (!check)
			res = check_command_in_path(argv[0], &arr_path);
		if (res == -1)
			printf("%s: command not found\n", argv[0]);
		else if (res == 1)
		{
			child_pid = fork();
			if (child_pid == -1)
				perror("Error:");
			else if (child_pid == 0)
			{
				if (execve(arr_path, argv, envp) == -1)
				{
					perror("./shell");
					break;
				}
			}
			else
				wait(&status);
		}
	}
}
/**
 * check_command_in_path - check if command is in path
 * @command: string to check
 * @arr_path: pointer to path of the command
 * Return: 1 if command is found else -1
 */

int check_command_in_path(char *command, char **arr_path)
{
	char *token, filepath[30], *path, cp[1024], *path_cpy;
	struct stat st;

	path = getenv("PATH");
	path_cpy = _strcpy(cp, path);
	if (stat(command, &st) == 0)
	{
		*arr_path = command;
		return (1);
	}
	token = strtok(path_cpy, ":");
	while (token != NULL)
	{
		_strcpy(filepath, token);
		_strcat(filepath, "/");
		_strcat(filepath, command);

		if (stat(filepath, &st) == 0)
		{
			if (S_ISREG(st.st_mode) || S_ISDIR(st.st_mode))
			{
				if (st.st_mode & S_IXUSR)
				{
					*arr_path = _strcpy(malloc(strlen(filepath) + 1), filepath);
					return (1);
				}
			}
		}
		token = strtok(NULL, ":");
	}
	return (-1);
}
/**
 * split_string - split a sting based on given delim
 * @buffer: string to split
 * @argv: store the splitted tokens
 * @delim: delimeter
 * Return: Nothing
 */

void split_string(char *buffer, char **argv, char *delim)
{
	char *token;
	int i = 0;

	token = strtok(buffer, delim);
	while (token != NULL)
	{
		argv[i++] = token;
		token = strtok(NULL, delim);
	}
	argv[i - 1] = strtok(argv[i - 1], "\n");
	argv[i] = NULL;
}

/**
 *_getline - custome getline
 * @lineptr: string recieved
 * @n: size of string to recieve
 * @fd: file descriptor
 * Return: number of read size or -1
 */
ssize_t _getline(char **lineptr, size_t *n, int fd)
{
	ssize_t read_size, i = 0;
	static char c;
	*n = MAX_SIZE;

	if (lineptr == NULL)
		return (-1);
	if (*lineptr == NULL || *n == 0)
	{
		*lineptr = malloc(sizeof(char) * MAX_SIZE);
		if (!*lineptr)
			return (-1);
		*n = MAX_SIZE;
	}
	while (1)
	{
		read_size = read(fd, &c, 1);
		if (read_size == -1)
			perror("Error ");

		if (read_size == 0)
		{
			if (i > 0)
			{
				(*lineptr)[i] = '\0';
				return (-1);
			}
			else
				return (-1);
		}
		(*lineptr)[i] = c;
		i++;
		if (c == '\n')
		{
			(*lineptr)[i] = '\0';
			return (i);
		}
	}
}
