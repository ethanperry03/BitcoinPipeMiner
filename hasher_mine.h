#ifndef __HASHER_MINE__
#define __HASHER_MINE__

/** hasher_mine - Ethan Perry - Nov 11, 2024
 * This program includes functions to generate random numbers, update values in an array, 
 * convert hexadecimal characters to decimal, check the number of leading zeros in a hashed value, 
 * and perform mining operations to find a sufficient hashed value. 
 * 
 * The main functionalities include: 
 *   - generate_rand: Generates a random 6-digit number based on the current time of day. 
 *   - update_value: Updates a specified position in an array with a given value. 
 *   - hex_to_decimal: Converts a hexadecimal character to its decimal equivalent. 
 *   - check_hashed: Checks if a hashed value has the required number of leading zeros. 
 *   - mining: Generates and updates random nonces, hashes values, and checks if the hash meets the criteria. 
 * 
 * * Note: 
 *   - The generate_rand function uses the current time of day to seed the random number generator, 
 *     ensuring a different random number is produced each time the function is called. 
 *   - The update_value function converts a numeric value to a string and updates the array at the specified position. 
 *   - The hex_to_decimal function converts hexadecimal characters ('0' to '9' and 'a' to 'f') to decimal values. 
 *   - The check_hashed function checks if the hash value has the specified number of leading zeros. 
 *   - The mining function attempts to find a nonce that produces a hash with the required number of leading zeros.
 */

/** int generate_rand();
 * Generates a random 6-digit number seeded by the current time of day.
 * @return int The generated random number.
 */
int generate_rand();

/** char* update_value(char*, int, int);
 * Updates the given array at the specified offset with a given value, 
 * formatted as a 6-digit number (either the random num or the counter).
 * @param array The array to be updated.
 * @param offset The starting index to update in the array.
 * @param val The value to be inserted into the array.
 * @return char* The updated array.
 */
char* update_value(char*, int, int);

/** int hex_to_decimal(char);
 * Converts a hexadecimal character to its decimal value.
 * @param hex  The hexadecimal character.
 * @return int The corresponding decimal value.
 */
int hex_to_decimal(char);

/** int check_hashed(char*, int);
 * Checks if the hashed value has the required number of leading zeros.
 * @param hashed The hashed value in hexadecimal format.
 * @param zeros  The number of leading zeros required.
 * @return int   Return 1 if the hashed value meets the requirement, otherwise 0.
 */
int check_hashed(char*, int);

/** char* mining(char*, int, int);
 * Mines for a valid hashed value with the specified number of leading zeros.
 * @param array  The array containing the data to be hashed.
 * @param offset The offset in the array where the nonce starts.
 * @param zeros  The number of leading zeros required in the hashed value.
 * @return char* The array with the valid hashed value for output.
 */
char* mining(char*, int, int);

#endif