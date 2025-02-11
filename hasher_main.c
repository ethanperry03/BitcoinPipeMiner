/** Hasher main - Ethan Perry - Nov 11, 2024 
 * This program handles command-line input, reads data from a specified file, 
 * performs a mining loop to find a sufficient hashed value, and writes the 
 * processed data to a specified pipe. The main functionalities include: 
 * - Handling SIGINT signals to terminate the process gracefully. 
 * - Parsing command-line arguments to retrieve the input file name, write pipe, 
 *   and the number of leading zeros required for the hashed value.
 * - Reading the content of the specified file, excluding certain characters, 
 *   and returning a dynamically allocated array with the file content. 
 * - Executing a mining loop to try nonces and find a hash value with the
 *   required number of leading zeros. 
 * - Writing the processed data into a specified pipe and closing the pipe. 
 * - Freeing allocated memory before exiting.  
 *
 *  Parameters: 
 *   - input_file: The name of the file to read data from. 
 *   - write_pipe: The write end of the pipe to send the processed data. 
 *   - number_of_leading_zeros: The number of leading zeros to prepend to the hashed value. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>  
#include <signal.h>
#include "hasher_io.h"  
#include "hasher_mine.h"

void handle_sigint(int sig) {
    // send signal message
    printf("Process was ended by a signal\n");
    // exit
    exit(-1);
}

int main(int argv, char** argc) {
    // declare signal handler
    signal(SIGINT, handle_sigint);

    // declare variables
    int zeros, offset;
    char* writePipe;
    char* infile;

    // parse the cmd line input and return by "reference" to store vars in main
    parse_cmd(argv, argc, &infile, &writePipe, &zeros);

    // store the block text by reading in the file and recording the amount in the array
    char* block = read_file(infile, &offset);

    // do the mining loop to try nonces and find sufficient hashed value
    block = mining(block, offset, zeros);

    // output to file and pass output file name to create file
    output_to_pipe(block, writePipe, offset);

    // free memory
    free(block);

    return 0;
}