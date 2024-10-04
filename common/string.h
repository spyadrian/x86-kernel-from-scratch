#ifndef _STRING_COMMON_H_
#define _STRING_COMMON_H_

#include "types.h"

// --- DEFINE ---

// ...

// --- FUNCTION ---

/**
 * @brief Returns the length of the string.
 *
 * @param s  Pointer to the null-terminated string whose length is to be determined.
 * @return The number of characters in the string, excluding the null terminator.
 */
uint_t strlen(const char *s);

/**
 * @brief Copies up to `count` characters from the source string to the destination.
 *
 * @param dest   Pointer to the destination buffer.
 * @param src    Pointer to the source string.
 * @param count  Maximum number of characters to copy.
 * @return A pointer to the destination string.
 *
 * This function copies at most `count` characters from the string `src` to `dest`. If the length of `src` is less than `count`,
 * the remainder of `dest` will be padded with null characters.
 */
char *strncpy(char *dest, const char *src, uint_t count);

/**
 * @brief Compares two strings alphanumerically.
 *
 * @param s1  Pointer to the first string.
 * @param s2  Pointer to the second string.
 * @return 0 if the strings are identical, a value > 0 if `s1` is greater than `s2`, and a value < 0 if `s1` is less than `s2`.
 *
 * This function compares two strings `s1` and `s2` lexicographically. The comparison is based on the alphanumeric value of the characters.
 */
int strcmp(const char *s1, const char *s2);

/**
 * @brief Compares two strings alphanumerically, but only up to `n` characters.
 *
 * @param s1  Pointer to the first string.
 * @param s2  Pointer to the second string.
 * @param n   Maximum number of characters to compare.
 * @return 0 if the strings are identical, a value > 0 if `s1` is greater than `s2`, and a value < 0 if `s1` is less than `s2`.
 *
 * This function is identical to `strcmp`, but only compares the first `n` characters of the strings.
 */
int strncmp(const char *s1, const char *s2, uint_t n);

/**
 * @brief Converts a string to its integer representation.
 *
 * @param s  Pointer to the string to convert.
 * @return The integer representation of the string.
 *
 * This function converts a string `s` to an unsigned integer (`uint_t`). Note that it does not validate the contents of the string,
 * so it assumes the string contains valid numeric characters.
 */
uint_t atoi(const char *s);

/**
 * @brief Converts a string to its lowercase representation in place.
 *
 * @param s  Pointer to the string to convert.
 * @return A pointer to the modified string.
 *
 * This function modifies the string `s` in place, converting all uppercase letters to their lowercase equivalents.
 */
char *tolower(char *s);

#endif // _STRING_COMMON_H_
