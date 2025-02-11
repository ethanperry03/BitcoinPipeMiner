# Hash Mining Simulation with Pipes

## Overview
This project implements a hash mining simulation in C, utilizing random number generation, multiple processes, inter-process communication using pipes, and performance measurement. The project consists of two programs: `hasher` for mining a valid nonce and `main` for managing multiple mining processes using pipes.

## Features
- Random nonce generation for hash computation.
- Multi-process execution to improve efficiency.
- Uses SHA-256 hashing.
- Inter-process communication using pipes.
- Measures execution time of the mining process.
- Debugging with GDB and Valgrind.

## Compilation & Execution
### Build
```sh
make
```

### Run
#### hasher
```sh
./hasher <input_file> <pipe_write_end> <num_zeroes>
```
#### main
```sh
./main <input_file> <output_file> <num_zeroes> <num_processes>
```

### Examples
```sh
./main data.txt result.txt 5 4
```

## Implementation Details
The project is organized into two main programs:
- `hasher.c`: Reads an input file, appends a nonce, and computes a SHA-256 hash until it meets the required number of leading zeroes in binary. The result is written to a pipe and the program terminates.
- `main.c`: Spawns multiple `hasher` processes, manages pipes for communication, waits for one to complete, terminates the others, and writes the successful result to an output file.

### Hash Computation
- Reads file contents into a buffer (up to 1,000,000 characters).
- Appends a random 6-digit number and iterates through possible nonces.
- Converts the SHA-256 hash from hex to binary to check for leading zeroes.
- Uses `srand(gettimeofday)` for precise randomization.

### Multi-Process Execution with Pipes
- `main.c` starts multiple `hasher` processes, assigning each process a different pipe for writing results.
- Waits for the first successful result, then terminates the remaining processes using `SIGINT`.
- Reads the winning result from the pipe and writes it to the output file.
- Ensures no zombie processes by reaping terminated children.

### Signal Handling
- `hasher.c` installs a `SIGINT` handler to clean up and terminate gracefully.
- `main.c` sends `SIGINT` to remaining processes once a valid hash is found.

### Timing Execution
Uses `gettimeofday` to measure execution time accurately.

## Debugging
Use GDB and Valgrind to debug memory errors:
```sh
gdb ./hasher
valgrind --leak-check=full ./hasher data.txt 5
```

## Code Style & Best Practices
- No global variables; use function parameters.
- Functions should ideally be ≤ 20 lines.
- Use `malloc()` for dynamic memory allocation.
- Handle file errors gracefully.
- Ensure pipes are closed properly to prevent resource leaks.

## Additional Notes
- `CTRL+C` terminates an infinite loop.
- Ensure proper cleanup of child processes in `main.c`.
- Remove debugging code before submission.
