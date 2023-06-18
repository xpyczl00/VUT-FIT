/**
* Project - Compiler for IFJ22
* 
* @brief Stack for IFJ21 - analysis of variable and function existance
* 
* @author Lukasz Pycz <xpyczl00>
* @file stack_table.c
*
**/

#include "stack_table.h"

bool Stack_check(TStack_table *stack, char *ID){

    S_element_table *new = stack->top;
    while (new!=NULL)
    {
        if(!strcmp(new->name,ID)) {
            return true;
        }
            new= new->next;
    }

    return false;



}

int Clear_frame(TStack_table *stack){
    while(!stack->top->is_delim){
        Stack_Pop_Table(stack);
    }
    if(stack->top->is_delim)
        Stack_Pop_Table(stack);
    else
        return ERROR_INTERN;
    return ERROR_OK;

}


void Stack_Init_Table(TStack_table *stack)
{
    stack->top = NULL;
}
int Stack_Push_Table(TStack_table *stack, char *ID)
{
    S_element_table *new = (S_element_table *)malloc(sizeof(S_element_table));
    if (new == NULL)
    {

        return ERROR_INTERN;
    }
    else
    {
        new->name = ID;
        new->next = stack->top;
        stack->top = new;
        // delim
    }
    return ERROR_INTERN;
}
void Stack_Pop_Table(TStack_table *stack)
{

    S_element_table *elem;
    if (stack->top != NULL)
    {
        elem = stack->top;
        stack->top = stack->top->next;
        free(elem);
    }
}

S_element_table *Stack_Top_Table(TStack_table *stack)
{

    return stack->top;
}
bool Stack_IsEmpty_Table(TStack_table *stack)
{

    return (stack->top == NULL);
}

void Stack_Dispose_Table(TStack_table *stack)
{
    while (stack->top != NULL)
    {
        S_element_table *del = stack->top;
        stack->top = stack->top->next;
        free(del);
        del = NULL;
    }
}
