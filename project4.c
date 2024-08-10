// Connor Seimears
// CST315 - Operating Systems
// Project 4: File System Manager - Improved Shell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>

#define MAX_LENGTH 256

// Define list of commands available
const char *commands[] = {
    "ls",
    "pwd",
    "touch",
    "cd",
    "cd ..",
    "clear",
    "cp",
    "find",
    "mkdir",
    "mv",
    "rmdir",
    "rm",
    "stat",
    "nano",
    "echo",
    "history",
    "open",
    "exit",
    NULL
};

//
void exit_execution(char** argv)
{
    int status = atoi(argv[1]);
    exit(status);
}

void exit_shell(char** argv)
{
    printf("Exiting the shell...\n");
    exit(0);
}

/* Prints the command history of current session by using
 * history_get() from readline library to retrieve command history
 * starting from history_base index up to history_length.
 */
void print_history() {
    int i;
    for (i = history_base; i < history_length; i++) {
        HIST_ENTRY *entry = history_get(i);
        if (entry == NULL) {
            continue;
        }
        printf("%d %s\n", i, entry->line);
    }
}

// Generate the prompt string that appears in the terminal
void print_prompt() {
    
    // Initialize char arrays for cwd, hostname, username
    char cwd[MAX_LENGTH];
    char hostname[MAX_LENGTH];
    char username[MAX_LENGTH];
    
    // Conditions if cwd, hostname, username are unobtainable
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        fprintf(stderr, "Error: failed to get current working directory\n");
    }

    if (gethostname(hostname, sizeof(hostname)) == -1) {
        fprintf(stderr, "Error: failed to get hostname\n");
    }

    if (getlogin_r(username, sizeof(username)) == -1) {
        fprintf(stderr, "Error: failed to get login name\n");
    }
    
    // Otherwise, display the prompt string properly
    printf("%s@%s:%s$ ", username, hostname, cwd);
}

// Check whether a given command is a built-in command or not.
int is_builtin_command(char *command)
{
    int i;
    for (i = 0; commands[i] != NULL; i++)
    {
        if (strcmp(command, commands[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}
void execute_command(char** argv) {
    if (strcmp(argv[0], "open") == 0) {
        // open command
        if (argv[1] == NULL) {
            printf("Error: expected file name after 'open'\n");
        } else {
            char command[100];
            strcpy(command, "open ");
            strcat(command, argv[1]);
            system(command);
        }
    } else if (strcmp(argv[0], "cd") == 0) {
        // cd command
        if (argv[1] == NULL) {
            printf("Error: expected directory after 'cd'\n");
        } else if (strcmp(argv[1], "..") == 0) {
            // change to parent directory
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                printf("Error: failed to get current working directory\n");
            } else {
                // find the last slash in cwd
                char *last_slash = strrchr(cwd, '/');
                if (last_slash == NULL) {
                    printf("Error: invalid current working directory\n");
                } else {
                    // remove the last segment of the path
                    *last_slash = '\0';
                    if (chdir(cwd) != 0) {
                        printf("Error: failed to change directory\n");
                    }
                }
            }
        } else {
            int result = chdir(argv[1]);
            if (result != 0) {
                printf("Error: failed to change directory\n");
            }
        }
    } else if (strcmp(argv[0], "pwd") == 0) {
        // pwd command
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            printf("Error: failed to get current working directory\n");
        }
    } else if (strcmp(argv[0], "ls") == 0) {
        // ls command
        if (argv[1] == NULL){
            system("ls");
        } else {
            char command[100];
            strcpy(command, "ls ");
            strcat(command, argv[1]);
            system(command);
        }
    } else if (strcmp(argv[0], "mkdir") == 0) {
        // mkdir command
        if (argv[1] == NULL) {
            printf("Error: expected directory name after 'mkdir'\n");
        } else {
            int result = mkdir(argv[1], 0700);
            if (result != 0) {
                printf("Error: failed to create directory\n");
            }
        }
    } else if (strcmp(argv[0], "rm") == 0) {
        // rm command
        if (argv[1] == NULL) {
            printf("Error: expected directory name after 'rm'\n");
        } else {
            int status;
            if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "-rf") == 0) {
                if (argv[2] == NULL) {
                    printf("Error: expected file or directory name after '%s'\n", argv[1]);
                    return;
                }
                if (strcmp(argv[1], "-rf") == 0) {
                    char command[100];
                    sprintf(command, "rm -rf %s", argv[2]);
                    status = system(command);
                } else {
                    status = remove(argv[2]);
                }
            } else {
                status = remove(argv[1]);
            }
            if (status != 0) {
                printf("Error: failed to remove file or directory\n");
            }
        }
    } else if (strcmp(argv[0], "exit") == 0) {
        // exit command
        if (argv[1] != NULL) {
            exit_execution(argv);
        } else {
            exit_shell(argv);
        }
    } else if (strcmp(argv[0], "history") == 0) {
        // history command
        print_history();
    } else {
        // other commands
        pid_t pid = fork();
        if (pid == 0) {
            // child process
            execvp(argv[0], argv);
            printf("Error: failed to execute command\n");
            exit(1);
        } else if (pid < 0) {
            // fork failed
            printf("Error: failed to create new process\n");
        } else {
            // parent process
            wait(NULL);
        }
    }
}

/*  Take a string as input and split the input into individual
 *  tokens and return as an array of strings ('char **'). The
 *  strtok() is used to perform this splitting. The tokens array
 *  is dynamically allocated. There is a NULL terminator to mark
 *  the end of the 'tokens' array.
 */
char **split_line(char *line)
{
    /* Initialize buffer size and position to keep track of the size
     * of the 'tokens' array and current position in the array.
     * Also, allocate memory for the tokens array using malloc().
     */
    int buffer_size = MAX_LENGTH, position = 0;
    char **tokens = malloc(buffer_size * sizeof(char *));
    char *token;
    
    /* Use strtok() to split the input line into tokens based on
     * whitespace characters, and add each token to the tokens
     * array. If the tokens array is not big enough to hold all the
     * tokens, increase it size using realloc().
     */
    if (!tokens)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    token = strtok(line, " \t\r\n\a");
    
    while (token != NULL)
    {
        tokens[position] = token;
        position++;
        
        if (position >= buffer_size)
        {
            buffer_size += MAX_LENGTH;
            tokens = realloc(tokens, buffer_size * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        
        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}

int main(int argc, char **argv)
{
    char *line;
    char **args;
    char cwd[1024];
    int status;
    using_history();

    do
    {
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s >> ", cwd);
        }
        else
        {
            perror("getcwd() error");
            return 1;
        }

        line = readline(NULL);
        if (line == NULL) {
            printf("Exiting the shell...\n");
            break;
        } else if (strlen(line) > 0) {
            add_history(line);
            args = split_line(line);
            execute_command(args);
            free(args);
        }
        free(line);

    } while (1);

    return 0;
}
