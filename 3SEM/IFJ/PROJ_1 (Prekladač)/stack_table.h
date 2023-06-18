/**
* Project - Compiler for IFJ22
* 
* @brief Stack for IFJ21 - analysis of variable and function existance
* 
* @author Lukasz Pycz <xpyczl00>
* @file stack_table.h
*
**/

#include "scanner.h"
#include <stdbool.h>
#include <stdlib.h>


/**
 * @struct S_element_table
 * @brief Includes element data and pointer to the next element
 */
typedef struct s_elem_t
{
    char *name;
    bool is_delim;
    struct s_elem_t *next;
}S_element_table;


/**
 * @struct TStack_table
 * @brief Includes pointer to the top of the stack
 */
typedef struct
{
    S_element_table *top;
} TStack_table;



bool Stack_check(TStack_table *stack, char *ID);

int Clear_frame(TStack_table *stack);
/**
 * @brief Initializes stack
 * @param stack Structure
 */
void Stack_Init_Table(TStack_table *stack);


/**
 * @brief Pushes new data to stack
 * @param stack Structure
 * @return 0 if malloc failed, 1 if success
 */
int Stack_Push_Table(TStack_table *stack, char *ID);


/**
 * @brief Removes data from the top of the stack
 * @param stack Structure
 */
void Stack_Pop_Table(TStack_table *stack);

/**
 * @brief Returns data from the top of the stack
 * @param stack Structure
 */
S_element_table *Stack_Top_Table(TStack_table *stack);

/**
 * @brief Checks if stack is empty
 * @param stack Structure
 * @return true if stack is empty, false if stack is not empty
 */
bool Stack_IsEmpty_Table(TStack_table *stack);

/**
 * @brief Removes data from the stack
 * @param stack Structure
 */
void Stack_Dispose_Table(TStack_table *stack);