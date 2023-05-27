#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define MAX_ALIASES 100
#define MAX_SIZE 200

char input_buffer[BUFFER_SIZE];
char* args_buffer[BUFFER_SIZE];
char* aliases[MAX_ALIASES];
int alias_count = 0;

int split_command(char* command) {
    int i = 0;
    char* token = strtok(command, " ");

    while (token != NULL) {
        args_buffer[i] = token;
        i++;
        token = strtok(NULL, " ");
    }

    args_buffer[i] = NULL;

    return i;
}

void display_prompt() {
    printf("$ ");
    fflush(stdout);
}

int execute_command(char* command) {
	int status;
	int exit_status;
	pid_t pid;

    command[strcspn(command, "\n")] = '\0';

    pid = fork();

    if (pid == -1) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        
        execvp(command, args_buffer);

      
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
      
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            exit_status = WEXITSTATUS(status);
            return exit_status;
        }
    }

    return -1;
}

void handle_exit_command()
{
	int exit_status;

    if (args_buffer[1] != NULL) {
        exit_status = atoi(args_buffer[1]);
        exit(exit_status);
    } else {
        exit(0);
    }
}

void handle_env_command() {
    extern char** environ;
	char** env;

    for (env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
}

void handle_setenv_command() {
    if (args_buffer[1] != NULL && args_buffer[2] != NULL) {
        if (setenv(args_buffer[1], args_buffer[2], 1) == -1) {
            perror("setenv");
        }
    } else {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
    }
}

void handle_unsetenv_command() {
    if (args_buffer[1] != NULL) {
        if (unsetenv(args_buffer[1]) == -1) {
            perror("unsetenv");
        }
    } else {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
    }
}

void handle_cd_command() {
    char* directory = args_buffer[1];
	char *oldpwd;
	char cwd[BUFFER_SIZE];

    if (directory == NULL || strcmp(directory, "~") == 0) {
        directory = getenv("HOME");
    } else if (strcmp(directory, "-") == 0) {
        directory = getenv("OLDPWD");
    }

    if (directory != NULL) {
        oldpwd = getenv("PWD");
        if (oldpwd != NULL) {
            setenv("OLDPWD", oldpwd, 1);
        }

        if (chdir(directory) == -1) {
            perror("chdir");
        } else {
            
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                setenv("PWD", cwd, 1);
            } else {
                perror("getcwd");
            }
        }
    }
}

ssize_t custom_getline(char** lineptr, size_t* n, FILE* stream) {
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
		read_size = read(fileno(stream), &c, 1);
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

int execute_command_with_operators(int start, int end) {
    int exit_status = 0;
    int command_start = start, i;

    for (i = start; i <= end; i++) {
        if (strcmp(args_buffer[i], ";") == 0) {
            args_buffer[i] = NULL;
            exit_status = execute_command(args_buffer[command_start]);
            command_start = i + 1;
        } else if (strcmp(args_buffer[i], "&&") == 0) {
            args_buffer[i] = NULL;
            exit_status = execute_command(args_buffer[command_start]);

            if (exit_status != 0) {
                break;
            }

            command_start = i + 1;
        } else if (strcmp(args_buffer[i], "||") == 0) {
            args_buffer[i] = NULL;
            exit_status = execute_command(args_buffer[command_start]);

            if (exit_status == 0) {
                break;
            }

            command_start = i + 1;
        }
    }

    if (command_start <= end)
	{
        exit_status = execute_command(args_buffer[command_start]);
    }

    return (exit_status);
}

void handle_alias_command()
{
	int i, j, alias_index;

    if (args_buffer[1] == NULL) {
   
        for (i = 0; i < alias_count; i++) {
            printf("%s\n", aliases[i]);
        }
    } else {
        i = 1;
        while (args_buffer[i] != NULL) {
            if (strchr(args_buffer[i], '=') != NULL) {
                
                char* alias = strdup(args_buffer[i]);
                if (alias == NULL) {
                    perror("strdup");
                    return;
                }

      
                alias_index = -1;
                for (j = 0; j < alias_count; j++) {
                    if (strncmp(alias, aliases[j], strlen(args_buffer[i])) == 0) {
                        alias_index = j;
                        break;
                    }
                }

                if (alias_index >= 0) {
                   
                    free(aliases[alias_index]);
                    aliases[alias_index] = alias;
                } else {
                    if (alias_count < MAX_ALIASES) {
                        aliases[alias_count] = alias;
                        alias_count++;
                    } else {
                        fprintf(stderr, "Maximum number of aliases reached\n");
                        free(alias);
                    }
                }
            } else {
                alias_index = -1;
                for (j = 0; j < alias_count; j++) {
                    if (strcmp(args_buffer[i], aliases[j]) == 0) {
                        alias_index = j;
                        break;
                    }
                }

                if (alias_index >= 0) {
                    printf("%s\n", aliases[alias_index]);
                }
            }

            i++;
        }
    }
}

int main() {
    char* line = NULL;
    size_t line_size = 0;
    ssize_t chars_read;
	int arg_count, i;

    while (1) {
        display_prompt();

        chars_read = custom_getline(&line, &line_size, stdin);

        if (chars_read == -1) {
            break;
        }

        if (chars_read > 0) {
            arg_count = split_command(line);
            if (arg_count > 0) {
                if (strcmp(args_buffer[0], "exit") == 0) {
                    handle_exit_command();
                } else if (strcmp(args_buffer[0], "env") == 0) {
                    handle_env_command();
                } else if (strcmp(args_buffer[0], "setenv") == 0) {
                    handle_setenv_command();
                } else if (strcmp(args_buffer[0], "unsetenv") == 0) {
                    handle_unsetenv_command();
                } else if (strcmp(args_buffer[0], "cd") == 0) {
                    handle_cd_command();
                } else if (strcmp(args_buffer[0], "alias") == 0) {
                    handle_alias_command();
                } else {
                    execute_command_with_operators(0, arg_count - 1);
                }
            }
        }
    }

    free(line);

    for (i = 0; i < alias_count; i++) {
        free(aliases[i]);
    }

    return 0;
}
