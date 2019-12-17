#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdint.h>

// Print a null-terminated string on standard error.
void __print_err(const char *s);

// Print a null-terminated string on standard output.
void __print(const char *s);

// Print a 32 bit signed integer on standard output.
void __print_int(int32_t i);

// Flush the standard output.
void __flush(void);

// Read a char from standard input and return a string
// containing it. If no char is available (end-of-file),
// return the empty string.
const char *__getchar(void);

// Return the ASCII code of the char in first position
// in the string, or -1 if the string is empty.
int32_t __ord(const char *s);

// Return a string containing the character whose ASCII
// code is given as argument. 0 will return the empty
// string, negative values or values above 255 will
// bail out with a fatal runtime error.
const char *__chr(int32_t i);

// Return the length of a string.
int32_t __size(const char *);

// Return a substring of s starting at character first
// with length length. Bail out with a fatal runtime error
// if first or length are negative or if the requested
// substring is out of bounds.
//
// Getting an empty substring is possible (length = 0)
// as long as first + length is not greater than the
// string length, so __substring("", 0, 0) is acceptable.
const char *__substring(const char *s, int32_t first, int32_t length);

// Concatenate two strings.
const char *__concat(const char *s1, const char *s2);

// Compare two strings and return -1, 0, or 1.
int32_t __strcmp(const char *s1, const char *s2);

// Check if two strings are equal and return 0 or 1.
int32_t __streq(const char *s1, const char *s2);

// Logical not, return 0 or 1.
int32_t __not(int32_t i);

// Exit to the operating system with the given exit status.
void __exit(int32_t c);

#endif // RUNTIME_H
