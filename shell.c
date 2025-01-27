#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>

// Function to skip whitespace
char* skip_initial_whitespace(char *str){
    while (*str && isspace(*str)){
        str++;
    }
    return str;
}

// Function to parse a quoted argument
char* parse_double_quoted_argument(char *temp, char **arg){
    temp++; 
    *arg=temp;
    while(*temp && *temp!='"'){
        temp++;
    }
    if(*temp=='\0'){
        fprintf(stderr, "Error: Mismatched quotes.\n Try again with a properly quoted argument.\n");
        arg[0] = NULL;
        return NULL;
    }
    *temp='\0'; 
    return temp+1; 
}

// Function to parse a normal argument
char* parse_normal_argument(char *temp, char **arg){
    *arg=temp;
    while(*temp && !isspace(*temp) && *temp!='\t'){
        temp++;
    }
    return temp;
}

// Main function to split input into arguments
void input_to_args(char *str, char **args){
    int i=0;
    char *temp=skip_initial_whitespace(str);
    while(*temp!='\0'){
        if (*temp=='"'){
            temp=parse_double_quoted_argument(temp, &args[i]);
            if(temp==NULL){
                args[0]=NULL; 
                return;
            }
        }else{
            temp=parse_normal_argument(temp, &args[i]);
        }
        // Null-terminate the current argument and move to the next
        if(*temp!='\0'){
            *temp='\0';
            temp=skip_initial_whitespace(temp+1);
        }
        i++;
    }
    args[i]=NULL; 
}

void input_redirection(char **args){
    for(int i=0; args[i]!=NULL; i++){
        if(strcmp(args[i], "<")==0){
            int fd=open(args[i+1], O_RDONLY);
            if (fd<0){
                perror("Failed to open file for reading");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
            break;
        }
    }
}
// Function to handle output redirection
void output_redirection(char **args){
    for(int i=0; args[i]!=NULL; i++){
        if(strcmp(args[i], ">")==0){
            int fd=open(args[i+1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
            if(fd<0){
                perror("Failed to open file for writing");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i]=NULL;
            break;
        }
    }
}
// Function to execute the command using execvp
void execute_commands_execvp(char **args){
    if(execvp(args[0], args)==-1){
        perror("Execution failed");
        exit(EXIT_FAILURE);
    }
}
// Function to execute the command with I/O redirection
void execute_command(char **args){
    pid_t pid=fork();
    if (pid<0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    if(pid==0){
        // Child process: handle I/O redirection, Execute the commands using execvp
        input_redirection(args);
        output_redirection(args);
        execute_commands_execvp(args);
    }else{
        // Parent process: wait for the child to finish
        waitpid(pid, NULL, 0);
    }
}

void handle_cd_builtin(char **args){
    if(args[1]==NULL){
        char *home_dir=getenv("HOME");
        if(home_dir==NULL||chdir(home_dir)!=0){
            perror("cd failed");
        }
    }
    else{
        if(chdir(args[1])!=0){
            perror("cd failed");
        }
    }
}

void display_help() {
    printf("\033[1;36m"); 
    printf("\n==================================================\n");
    printf("             Custom Shell - Help Menu        \n");
    printf("==================================================\n");
    printf("\033[0m");

    printf("\n\033[1;33mSUPPORTED COMMANDS:\033[0m\n");
    printf("  \033[1;32mhelp\033[0m        : Display this help menu.\n");
    printf("  \033[1;32mcd <dir>\033[0m    : Change the current directory to <dir>.\n");
    printf("                  Example: cd /home/user\n");
    printf("  \033[1;32mexit\033[0m        : Exit the shell.\n");
    printf("  \033[1;32m<command>\033[0m   : Execute external commands (e.g., ls, ./fact).\n");
    printf("                  Example: ls -l\n");

    printf("\n\033[1;33mI/O REDIRECTION:\033[0m\n");
    printf("  \033[1;32m>\033[0m           : Redirect output to a file.\n");
    printf("                  Example: ls -l > output.txt\n");
    printf("  \033[1;32m<\033[0m           : Redirect input from a file.\n");
    printf("                  Example: wc -l < input.txt\n");

    printf("\n\033[1;36m==================================================\n");
    printf("                End of Help Menu           \n");
    printf("==================================================\n");
    printf("\033[0m"); 
}


void print_shell_prompt(char *cwd){
    if(getcwd(cwd, 1024) == NULL){
        perror("getcwd failed");
        exit(1);
    }
    printf("%s> ", cwd);
}

void read_input(char *input, size_t size){
    // Read user input and remove trailing newline
    if(fgets(input, size, stdin) == NULL){
        printf("\n");
        input[0] = '\0';
        return;
    }
    input[strcspn(input, "\n")] = '\0';
}

void process_command(char *input, char *args[]){
    // Parse the input into arguments
    input_to_args(input, args);

    if (args[0] == NULL) return;
    
    // Handle built-in commands
    if (strcmp(args[0], "exit") == 0) exit(0);
    else if (strcmp(args[0], "cd") == 0) handle_cd_builtin(args);
    else if (strcmp(args[0], "help") == 0) display_help();
    else{
        execute_command(args);
    }
}


int main() {
    char input[100];
    char *args[64];
    char path[1024];

    printf("\n\033[1;36mCustom Shell Created by Aman Yadav.\033[0m\n"); // Cyan color for the first line
    printf("\033[1;33mType 'help' for a list of commands.\033[0m\n");    // Yellow color for the second line
    printf("\033[1;31mType 'exit' to quit.\033[0m\n");  

    while(1){
        print_shell_prompt(path);
        read_input(input, sizeof(input));
        process_command(input, args);
    }

    return 0;
}