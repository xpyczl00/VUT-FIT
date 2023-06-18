/**
 * Project - Compiler for IFJ22
 *
 * @brief Syntax analysis of expressions
 *
 * @author Marek Krizan <xkriza08>
 * @author Matej Hložek <xhloze02>
 * @file expressions.h
 *
 **/

#ifndef IFJ_EXPRESSIONS_H
#define IFJ_EXPRESSIONS_H

#include "scanner.h"
#include "stack.h"
#include "error_codes.h"
#include "code_gen.h"
#include "symtable.h"
#include "stack_table.h"

/**
 * @brief Initialization and start of the expresion analysis
 * @param token Structure Token
 * @param func_table Table containing defined functions
 * @param var_table Table containing defined variables
 * @param var_stack Stack to check variable frame
 * @param func_stack Stack to check function frame
 * @param var Variable where result should be stored
 * @return Error code from error_codes.h
 */
int expression_analysis(Token *token, ht_table_t *func_table, ht_table_t *var_table, TStack_table *var_stack, ht_item_t *var);

/**
 * @brief Analysis of function call
 * @param token Structure Token
 * @param func_table Table containing defined functions
 * @param var Variable where result should be stored
 * @param var_table Table containing defined variables
 * @param var_stack Stack to check variable frame
 * @return Error code from error_codes.h
 */
int exp_function_analysis(Token *token, ht_table_t *func_table, ht_item_t *var, ht_table_t *var_table, TStack_table *var_stack);

/**
 * @brief Bottom-up parsing
 * @param token Structure Token
 * @param var_table Table containing defined variables
 * @param var_stack Stack to check variable frame
 * @return Error code from error_codes.h
 */
int analysis(Token *token, ht_table_t *var_table, TStack_table *var_stack);

/**
 * @brief Decides what is the next action based on given rule
 * @param rule Rule from precedence table
 * @param current_token Structure Token
 * @param var_table Table containing defined variables
 * @param var_stack Stack to check variable frame
 * @return Error code from error_codes.h
 */
int apply_rule(char rule, Token *current_token, ht_table_t *var_table, TStack_table *var_stack);

/**
 * @brief Reduces terminals into expressions
 * @param var_table Table containing defined variables
 * @param var_stack Stack to check variable frame
 * @return Error code from error_codes.h
 */
int rule_gt(ht_table_t *var_table, TStack_table *var_stack);

/**
 * @brief check if operands have the right type required by operator and convert them if they don't match
 * @param left_op lef operand
 * @param right_op right operand
 * @param operator  operator
 * @return Error code from error_codes.h
 */
int check_types(TStack_element left_op, TStack_element right_op, TStack_element *operator);

/**
 * @brief Find rule based on stack top and current token
 * @param stack_top Top of expression stack (Structure TStack_element)
 * @param current_token Structure Token
 * @param rule Pointer to return found rule
 * @return Error code from error_codes.h
 */
void find_rule(TStack_element *stack_top, Token *current_token, char *rule);

/**
 * @brief Reduction of expression with binary operator
 * @return Error code from error_codes.h
 */
int reduce_bin_op();

/**
 * @brief Reduction of expression inside parentheses
 * @return Error code from error_codes.h
 */
int reduce_parent();

#endif // IFJ_EXPRESSIONS_H
