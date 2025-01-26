#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define PATH_SIZE 1024

// Function to parse user input with handling for quoted arguments
void parse_input(char *input, char **args)
{
    int i = 0;
    char *token = input;

    while (*token != '\0')
    {
        // Skip leading spaces or tabs
        while (*token == ' ' || *token == '\t')
        {
            token++;
        }

        if (*token == '\0')
            break;

        // Handle quoted arguments
        if (*token == '"')
        {
            token++; // Skip the opening quote
            args[i] = token;

            // Find the closing quote
            while (*token != '"' && *token != '\0')
            {
                token++;
            }

            if (*token == '\0')
            {
                fprintf(stderr, "Error: Mismatched quotes.\n");
                args[0] = NULL; // Mark command as invalid
                return;
            }

            *token = '\0'; // Null-terminate the argument
            token++;       // Move past the closing quote
        }
        else
        {
            // Handle non-quoted arguments
            args[i] = token;
            while (*token != ' ' && *token != '\t' && *token != '\0')
            {
                token++;
            }
        }

        // Null-terminate the current argument and move to the next
        if (*token != '\0')
        {
            *token = '\0';
            token++;
        }
        i++;
    }

    args[i] = NULL; // Mark the end of the arguments
}

// Function to execute external commands with optional I/O redirection
void execute_command(char **args)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        // Child process: handle I/O redirection
        for (int i = 0; args[i] != NULL; i++)
        {
            if (strcmp(args[i], ">") == 0)
            {
                // Redirect output to file
                int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0)
                {
                    perror("Failed to open file for writing");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                args[i] = NULL; // Remove '>' and filename from args
                break;
            }
            else if (strcmp(args[i], "<") == 0)
            {
                // Redirect input from file
                int fd = open(args[i + 1], O_RDONLY);
                if (fd < 0)
                {
                    perror("Failed to open file for reading");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
                args[i] = NULL; // Remove '<' and filename from args
                break;
            }
        }

        // Execute the command
        if (execvp(args[0], args) == -1)
        {
            perror("Execution failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Parent process: wait for the child to finish
        waitpid(pid, NULL, 0);
    }
}

// Function to handle the "cd" built-in command
void handle_cd(char **args)
{
    if (args[1] == NULL)
    {
        const char *home = getenv("HOME");
        if (home == NULL || chdir(home) != 0)
        {
            perror("cd failed");
        }
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("cd failed");
        }
    }
}

// Function to display the help menu
void display_help()
{
    printf("Custom Shell - Help Menu\n");
    printf("The following commands are supported:\n");
    printf("  help        : Display this help menu.\n");
    printf("  cd <dir>    : Change the current directory to <dir>.\n");
    printf("  exit        : Exit the shell.\n");
    printf("  <command>   : Execute external commands (e.g., ls, ./fact).\n");
    printf("  I/O Redirection:\n");
    printf("     Use '>' to redirect output to a file.\n");
    printf("     Use '<' to redirect input from a file.\n");
    printf("\n");
}

int main()
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    char cwd[PATH_SIZE]; // Buffer for the current working directory

    printf("Custom Shell Created by Aman Yadav ❤️. \nType 'help' for a list of commands.\nType 'exit' to quit.\n");

    while (1)
    {
        // Get the current working directory
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd failed");
            exit(1);
        }

        // Print the shell prompt with the current directory
        printf("%s> ", cwd);

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        // Remove trailing newline from input
        input[strcspn(input, "\n")] = '\0';

        // Parse the input into arguments
        parse_input(input, args);

        // Skip empty commands
        if (args[0] == NULL)
        {
            continue;
        }

        // Handle built-in commands
        if (strcmp(args[0], "exit") == 0)
        {
            break;
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            handle_cd(args);
        }
        else if (strcmp(args[0], "help") == 0)
        {
            display_help();
        }
        else
        {
            // Execute other commands
            execute_command(args);
        }
    }

    printf("Shell exited. I appreciate you using my Custom Shell! \nThank You 😇😇\n");
    return 0;
}
