
#include "strbuild.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "system.h"

#define DEFAULT_STRING_SIZE 16

/**
 * @brief Initializes a new string builder.
 * @return A pointer to the newly created string builder.
 */
C_STR_PUBLIC(string_builder*) builder() {
    string_builder *str = (string_builder*) malloc(sizeof(struct _builder));
    str->_Dest = (char*) malloc(sizeof(char) * DEFAULT_STRING_SIZE);
    str->_Dest[0] = '\0';
    str->_Capacity = DEFAULT_STRING_SIZE;
    str->_Size = 0;
    return str;
}

/**
 * @brief Appends a source string to the string builder.
 * @param pBuilder Pointer to the string builder.
 * @param source   The source string to append.
 */
C_STR_PUBLIC(void) str_append(string_builder* __restrict__ pBuilder, const char* __restrict__ source) {
    while (strlen(source) > (pBuilder->_Capacity - pBuilder->_Size)) {
        str_reallocate(pBuilder);
    }
    strcat(pBuilder->_Dest, source);
    pBuilder->_Size = strlen(pBuilder->_Dest) + 1;
}

/**
 * @brief Reallocates memory for the string builder if necessary.
 * @param pBuilder Pointer to the string builder.
 */
C_STR_PUBLIC(void) str_reallocate(string_builder *pBuilder) {
    char* temp = realloc(pBuilder->_Dest, pBuilder->_Capacity * 2);
    if (temp) {
        pBuilder->_Dest = temp;
        pBuilder->_Capacity *= 2;
    }
}

/**
 * @brief Retrieves the string from the string builder.
 * @param pBuilder Pointer to the string builder.
 * @return A dynamically allocated string containing the contents of the string builder.
 */
C_STR_PUBLIC(char*) str_get(string_builder* pBuilder) {
    char* string = (char*) calloc(pBuilder->_Size + 1, sizeof(char));
    strcpy(string, pBuilder->_Dest);
    return string;
}

/**
 * @brief Serializes the string builder's content to a file.
 * @param pBuilder Pointer to the string builder.
 * @param fBuf     The filename to which to serialize the content.
 */
C_STR_PUBLIC(void) str_serialize(string_builder *pBuilder, const char* fBuf) {
    FILE* fptr = fopen(fBuf, "a");
    if (fptr == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(fptr, "%s\n", pBuilder->_Dest);
    if (fclose(fptr) != 0) {
        perror("Error closing file");
    }
}

/**
 * @brief Deserializes content from a file into the string builder.
 * @param pBuilder Pointer to the string builder.
 * @param fBuf     The filename from which to deserialize the content.
 */
C_STR_PUBLIC(void) str_deserialize(string_builder *pBuilder, const char* fBuf) {
    FILE* fptr = fopen(fBuf, "r");
    if (fptr == NULL) {
        perror("Error opening file");
        return;
    }
    char ch;
    str_append(pBuilder,"");
    while ((ch = fgetc(fptr))!=EOF) {
        char* string_format = character_parseString(ch);
        str_append(pBuilder, string_format);
        free(string_format);
    }
}

C_STR_PUBLIC(void) str_clear(string_builder *pBuilder) {
    pBuilder->_Dest[0] = '\0';
    pBuilder->_Size = 0;
}