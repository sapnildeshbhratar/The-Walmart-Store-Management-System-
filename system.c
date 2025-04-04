
/*
 * File: system.h
 * --------------------
 * This header file defines a collection of utility functions for input/output operations, string manipulation,
 * and array handling. These functions are designed to simplify common programming tasks in C, such as reading
 * user input, parsing numeric values from strings, converting data types to strings, copying arrays, and obtaining
 * timestamps. They provide a portable and straightforward way to handle these operations in C programs.
 *
 * Usage:
 * 1. Include this header file in your C program using the #include directive.
 *    Example: #include "system.h"
 *
 * 2. Call the appropriate functions based on your requirements. Ensure that you link against the appropriate
 *    library if required (e.g., -lm for math functions).
 *
 *    Example usage:
 *    - Reading an integer from the standard input:
 *      int num = system_readInt();
 *
 *    - Parsing an integer from a string:
 *      const char* str = "123";
 *      int num = string_parseInt(str);
 *
 *    - Copying arrays:
 *      int sourceArray[] = {1, 2, 3, 4, 5};
 *      int destArray[5];
 *      system_arrayCopy(destArray, sourceArray, 5, sizeof(int));
 *
 *    - Obtaining a timestamp:
 *      char* timestamp = system_timeStamp();
 *
 * 3. Remember to free dynamically allocated memory (if any) returned by functions such as system_read and
 *    system_timeStamp using the free function when it's no longer needed to avoid memory leaks.
 */

#include "system.h"

/*
 * Function: system_readInt
 * Usage: int num = system_readInt();
 * -----------------------------------
 * Reads an integer from the standard input and returns it.
 */
C_SYSTEM_PUBLIC_INTERFACE(int)
system_readInt() {
    int stream;
    char buffer[100];
    scanf("%s",buffer);
    char* endPtr;
    stream = strtol(buffer, &endPtr, 10);
    return stream;
}

/*
 * Function: system_readLong
 * Usage: long num = system_readLong();
 * -------------------------------------
 * Reads a long integer from the standard input and returns it.
 */
C_SYSTEM_PUBLIC_INTERFACE(long)
system_readLong() {
    long stream;
    char buffer[100];
    scanf("%s",buffer);
    char* endPtr;
    stream = strtol(buffer, &endPtr, 10);
    return stream;
}

/*
 * Function: system_readFloat
 * Usage: float num = system_readFloat();
 * ---------------------------------------
 * Reads a floating-point number from the standard input and returns it.
 */
C_SYSTEM_PUBLIC_INTERFACE(float)
system_readFloat() {
    float stream;
    char buffer[100];
    scanf("%s",buffer);
    char* endPtr;
    stream = strtof(buffer, &endPtr);
    return stream;
}

/*
 * Function: system_readDouble
 * Usage: double num = system_readDouble();
 * -----------------------------------------
 * Reads a double-precision floating-point number from the standard input and returns it.
 */
C_SYSTEM_PUBLIC_INTERFACE(long double)
system_readDouble() {
    long double stream;
    char buffer[100];
    scanf("%s",buffer);
    char* endPtr;
    stream = strtold(buffer, &endPtr);
    return stream;
}

/*
 * Function: system_read
 * Usage: char* str = system_read(100);
 * --------------------------------------
 * Reads a string of specified size from the standard input and returns it.
 */
C_SYSTEM_PUBLIC_INTERFACE(char*)
system_read(int size) {
    fflush(stdin);
    char* stream = (char*) malloc(sizeof(char)*size);
    fgets(stream,size,stdin);
    stream[strlen(stream) - 1] = '\0';
    return stream;
}

/*
 * Function: system_readCharacter
 * Usage: char ch = system_readCharacter();
 * -----------------------------------------
 * Reads a single character from the standard input and returns it.
 */
C_SYSTEM_PUBLIC_INTERFACE(char)
system_readCharacter() {
    char ch;
    scanf("%c",&ch);
    return ch;
}

/*
 * Function: string_parseInt
 * Usage: int num = string_parseInt("123");
 * ----------------------------------------
 * Parses an integer from the given string and returns it.
 * Throws a ParseException if the string contains non-numeric characters.
 * Throws an OverflowException if the parsed integer exceeds the limit.
 */
C_SYSTEM_PUBLIC_INTERFACE(int)
string_parseInt(const char* string) {
    int valueInt = 0;
    int iterable;
    for (iterable = 0; iterable < 30 && string[iterable]!='\0' ; ++iterable) {
        if (string[iterable] < 48 || string[iterable] > 57) {
            throw ParseException();
        }
        valueInt = valueInt * 10 + (string[iterable] - 48);
    }
    if (iterable == 31) { throw OverflowException(); }
    else { return valueInt; }
}

/*
 * Function: string_parseDecimal
 * Usage: double num = string_parseDecimal("123.456");
 * ---------------------------------------------------
 * Parses a double-precision floating-point number from the given string and returns it.
 * Throws a ParseException if the string contains non-numeric characters.
 */
