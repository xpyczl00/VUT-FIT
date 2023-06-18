/**
 * @file error.c
 * @author Lukasz Pycz,xpyczl00 ,VUT FIT
 * @date 24/3/2021
 * @brief IJC-DU1, příklad a)
 * @details Definition of printf-like error functions
 *          Compiled: gcc 9.2
 */
#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

 
void error_exit(const char *fmt,...){
    va_list error;
    va_start(error, fmt);

    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, fmt, error);

    va_end(error);
    exit(1);
}
void warning_msg(const char *fmt, ...){
    va_list error;
    va_start(error, fmt);

    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, fmt, error);

    va_end(error);
    exit(1);


}
