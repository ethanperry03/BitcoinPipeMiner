#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#define MEGA 1000000

void parse_cmd(int argv, char** argc, char** infile, char** writeEnd, int* zeros){    
    // Ensure all options are entered into command line
    if (argv != 4) {
        printf("error: %d parameters received\nexpected three parameters\n", argv - 1);
        exit(-1);
    }

    // set infile to expected file name from cmd line input
    *infile = argc[1];

    // save outfile name
    *writeEnd = argc[2];

    // check that integer is reasonable
    *zeros = atoi(argc[3]);
    if (*zeros < 0 || *zeros > 256) {
        printf("%d is not a valid amount of preceding\nzeros for the hashed value\n", *zeros);
        exit(-1);
    }
}

char* read_file(char* filename, int* offset) {
    // dymaically allocate chunk of memory that can store 1MB of characters
    // 6 characters for rand, 6 characters for counter, and 1 for null term
    char* array = malloc(MEGA + 12 + 1);

    // open file to read
    FILE* infile = fopen(filename, "r");

    // check pointer is valid
    if (infile == NULL) {
        printf("Invalid input file, please try again\n");
        exit(-1);
    }

    // counter is 0
    int index = 0;
    // char
    char cha;
    // Read characters until the end of the file
    while ((cha = fgetc(infile)) != EOF) {
        // ignore newline and \r characters
        if (cha != '\n' && cha != '\r' && cha != '\t') {  
            array[index] = cha;
            index = index + 1;
        }
    }
    // close file pointer
    fclose(infile);
    // return the offset of where the input stuff ends 
    *offset = index;
    // null terminate
    array[index+12] = '\0';
    // return array
    return array;
}

void output_to_pipe(char* block, char* writePipe, int offset) {
    // convert pipe string to the actual integer value
    int writeVal = atoi(writePipe);
    // write the block into the writeVal end and of size offset+12 digits
    write(writeVal, block, (offset+12)*sizeof(char));
    // close the pipe
    close(writeVal);
}   