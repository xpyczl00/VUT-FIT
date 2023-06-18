/**
* Project - Compiler for IFJ22
* 
* @brief Scanner for IFJ22 - Lexical analysis
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file dyn_string.c
*
**/

#include <stdlib.h>
#include "dyn_string.h"

#define DYN_STRING_LENGTH 8 /// 1 byte

int dyn_string_init(Dyn_string *s)
{
    if ((s->string = (char *)malloc(DYN_STRING_LENGTH*sizeof(char))) != NULL)                         // Successful allocation
    {
        s->length=0;
        s->string[0]= '\0';                                                     // Setting parameters
        s->size = DYN_STRING_LENGTH;

        return ERROR_OK;
    }

    return ERROR_INTERN;
}

void dyn_string_free(Dyn_string *s)
{
    free(s->string);
}

void dyn_string_clear(Dyn_string *s)
{
    s->length = 0;
    s->size = 0;
    s->string[s->length] = '\0';
}

bool dyn_string_add_char(Dyn_string *s, char c)
{
    if (s->length + 1 >= s->size)
    {
        unsigned int new_size = s->length + DYN_STRING_LENGTH;
        if ((s->string = (char *)realloc(s->string, new_size))==NULL)                    // Successful reallocation
        {
            return false;
        }
        s->size = new_size;
    }

    s->string[s->length] = c;
    s->length++;
    s->string[s->length] = '\0';                                                        // Adding character and terminator

    return true;
}

bool dyn_string_add_const_str(Dyn_string *s, const char *const_string)
{
    int const_str_length = (int)strlen(const_string);

    if (s->length + const_str_length + 1 >= s->size)
    {
        int new_size = s->length + const_str_length + 1;
        if ((s->string = (char *)realloc(s->string, new_size))==NULL)                    // Successful reallocation
        {
            return false;
        }
        s->size = new_size;
    }

    s->length += const_str_length;
    strcat(s->string, const_string);
    s->string[s->length] = '\0';                                                         // Setting new lenght, concatenating and adding terminator

    return true;
}

bool dyn_string_copy(Dyn_string *src, Dyn_string *dest)
{
    unsigned int new_length = src->length;
    if (new_length >= (unsigned) dest->size)
    {
        if ((dest->string = (char *)realloc(dest->string, new_length + 1))==NULL)        // Successful reallocation
        {
            return false;
        }
        dest->size = new_length + 1;
    }

    if(new_length>0){
        strcpy(dest->string, src->string);
    }
    dest->length = new_length;                                                           // No need for terminator - it's already in 'src'

    return true;
}

int dyn_string_compare(Dyn_string s, const char *const_string)
{
    return strcmp(s.string, const_string);
}