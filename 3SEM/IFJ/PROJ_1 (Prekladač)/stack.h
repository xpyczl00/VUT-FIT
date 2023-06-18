/**
* Project - Compiler for IFJ22
* 
* @brief Stack for IFJ21 - syntax analysis of expressions
* 
* @author Marek Križan <xkriza08>
* @file stack.h
*
**/
#include "scanner.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * @struct TStack_element
 * @brief Includes element data and pointer to the next element
 */
typedef struct s_elem
{
    Token token;
    bool handle;
    bool terminal;
    struct s_elem *next;
    Token_type return_type;
} TStack_element;

/**
 * @struct TStack
 * @brief Includes pointer to the top of the stack
 */
typedef struct
{
    TStack_element *top;
} TStack;

/**
 * @brief Initializes stack
 * @param stack Structure
 */
void Stack_Init(TStack *stack);


/**
 * @brief Pushes new data to stack
 * @param stack Structure
 * @return 0 if malloc failed, 1 if success
 */
int Stack_Push(TStack *stack, Token token, bool handle, bool terminal, Token_type return_type);


/**
 * @brief Removes data from the top of the stack
 * @param stack Structure
 */
void Stack_Pop(TStack *stack);

/**
 * @brief Returns data from the top of the stack
 * @param stack Structure
 */
TStack_element *Stack_Top(TStack *stack);

/**
 * @brief Checks if stack is empty
 * @param stack Structure
 * @return true if stack is empty, false if stack is not empty
 */
bool Stack_IsEmpty(TStack *stack);

/**
 * @brief Removes data from the stack
 * @param stack Structure
 */
void Stack_Dispose(TStack *stack);