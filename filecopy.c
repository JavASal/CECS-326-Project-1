#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int pipefd[2];
    pid_t pid;

    // Create the pipe, check for errors
    if (pipe(pipefd) == -1) {
        printf("Error creating a pipe.\n");
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        printf("Error during fork.\n");
        exit(1);
    }

    // Child process
    if (pid == 0) {
        // Close unused pipe write end in child process
        close(pipefd[1]);

        // Check if the output file exists and warn the user
        FILE *myFile = fopen(argv[2], "r");
        if (myFile != NULL) {
            printf("Output file exists and will be overwritten!\n");
            fclose(myFile);
            myFile = fopen(argv[2], "w");
        } else {
            myFile = fopen(argv[2], "w");
        }

        // Read from the pipe and write to the output file
        char ch;
        while (read(pipefd[0], &ch, 1) > 0) {
            // Write each character to the output file
            fputc(ch, myFile);
        }

        // Close pipe and file after use
        close(pipefd[0]);
        fclose(myFile);
    }
    // Parent process
    else {
        // Close unused pipe read end in parent process
        close(pipefd[0]);

        // Open the input file for reading
        FILE *input = fopen(argv[1], "r");
        if (input == NULL) {
            printf("There was an error opening the file in the parent process.\n");
            exit(1);
        }

        // Read characters from the input file and write them to the pipe
        char ch;
        while ((ch = fgetc(input)) != EOF) {
            write(pipefd[1], &ch, 1);
        }

        // Close input file and pipe after use
        fclose(input);
        close(pipefd[1]);
    }

    return 0;
}
