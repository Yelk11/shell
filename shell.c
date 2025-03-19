#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <pwd.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64
#define HISTORY_SIZE 1000

// Shell state
char *history[HISTORY_SIZE];
int history_count = 0;
char current_dir[PATH_MAX];
char *home_dir;

// Function declarations
void print_prompt(void);
char *read_command(void);
char **parse_command(char *command);
void execute_command(char **args);
void handle_signal(int sig);
void add_to_history(const char *command);
void cleanup(void);

// Signal handler for Ctrl+C
void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("\n");
        print_prompt();
    }
}

// Get current working directory
void get_current_dir(void) {
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd() error");
        exit(1);
    }
}

// Print the shell prompt
void print_prompt(void) {
    char username[256];
    char hostname[256];
    struct passwd *pw = getpwuid(getuid());
    
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        strcpy(hostname, "unknown");
    }
    
    strcpy(username, pw ? pw->pw_name : "unknown");
    get_current_dir();
    
    printf("%s@%s:%s$ ", username, hostname, current_dir);
}

// Read command from user input
char *read_command(void) {
    char *input = malloc(MAX_INPUT_SIZE);
    if (!input) {
        perror("malloc failed");
        exit(1);
    }
    
    if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
        free(input);
        return NULL;
    }
    
    // Remove trailing newline
    input[strcspn(input, "\n")] = 0;
    return input;
}

// Parse command into arguments
char **parse_command(char *command) {
    char **args = malloc(MAX_ARGS * sizeof(char *));
    if (!args) {
        perror("malloc failed");
        exit(1);
    }
    
    char *token = strtok(command, " ");
    int i = 0;
    
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    
    args[i] = NULL;
    return args;
}

// Add command to history
void add_to_history(const char *command) {
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(command);
    }
}

// Execute the command
void execute_command(char **args) {
    if (args[0] == NULL) {
        return;
    }
    
    // Handle built-in commands
    if (strcmp(args[0], "cd") == 0) {
        char *path = args[1] ? args[1] : home_dir;
        if (chdir(path) != 0) {
            perror("cd failed");
        }
        return;
    }
    
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
    
    // Fork and execute external command
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        return;
    }
    
    if (pid == 0) {
        // Child process
        execvp(args[0], args);
        perror("exec failed");
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

// Cleanup allocated memory
void cleanup(void) {
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }
}

int main(void) {
    // Setup signal handling
    signal(SIGINT, handle_signal);
    
    // Get home directory
    home_dir = getenv("HOME");
    if (!home_dir) {
        home_dir = "/";
    }
    
    printf("Welcome to C Shell!\n");
    printf("Type 'exit' to quit\n");
    
    while (1) {
        print_prompt();
        
        char *command = read_command();
        if (!command) {
            printf("\n");
            break;
        }
        
        if (strlen(command) > 0) {
            add_to_history(command);
            char **args = parse_command(command);
            execute_command(args);
            free(args);
        }
        
        free(command);
    }
    
    cleanup();
    return 0;
} 