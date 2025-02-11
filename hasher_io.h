#ifndef __HASHER_IO__
#define __HASHER_IO__

/** hasher_io - Ethan Perry - Nov 11, 2024
 * Helps mining simulator by parsing command line arguments, error checking them, reading
 * the input file, and writing the data through a pipe.
 *
 * This file includes several key functionalities:
 * - Parsing command-line arguments to get the input file, writing end of pipe, 
 *   and the number of leading zeros.
 * - Reading the contents of the input file into a dynamically allocated array.
 * - Writing the data to the pipe
 */

/** void parse_cmd(int, char**, char**, char**, int*);
 * Parses command-line arguments and sets the input file, writing end of pipe, and zeros value.
 * @param argv Number of command-line arguments.
 * @param argc Array of command-line argument strings.
 * @param infile Pointer to store the input file name.
 * @param writeEnd Pointer to store the int value of the write end to the pipe.
 * @param zeros Pointer to store the number of leading zeros.
 * @return void
 */
void parse_cmd(int, char**, char**, char**, int*);

/** char* read_file(char*, int*);
 * Reads a file into a dynamically allocated array and returns the offset.
 * @param filename Name of the file to read.
 * @param offset   Pointer to store the offset where the input ends.
 * @return char*   Pointer to the dynamically allocated array containing the file contents.
 */
char* read_file(char*, int*);

/** void output_to_pipe(char*, char*, int);
 * Writes the contents of a block to a specified output file.
 * @param block Pointer to the block of data to write.
 * @param writePipe Writing end of the pipe
 * @param offset Used to only write valid characters to the pipe from block
 * @param outfileName Name of the output file.
 * @return void
 */
void output_to_pipe(char*, char*, int);

#endif