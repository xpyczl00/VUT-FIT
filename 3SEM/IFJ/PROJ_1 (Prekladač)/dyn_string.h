/**
* Project - Compiler for IFJ22
* 
* @brief Scanner for IFJ22 - Lexical analysis
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file dyn_string.h
*
**/

#ifndef _DYN_STRING_H
#define _DYN_STRING_H

#include <stdbool.h>
#include <string.h>
#include "error_codes.h"


/**
 * @struct Dynamic string
 * @brief Representation of dynamic string with its value, length and size 
 */
typedef struct
{
    char *string;
     int length;
     int size;
} Dyn_string;

/**
 * @param s Pointer to dynamic string
 * @brief Initialization of memory for dynamic string
 * @return True if success, else false
 */
int dyn_string_init(Dyn_string *s);

/**
 * @param s Pointer to dynamic string
 * @brief Frees the memory allocated for string
 * @return Void
 */
void dyn_string_free(Dyn_string *s);

/**
 * @param s Pointer to dynamic string.
 * @brief Clears dynamic string content
 * @return Void
 */
void dyn_string_clear(Dyn_string *s);

/**
 * @param s Dynamic string
 * @param c Char to add
 * @brief Adds char to the end of dynamic string
 * @return True if success, else false
 */
bool dyn_string_add_char(Dyn_string *s, char c);

/**
 * @param s Dynamic string
 * @param const_string Constant string
 * @brief Adds constant string to the end of dynamic string
 * @return True if success, else false
 */
bool dyn_string_add_const_str(Dyn_string *s, const char *const_string);

/**
 * @param src Source string
 * @param dest Destination string
 * @brief  Copies string from source to destination
 * @return True if success, else false
 */
bool dyn_string_copy(Dyn_string *src, Dyn_string *dest);

/**
 * @param s Dynamic string
 * @param const_string Constant string
 * @brief  Compares dynamic string with constant string
 * @return Returns 1, 0, or -1 (greater than, equal to, or less than s2)
 */
int dyn_string_compare(Dyn_string s, const char *const_string);

#endif  /* DYN_STRING_H*/