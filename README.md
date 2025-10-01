# CECS-326-Project-1

File Copy project, this program is a command line program that copies an input file into an output destination.

This program requires GCC 15.2.1 or cmake min version 4.1.1 on linux machines for compilation. Compilation can be achieved doing the following:
"gcc filecopy.c -o filecopy"

Once compiled the program can be run with the following:
./filecopy <sourcefile> <destination file>

- Besides the program name itself the only two other valid parameters are a valid source file.
- If a non-existing source file or non-accessible source file if given program will not proceed.
- Should the destination file not exist one will be created.
- Important to note that if a destination file ALREADY EXISTS it will be OVERWRITTEN!