C_SYSTEM_PUBLIC_INTERFACE(double)
string_parseDecimal(const char* string) {
    double valueDouble = 0;
    int iterable;
    for (iterable = 0; iterable < 30 && string[iterable]!='\0' && string[iterable]!='.' ; ++iterable) {
        if (string[iterable] < 48 || string[iterable] > 57) {
            throw ParseException();
        }
        valueDouble = valueDouble * 10 + (string[iterable] - 48);
    }

    if (string[iterable] == '.') {
        iterable++;
        int count = 0;
        int decimal = 10;
        while (string[iterable]!='\0' && count < 8) {
            if (string[iterable] < 48 || string[iterable] > 57) {
                throw ParseException();
            }
            valueDouble = valueDouble * 10 + (string[iterable] - 48);
            count++;
            iterable++;
            decimal = decimal * 10;
        }
        decimal /= 10;
        valueDouble = valueDouble / decimal;
    }
    return valueDouble;
}

/*
 * Function: decimal_parseString
 * Usage: char* str = decimal_parseString(123.456);
 * -------------------------------------------------
 * Converts the given double-precision floating-point number to a string and returns it.
 */
C_SYSTEM_PUBLIC_INTERFACE(char*)
decimal_parseString(const double consumer) {
    int length = (int)log10(consumer) + 1 + 15;
    char* string = (char*) malloc(length + 1);
    string[0] = '\0';
    snprintf(string, length, "%s%lf", string, consumer);
    return string;
}

/*
 * Function: integer_parseString
 * Usage: char* str = integer_parseString(123);
 * ---------------------------------------------
 * Converts the given integer to a string and returns it.
 */
C_SYSTEM_PUBLIC_INTERFACE(char*)
integer_parseString(const int consumer) {
    int length = (int)log10(consumer) + 2;
    char* string = (char*) malloc(length + 1);
    string[0] = '\0';
    snprintf(string, length, "%s%d", string, consumer);
    return string;
}

/*
 * Function: character_parseString
 * Usage: char* str = character_parseString('A');
 * -----------------------------------------------
 * Converts the given character to a string and returns it.
 */
C_SYSTEM_PUBLIC_INTERFACE(char*)
character_parseString(const char consumer) {
    char* string = (char*) malloc(2);
    string[1] = '\0';
    string[0] = consumer;
    return string;
}

/*
 * Function: system_arrayCopy
 * Usage: system_arrayCopy(dest, source, size, type_size);
 * --------------------------------------------------------
 * Copies the contents of one array to another.
 */
C_SYSTEM_PUBLIC_INTERFACE(void)
system_arrayCopy(void* dest, void* source, int size, int type_size) {
    size_t total_bytes = size * type_size;

    char* dest_ptr = (char*) dest;
    char* source_ptr = (char*) source;

    for (int iterable = 0; iterable < total_bytes; ++iterable) {
        *(dest_ptr + iterable) = *(source_ptr + iterable);
    }
}

/*
 * Function: system_timeStamp
 * Usage: char* timestamp = system_timeStamp();
 * ---------------------------------------------
 * Retrieves the current timestamp and returns it as a string.
 */
C_SYSTEM_PUBLIC_INTERFACE(char*)
system_timeStamp() {
    time_t curr_time;
    time(&curr_time);
    char* strTime = ctime(&curr_time);
    return strTime;
}

/*
 * Function: date_get
 * Usage: int valueDate = date_get(date);
 * ---------------------------------------------
 * Fetches the date from the const character string
 */
C_SYSTEM_PUBLIC_INTERFACE(int)
date_get(const char* date) {
    int valueDate = 0;
    valueDate = (date[0] - 48) * 10 + (date[1] - 48);
    return valueDate;
}

/*
 * Function: date_getMonth
 * Usage: int valueMonth = date_getMonth(date);
 * ---------------------------------------------
 * Fetches the month from the const character string
 */
C_SYSTEM_PUBLIC_INTERFACE(int)
date_getMonth(const char* date) {
    int valueMonth = 0;
    valueMonth = (date[3] - 48) * 10 + (date[4] - 48);
    return valueMonth;
}

/*
 * Function: date_getYear
 * Usage: int valueYear = date_getYear(date);
 * ---------------------------------------------
 * Fetches the year from the const character string
 */
C_SYSTEM_PUBLIC_INTERFACE(int)
date_getYear(const char* date) {
    int valueYear;
    valueYear = (date[6] - 48) * 1000 + (date[7] - 48)*100 + (date[8] - 48)*10 + (date[9] - 48);
    return valueYear;
}

C_SYSTEM_PUBLIC_INTERFACE(int)
system_currDate() {
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);
    return localTime->tm_mday;
}

C_SYSTEM_PUBLIC_INTERFACE(int)
    system_currMonth() {
        time_t currentTime;
        struct tm *localTime;

        time(&currentTime);
        localTime = localtime(&currentTime);
        return localTime->tm_mon + 1;
    }

C_SYSTEM_PUBLIC_INTERFACE(int)
    system_currYear() {
        time_t currentTime;
        struct tm *localTime;

        time(&currentTime);
        localTime = localtime(&currentTime);
        return localTime->tm_year + 1900;
    }