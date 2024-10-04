#ifndef _STDIO_COMMON_H_
#define _STDIO_COMMON_H_

#include <stdarg.h>

// --- DEFINE ---

// ...

// --- FUNCTION ---

/**
 * @brief Writes a formatted string to the buffer, up to a maximum of size characters.
 *
 * @param dest  Pointer to the buffer where the resulting string will be stored.
 * @param size  The maximum number of characters to write (including the null terminator).
 * @param fmt   The format string (similar to printf).
 * @param args  A va_list representing the variable arguments for formatting.
 * @return The number of characters written (not including the null terminator).
 *
 * This function formats the string according to the format specifiers provided in `fmt` and writes at most `size` characters
 * to the buffer `dest`. The variadic arguments are passed as a `va_list` for flexibility in function calls.
 */
int vsnprintf(char *dest, int size, const char *fmt, va_list args);

/**
 * @brief Writes a formatted string to the buffer, up to a maximum of size characters.
 *
 * @param dest  Pointer to the buffer where the resulting string will be stored.
 * @param size  The maximum number of characters to write (including the null terminator).
 * @param fmt   The format string (similar to printf).
 * @param ...   Additional arguments based on the format specifiers in `fmt`.
 * @return The number of characters written (not including the null terminator).
 *
 * This function formats the string according to the format specifiers provided in `fmt` and writes at most `size` characters
 * to the buffer `dest`. It accepts a variable number of arguments after `fmt` and is often used for formatted output to
 * strings.
 */
int snprintf(char *dest, int size, char *fmt, ...);

#endif // _STDIO_COMMON_H_
