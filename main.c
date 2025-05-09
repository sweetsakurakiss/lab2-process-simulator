#include <stdio.h>      // For printf(), perror()
#include <stdlib.h>     // For exit()
#include <unistd.h>     // For fork(), execvp()
#include <sys/wait.h>   // For wait(), WIFEXITED(), etc.

#define NUM_CHILDREN 10  // We will create 10 child processes

int main() {
    pid_t pid;       // Used to store child process ID
    int status;      // Used to store child exit status

    // Define 10 different commands for the child processes
    char *commands[NUM_CHILDREN][3] = {
        {"echo", "Hello Mariatu", NULL},     // Say Hello
        {"ls", "-l", NULL},                  // List files
        {"whoami", NULL},                    // Show current user
        {"date", NULL},                      // Show current date
        {"pwd", NULL},                       // Show current directory
        {"hostname", NULL},                  // Show computer name
        {"uname", "-a", NULL},               // Show system info
        {"id", NULL},                        // Show user ID info
        {"ps", NULL},                        // Show running processes
        {"uptime", NULL}                     // Show system uptime
    };

    // Loop to create child processes
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid = fork();  // Create a new process

        if (pid < 0) {
            // If fork() returns a negative number, there was an error
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0) {
            // This block runs in the child process
            printf("Child %d [PID: %d] is running command: %s\n",
                   i + 1, getpid(), commands[i][0]);

            // Replace child process image with command using execvp
            execvp(commands[i][0], commands[i]);

            // If execvp fails, we print an error and exit with code 1
            perror("Exec failed");
            exit(1);
        }

        // If pid > 0, we are in the parent and continue the loop to create more children
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t wpid = wait(&status);  // Wait for any child to finish

        if (WIFEXITED(status)) {
            // If the child exited normally
            printf("Parent: Child with PID %d exited with status %d\n",
                   wpid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            // If the child was terminated by a signal
            printf("Parent: Child with PID %d was terminated by signal %d\n",
                   wpid, WTERMSIG(status));
        } else {
            // If the child ended in another way (rare)
            printf("Parent: Child with PID %d ended abnormally\n", wpid);
        }
    }

    return 0;  // Program complete
}
