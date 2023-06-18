/**
* Project - Compiler for IFJ22
* 
* @brief Code generator for IFJ22
* 
* @author Matej Koreň <xkoren10>
* @author Matej Hložek <xhloze02>
* @file code_gen.h
*
**/

#ifndef _CODE_GEN_H
#define _CODE_GEN_H

#include "scanner.h"
#include "symtable.h"

/**
 * @brief Generates prologue of ifj22code file
 */
void gen_header();

/**
 * @brief Generates instruction and pushes the result to stack
 * @param type Operator / instruction token
 */
void gen_instruction(Token op);

/**
 * @brief Generates push instruction with given token
 * @param op Token
 */
void gen_push(Token op);

/**
 * @brief Generates pushs with false
 */
void gen_push_false();

/**
 * @brief Generates pop instruction, pops into given variable
 * @param var Variable to pop into
 */
void gen_pops(ht_item_t var);

/**
 * @brief Generates variable declaration
 * @param id Name of the variable
 */
void gen_var_def(char* id);

/**
 * @brief Generates setting of a variable with given type and value
 * @param var Variable we want to set (value.string)
 * @param val Value we want to set
 */
void gen_var_setval(Token var, Token val);
// MOVE <where> <what>  / MOVE <Symbol.var> <type@value>

/**
 * @brief Generates Type instruction with variable where symbol type will be set
 * @param var Variable where type will be stored
 * @param symbol Symbol of which type will be stored
 */
void gen_type(Token var, Token symbol);

/**
 * @brief Generates strlen function which returns length of symbol to var
 * @param var Variable where length will be stored
 * Symbol will be popped from stack
 */
void gen_strlen(char* var_name);

/**
 * @brief Generates getchar function which gets char from left_symbol on index right_symbol and returns it to var
 * @param var Variable where character will be stored
 * @param left_symbol String or variable from which character will be received
 * @param right_symbol Index position of character
 */
void gen_getchar(Token var, Token left_symbol, Token right_symbol);

/**
 * @brief Generates setchar function which sets right_symbol as char of var on index right_symbol
 * @param var Variable which acts both as source string as well as variable where changed string will be stored
 * @param left_symbol Index of char
 * @param right_symbol Single character which will be set
 */
void gen_setchar(Token var, Token left_symbol, Token right_symbol);

/**
 * @brief Generates start of an user function
 * @param func_name ID/name of function
 * @param args Structure of function arguments
 * @param returns Structure of function return variables
 */
void gen_function_start(char* func_name, Token_type returns);

/**
 * @brief Generates end of an user function
 */
void gen_function_end(ht_item_t symbol);

/**
 * @brief Generates input parameter for function
 * @param param_name Input parameter name
 * @param param_val Input parameter value
 */
void gen_function_input_param(char* param_name, Token param_val);

/**
 * @brief Generates call of an user function
 * @param func_name ID/name of function
 */
void gen_function_call(char* func_name);

/**
 * @brief Generates built-in function read
 * @param id The variable where data will be stored
 * @param type Type of incoming data
 */
void gen_read(char* id, char* type);

/**
 * @brief Generates built-in function write
 * @param var Variable or string to be printed
 */
void gen_write(Token var);

/**
 * @brief Generates start of a loop
 */
void gen_loop_start();

/**
 * @brief Generates end of a loop
 */
void gen_loop_end();

/**
 * @brief Generates start of IF
 */
void gen_if_start();

/**
 * @brief Generates else branch of IF
 */
void gen_if_else();

/**
 * @brief Generates end of IF
 */
void gen_if_end();

/**
 * @brief Generates instructions for conditions 
 * @param type Instruction type
 */
void gen_condition(Token_type type);

/**
 * @brief Idk, find workaround
 * I am very much aware this is not working (2022)
 * @param var Variable to substring
 * @param index_from Index from varible will be substringed
 * @param index_to Index where substring will end
 */
void gen_substring(Token var, Token index_from, Token index_to);

/**
 * @brief Generates built-in function for converting char to int
 * @param var String or variable with string
 * @param index Variable or integer with index of char
 */
void gen_ord(Token var, Token index);

/**
 * @brief Generates built-in function for getting one char from string
 * @param var Variable
 * @param index Index of char
 */
void gen_chr(Token var, Token index);

/**
 * @brief Generates built-in function for converting float/ number to integer
 * @param var Variable to be converted
 */
void gen_float2int(Token var);

/**
 * @brief Generates stack version of gen_float2int
 */
void gen_float2ints();

/**
 * @brief Generates built-in function for converting integer to float
 * @param var Variable to be converted
 */
void gen_int2float(Token var);

/**
 * @brief Generates stack version of gen_int2float
 */
void gen_int2floats();

/**
 * @brief Generates exit instruction
 */
void gen_exit();
#endif