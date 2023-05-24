#ifndef MAIN
#define MAIN

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

char *_strcat(char *dest, const char *src);
char *_strcpy(char *dest, char *src);
int _strcmp(char *s1, char *s2);
int _atoi(char *str);
int check_command_in_path(char *command, char **arr);
void split_string(char *buffer, char **argv, char *delim);
ssize_t _getline(char **lineptr, size_t *n, int fd);
char *my_strtok(char *str, const char *delim);

void shell_env(char **, char **);
void shell_setenv(char **, char **);
void shell_unsetenv(char **, char **);
void exit_shell(int);
void change_directory(char **argv, char **envp);
/**
 * struct builtin_cmd - struct to check command given
 * @name: name of the command
 * @func_str: function
 * @func_int: function
 * Descriptor: as mentioned above
 */
typedef struct builtin_cmd
{
	char *name;
	void (*func_str)(char **, char **);
	void (*func_int)(int);
} builtin_cmd;
void exec_builtin(struct builtin_cmd *, char **argv, char **envp);
void start_shell(char **cmd, char **envp, struct builtin_cmd *, int check);
#endif
