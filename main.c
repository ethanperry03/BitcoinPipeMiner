/** PipeCoin - Ethan Perry - Nov 11, 2024
 * This program handles the creation and management of multiple processes to perform mining operations.
 * The main functionalities include: 
 *   - Creating a specified number of child processes using fork and pipe. 
 *   - Each child process executes the mining operation to find a nonce that produces 
 *     a hash with the required number of leading zeros. 
 *   - The first process to find a valid hash writes the result to a file, while all 
 *     other processes are terminated. 
 * 
 * Usage: ./program_name <input file> <output file> <leading zeros> <number of processes> * 
 * Parameters: 
 *   - input_file: The name of the file to read data from. 
 *   - output_file: The name of the file to write the resulting block. 
 *   - number_of_leading_zeros: The required number of leading zeros for the hash value.
 *   - number_of_processes: The number of parallel processes to use for mining. 
 * 
 * Note: 
 *   - The program starts by creating the specified number of child processes. 
 *   - Each child process performs the mining operation and communicates with the parent 
 *     process through pipes. 
 *   - The parent process waits for the first child process to find a valid hash and then 
 *     terminates all other child processes. 
 *   - The total execution time is calculated and displayed. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#define MEGA 1000000

void write_to_file(char* outfileName, char* block) {
    // declare counter
    int counter = 0;
    
    // create file pointer and file to write output to
    FILE* outfile = fopen(outfileName, "w");

    // while you are not at a null char, read the next and write it
    while (block[counter] != '\0') {
        // write the char into the file
        fprintf(outfile, "%c", block[counter]);
        // update the counter
        counter = counter + 1;
    }

    // close off file pointer
    fclose(outfile);
}   

int main(int argv, char** argc) {
    // error check valid number of inputs
    if (argv != 5) {
        fprintf(stderr, "Error: invalid number of inputs\n");
        exit(1);
    }

    // iterator
    int i;
    // input file name
    char* infile = argc[1];
    // output file name
    char* outfile = argc[2];

    // open file to read
    FILE* pfile = fopen(infile, "r");
    // check if file pointer is null (indicating invalid file)
    if (pfile == NULL) {
        printf("\nerror: '%s' is an invalid file\n", infile);
        printf("ensure file exists and entered correctly\n\n");
        exit(-1);
    }
    // close file pointer
    fclose(pfile);

    // get number of processes to start
    int zeros = atoi(argc[3]);
    // number of processes to start
    int procs = atoi(argc[4]);
    // declare argChild array to pass into next function
    char* argChild[5];
    // NULL terminate the input array of strings
    argChild[4] = NULL;

    // allocate array to store all the pids from the forks
    pid_t* processIDs = malloc(procs * sizeof(pid_t));
    // store an array of all the read/write pipe ends
    int allPipes[procs][2];
    // stores the writing end of each pipe and converted 
    // to a string to be passed to child process
    char buffer[20];
    // string to store the number of zeros
    char zeroStr[5];

    // start clock object
    struct timeval start, end;
    gettimeofday(&start, NULL);

    printf("\nRunning...\n");
    // start procs amount of processes to simulateous check nonces
    for (i = 0; i < procs; i++) {    
        // create each pipe before the fork
        int pipeRet = pipe(allPipes[i]);
        // if pipe failed, exit out
        if(pipeRet == -1) {
            printf("Pipe error\n");
            exit(-1);
        }

        // Create a new process and record pid
        pid_t pid = fork(); 
        
        // if the pid is less than 0, error flag, exit
        if (pid < 0) {
            printf("Fork failed\n");
            exit(-1);
        } 
        // else if it is a child process, start mining!
        else if (pid == 0) {
            // close the reading end for the child process
            close(allPipes[i][0]);

            // put in the porgram name for the execvp call
            argChild[0] = "./hasher";

            // add the input file name
            argChild[1] = infile;

            // pass the writing end of the childs pipe as a string
            sprintf(buffer, "%d", allPipes[i][1]);
            argChild[2] = buffer;

            // number of zeros that should be prepended for valid hashed value
            sprintf(zeroStr, "%d", zeros);
            argChild[3] = zeroStr;

            // call execvp to create a hasher for this child process
            execvp(argChild[0], argChild);

            // throw error if child gets passed execvp
            fprintf(stderr, "Child error\texecvp did not work\n");
            return 1;
        } 
        // Parent process records all pids
        else {
            // close the writing end of the pipe for the parent
            close(allPipes[i][1]);
            // record pid's 
            processIDs[i] = pid;
        }
    }
    
    // reap programs as they wake up and using wait, find which process just woke up
    pid_t procEnded;
    // &int param stores data related to process?
    procEnded = wait(NULL);
    
    // stop clock and calculate how long it has been
    gettimeofday(&end, NULL);
    // Calculate the elapsed time in seconds and microseconds
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    // If the end microseconds are smaller than the start microseconds, adjust the seconds
    if (microseconds < 0) {
        seconds--;
        microseconds += MEGA;
    }

    char* returnBuffer = malloc((MEGA+12) * sizeof(char));

    // procEnded is terminated, so kill all other processes
    for (int i = 0; i < procs; i++) {
        if (procEnded != processIDs[i]) {
            kill(processIDs[i], SIGINT);
            wait(NULL);
        }
        // else, it was the first child to return so print out message
        else {
            // read from the pipe and null terminate
            int numBytes = read(allPipes[i][0], returnBuffer, (MEGA+12-1)*sizeof(char));
            returnBuffer[numBytes] = '\0';

            // write to file
            write_to_file(outfile, returnBuffer);

            // flush output from std
            fflush(stdout);
        }
        // close reading end of ith pipe
        close(allPipes[i][0]);
    }

    // output message
    printf("\nSuccessful block outfile:   %s\n", outfile);
    printf("Amount of leading zeros:    %d\n", zeros);
    printf("Total time for program was: %1.6f\n\n", seconds + microseconds/(MEGA*1.0));

    // free memory
    free(processIDs);
    free(returnBuffer);

    return 0;
}   