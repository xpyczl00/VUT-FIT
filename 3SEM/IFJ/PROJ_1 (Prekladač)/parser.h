/**
* Project - Compiler for IFJ22
*
* @brief Parser for IFJ22 - Syntax analysis
*
* @author Matej Koreň <xkoren10
* @author Lukasz Pycz <xpyczl00>
* @file parser.h
*
**/

#ifndef _PARSER_H
#define _PARSER_H

#include "error_codes.h"
#include "dyn_string.h"
#include "symtable.h"
#include "scanner.h"
#include "expressions.h"
#include <stdbool.h>



/**
 * @brief Analysis of prolog
 * <Prolog> => <?php declare (strict_types=1);
 * @return Errors
*/
int Prolog_check();

/**
 * @brief Analysis of function definition
 * <Define_function> => <Function_header> <Command_sequence>
 * @return Errors
*/
int Define_function();

/**
 * @brief Analysis of function header
 * <Function_header> => function <ID> (<List_of_parameters> ) : <Return_type>
 * @return Errors
*/
int Function_header();

/**
 * @brief Analysis of command sequence
 * <Parameter_definition> => <Parameter_type> <Parameter_ID>
 * @return Errors
*/
int Command_sequence();

/**
 * @brief Analysis of command 
 * <Command> => <Parameter_ID> = <Expression>
 * @return Errors
*/
int Command();

/**
 * @brief Analysis of command with function
 * <Command> => <Parameter_ID> = <Expression>
 * @return Errors
*/
int Command_Function();

/**
 * @brief Analysis of command IF
 * <Command> => <Parameter_ID> = <Expression>
 * @return Errors
*/
int Command_IF();

/**
 * @brief Analysis of command WHILE
 * <Command> => <Parameter_ID> = <Expression>
 * @return Errors
*/
int Command_WHILE();

/**
 * @brief Analysis of input parameters
 * 
 * @return Errors
*/
int Input_parameters();

/**
 * @brief Analysis of list of parameters
 * <List_of_parameters> => <Parameter_definition>, <List_of_parameters>
 * @return Errors
*/
int List_of_parameters();

/**
 * @brief Analysis of parameter
 * <Parameter_definition> => <Parameter_type> <Parameter_ID>
 * @return Errors
*/
int Parameter_definition();

/**
 * @brief Analysis of epilog
 * <Epilog> => ?>
 * @return Errors
*/
int Epilog();

/**
 * @brief Analysis of program
 * <Program> => <Prolog> <Define_function> <Program_body><Epilog> EOF
 * @return Errors
*/
int Program();

#endif