#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

void parse_input(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " \t\n");

    while (args[i] != NULL && i < MAX_ARGS - 1) {
        i++;
        args[i] = strtok(NULL, " \t\n");
    }
}

void command_help() {
    printf("Available commands:\n");
    printf("  help        Show this help\n");
    printf("  pwd         Print current directory\n");
    printf("  cd <path>   Change directory\n");
    printf("  exit        Exit shell\n");
}

int execute_builtin(char **args) {
    if (args[0] == NULL) return 1;

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    if (strcmp(args[0], "help") == 0) {
        command_help();
        return 1;
    }

    if (strcmp(args[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
        else
            perror("getcwd");
        return 1;
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "cd: missing path\n");
        } else {
            if (chdir(args[1]) != 0)
                perror("cd");
        }
        return 1;
    }

    return 0;
}

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {
        printf("mini-shell> ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }

        parse_input(input, args);

        if (execute_builtin(args)) {
            continue;
        }

        printf("Unknown command: %s\n", args[0]);
    }

    return 0;
}   