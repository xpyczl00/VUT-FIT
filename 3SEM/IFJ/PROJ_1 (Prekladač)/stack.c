/**
* Project - Compiler for IFJ22
* 
* @brief Stack for IFJ21 - syntax analysis of expressions
* 
* @author Marek Križan <xkriza08>
* @file stack.c
*
**/

#include "stack.h"


void Stack_Init(TStack *stack)
{
    stack->top = NULL;
}

int Stack_Push(TStack *stack, Token token, bool handle, bool terminal, Token_type return_type)
{
    TStack_element *new = (TStack_element *)malloc(sizeof(TStack_element));
    if (new == NULL)
    {
        return ERROR_INTERN;
    }
    else
    {   
        new->token = token;
        new->next = stack->top;
        new->handle = handle;
        new->terminal = terminal;
        new->return_type = return_type;
        stack->top = new;
    }
    return ERROR_OK;
}

void Stack_Pop(TStack *stack)
{
    TStack_element *elem;
    if (stack->top != NULL)
    {
        elem = stack->top;
        stack->top = stack->top->next;
        free(elem);
    }
}

TStack_element *Stack_Top(TStack *stack)
{
    return stack->top;
}

bool Stack_IsEmpty(TStack *stack)
{
    return (stack->top == NULL);
}

void Stack_Dispose(TStack *stack){
    while(stack->top != NULL){
        TStack_element *del = stack->top;
        stack->top = stack->top->next;
        free(del);
        del = NULL;
    }
}