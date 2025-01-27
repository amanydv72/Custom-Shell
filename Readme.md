# Custom Command-Line Shell

This project is a custom command-line shell implemented in C. The shell supports executing external commands, handling built-in commands (like `cd`, `help`, and `exit`), and supports basic I/O redirection. It is designed to mimic the functionality of a typical Unix shell in a simplified way.

## Features

- **Command Execution**: Executes standard commands like `ls`, `cat`, `echo`, etc.
- **Built-in Commands**:
  - `cd`: Change the current working directory.
  - `help`: Display help information.
  - `exit`: Exit the shell.
- **Prompt with Current Directory**: Displays the current working directory in the prompt.
- **Input Parsing**: Properly parses commands and arguments.
- **I/O Redirection**: Handles `>` for output redirection and `<` for input redirection.

## How to Compile

To compile the program, you can use the provided Makefile. Simply run the following command:

```bash
make
```

This will compile the `fact` program and the shell program. You can also run individual compilation commands with `make fact` or `make shell`.

## How to Run

Run the compiled shell using the following command:

```bash
./shell
```

You will see a prompt showing the current working directory. From there, you can start typing commands.

##

You will see a prompt showing the current working directory. From there, you can start typing commands.

## Usage Examples

### 1. Executing External Commands

```bash
/home/user> ls -l
/home/user> echo "Hello, World!"
```

### 2. Changing Directory

```bash
/home/user> cd Documents
/home/user/Documents>
```

### 3. Displaying Help

```bash
/home/user> help
Help: This shell supports the following commands:
- exit: Exit the shell
- cd <directory>: Change the working directory
- help: Display this help message
```

### 4. Exiting the Shell

```bash
/home/user> exit
```

### 5. I/O Redirection

#### Redirecting Output to a File
```bash
/home/user> echo "This is a test" > output.txt
```

#### Redirecting Input from a File
```bash
/home/user> cat < input.txt
```

## Project Structure

The shell is implemented in a single file, `shell.c`, with the following key functions:

- **`main()`**: Main function to initialize the shell loop.
- **`print_shell_prompt()`**: Prints the prompt with the current working directory.
- **`read_input()`**: Reads user input and splits it into tokens.
- **`process_command()`**: Parses the tokens and executes the corresponding command using the fork(), execvp(), and waitpid() system calls.


