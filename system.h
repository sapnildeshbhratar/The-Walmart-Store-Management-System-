

#ifndef SYSTEM_SYSTEM_H
#define SYSTEM_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>

/* Windows  */
#define SYS_STDCALL __stdcall

/*
 * Define C_SYSTEM_PUBLIC_INTERFACE macro to specify visibility and calling convention for exported functions.
 */
#ifdef _WIN64
#define C_SYSTEM_PUBLIC_INTERFACE(type)   __declspec(dllexport) type SYS_STDCALL __attribute__((unused))
#elif __GNUC__
#define C_SYSTEM_PUBLIC_INTERFACE(type)   __attribute__((visibility("default"))) type SYS_STDCALL __attribute__((unused))
#endif

/*
 * Macro for throwing exceptions.
 */
#define throw \
fprintf(stderr,"Exception at Line ::%d in File ::%s",__LINE__,__FILE__);

/*
 * Macro for throwing overflow exception.
 */
#define OverflowException() \
fprintf(stderr, " Number of range"); exit(EXIT_FAILURE)

/*
 * Macro for throwing parse exception.
 */
#define ParseException() \
fprintf(stderr, " Number is not a digit"); exit(EXIT_FAILURE)

/*
 * Function declarations
 */

/* These functions facilitate reading various types of data from the standard input stream (stdin).
 * They prompt the user for input and return the entered values of different data types.
 * If the input does not match the expected data type, it may cause unexpected behavior.*/
C_SYSTEM_PUBLIC_INTERFACE(int) system_readInt();
C_SYSTEM_PUBLIC_INTERFACE(long) system_readLong();
C_SYSTEM_PUBLIC_INTERFACE(float) system_readFloat();
C_SYSTEM_PUBLIC_INTERFACE(long double) system_readDouble();
C_SYSTEM_PUBLIC_INTERFACE(char*) system_read(int size);
C_SYSTEM_PUBLIC_INTERFACE(char) system_readCharacter();

/* These functions handle string parsing and conversion operations.
 * They parse numeric values from strings, convert numeric values to string representations,
 * and convert individual characters to string representations.
 * If the input strings contain non-numeric characters or if the parsed values exceed the expected range,
 * exceptions are thrown.*/
C_SYSTEM_PUBLIC_INTERFACE(int) string_parseInt(const char* string);
C_SYSTEM_PUBLIC_INTERFACE(double) string_parseDecimal(const char* string);


C_SYSTEM_PUBLIC_INTERFACE(char*) decimal_parseString(const double consumer);
C_SYSTEM_PUBLIC_INTERFACE(char*) integer_parseString(const int consumer);
C_SYSTEM_PUBLIC_INTERFACE(char*) character_parseString(const char consumer);

/* This function is responsible for copying the contents of one array to another.
 * It ensures that the destination array has enough space to accommodate the copied elements.
 * If the size of the destination array is insufficient, it may lead to buffer overflow. */
C_SYSTEM_PUBLIC_INTERFACE(void) system_arrayCopy(void* dest, void* source, int size, int type_size);

/* This function retrieves the current timestamp and returns it as a string representation.
 * The timestamp includes the current date and time in a human-readable format.
 * The returned string should be freed after use to avoid memory leaks. */
C_SYSTEM_PUBLIC_INTERFACE(char*) system_timeStamp();
C_SYSTEM_PUBLIC_INTERFACE(int) system_currDate();
C_SYSTEM_PUBLIC_INTERFACE(int) system_currMonth();
C_SYSTEM_PUBLIC_INTERFACE(int) system_currYear();

/* These functions extract day, month, and year components from the given date string.
 * They assume that the date string follows the format "dd-mm-yyyy".
 * If the format of the date string is incorrect, the extracted components may be inaccurate. */
C_SYSTEM_PUBLIC_INTERFACE(int) date_get(const char* date);
C_SYSTEM_PUBLIC_INTERFACE(int) date_getMonth(const char* date);
C_SYSTEM_PUBLIC_INTERFACE(int) date_getYear(const char* date);


#endif //SYSTEM_SYSTEM_H
