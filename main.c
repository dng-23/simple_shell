#include "main.h"
#define ARGS 10
#define MAX_SIZE 200

/**
 * main - main function
 * Return: Always zero
 */

int main()
{
    char* line = NULL;
    size_t line_size = 0;
    ssize_t chars_read;

    while (1) {
        display_prompt();

        chars_read = custom_getline(&line, &line_size, stdin);

        if (chars_read == -1) {
            break;
        }

        if (chars_read > 0) {
            int arg_count = split_command(line);
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
                    int exit_status = execute_command_with_operators(0, arg_count - 1);
                }
            }
        }
    }

    free(line);

    for (int i = 0; i < alias_count; i++) {
        free(aliases[i]);
    }

    return 0;
}