#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Check to see if correct number of arguments were passed in. Must be 3, first arg will always be the program name.
    /*
     * list of accepted arguments
     * 1. program name
     * 2. source file name, must be in same directory as program
     * 3. destination file to write to, will be placed in same directory as program
     */
    if(argc != 3){
        printf("Error: Incorrect number of arguments passed.\nRun command as follows:\n");
        printf("\'./<program name> <source file> <destination file>\'\n");
        exit(1);
    }

    // Check to see if the input file exists before proceeding
    FILE *inputFileCheck = fopen(argv[1], "r");
    if(inputFileCheck == NULL){
        printf("Error: Unable to open source file \'%s\'.\n", argv[1]);
        exit(1);
    }

    // Variables for pipe and child process
    int pipefd[2];
    pid_t pid;

    // Create the pipe, check for errors
    if (pipe(pipefd) == -1) {
        printf("Error: Unable to create pipe.\n");
        exit(2);
    }

    //Error handling in case child creation fails
    pid = fork();
    if (pid == -1) {
        printf("Error: Unable to create child process.\n");
        exit(3);
    }

    // Child process
    if (pid == 0) {
        // Child process does not need to use the write portion of the pipe so we close it
        close(pipefd[1]);

        // Checks to see if the output file already exists, if so it show warning to user. Need to add y/n option.
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

        // Close pipe and file
        close(pipefd[0]);
        fclose(myFile);
        printf("File successfully copied from %s to %s.\n", argv[1], argv[2]);
    }
    // Parent process
    else {
        // Parent process does not need to use the read portion of the pipe so we close it
        close(pipefd[0]);

        // Open the input file for reading
        FILE *input = fopen(argv[1], "r");
        if (input == NULL) {
            printf("Error: Unable to open the file in the parent process.\n");
            exit(1);
        }

        // Read characters from the input file and write them to the pipe, need to look into how to use a proper buffer
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
