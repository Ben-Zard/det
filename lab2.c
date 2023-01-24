#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>


int main() {
char input[256];
char *token;
char *args[64];
int i;
pid_t pid;
struct rusage usage;
while (1) {
    printf("$ "); // Display prompt
    fgets(input, 256, stdin); // Receive user input
    token = strtok(input, " \n"); // Tokenize the line


    i = 0;
    while (token != NULL) {
        args[i] = token; // Add each token to the args array
        token = strtok(NULL, " \n");
        i++;
    }
    args[i] = NULL; // Null terminate the args array


    if (strcmp(args[0], "quit") == 0) { // Exit the shell if user enters "quit"
        exit(0);
    }


    pid = fork(); // Spawn a child process
    if (pid == 0) {
        execvp(args[0], args); // Use execvp to execute the command
        perror("Error: ");
        exit(1);
    } else {
        waitpid(pid, NULL, 0); // Wait for the child process to finish
        getrusage(RUSAGE_CHILDREN, &usage); // Collect resource usage statistics
        printf("User CPU time used: %ld.%06ld seconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
        printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);
    }
}
return 0;
}