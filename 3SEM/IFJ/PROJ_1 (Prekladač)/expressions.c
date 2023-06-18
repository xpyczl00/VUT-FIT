/**
 * Project - Compiler for IFJ22
 *
 * @brief Syntax analysis of expressions
 *
 * @author Marek Krizan <xkriza08>
 * @author Matej Hložek <xhloze02>
 * @file expressions.c
 *
 **/

#include "expressions.h"

const char table[8][8] = {
    // 0*/  1+-.  2<=>  3===!==  4(  5)  6i  7$
    {'>', '>', '>', '>', '<', '>', '<', '>'},
    {'<', '>', '>', '>', '<', '>', '<', '>'},
    {'<', '<', '>', '>', '<', '>', '<', '>'},
    {'<', '<', '<', '>', '<', '>', '<', '>'},

    {'<', '<', '<', '<', '<', '=', '<', 'e'},
    {'>', '>', '>', '>', 'e', '>', 'e', '>'},

    {'>', '>', '>', '>', 'e', '>', 'e', '>'},
    {'<', '<', '<', '<', '<', 'e', '<', 'e'},
};

TStack expression_stack;

void find_rule(TStack_element *stack_top, Token *current_token, char *rule)
{
    int i1, i2;

    // get terminal from top of the stack
    Token_type tokenType = stack_top->token.type;
    if (!stack_top->terminal)
    {
        tokenType = stack_top->next->token.type;
    }

    switch (tokenType)
    {
    case MULTIPLY:
    case DIVIDE:
        i1 = 0;
        break;

    case PLUS:
    case MINUS:
    case CONCAT:
        i1 = 1;
        break;

    case LESS:
    case LESS_OR_EQUALS:
    case MORE:
    case MORE_OR_EQUALS:
        i1 = 2;
        break;

    case EQUALS:
    case NOT_EQUALS:
        i1 = 3;
        break;

    case LEFT_PARENTHESIS:
        i1 = 4;
        break;

    case RIGHT_PARENTHESIS:
        i1 = 5;
        break;

    case VAR_ID:
    case FUNCTION_ID:
    case INTEGER:
    case FLOAT:
    case STRING:
        i1 = 6;
        break;

    case KEYWORD:
        if (stack_top->token.value.keyword == KEYWORD_NULL)
        {
            i1 = 6;
            break;
        }
        else
        {
            *rule = 'e';
            return;
        }

    case DOLLAR:
        i1 = 7;
        break;

    default:
        *rule = 'e';
        return;
    }

    switch (current_token->type)
    {
    case MULTIPLY:
    case DIVIDE:
        i2 = 0;
        break;

    case PLUS:
    case MINUS:
    case CONCAT:
        i2 = 1;
        break;

    case LESS:
    case LESS_OR_EQUALS:
    case MORE:
    case MORE_OR_EQUALS:
        i2 = 2;
        break;

    case EQUALS:
    case NOT_EQUALS:
        i2 = 3;
        break;

    case LEFT_PARENTHESIS:
        i2 = 4;
        break;

    case RIGHT_PARENTHESIS:
        i2 = 5;
        break;

    case VAR_ID:
    case FUNCTION_ID:
    case INTEGER:
    case FLOAT:
    case STRING:
        i2 = 6;
        break;

    case KEYWORD:
        if (current_token->value.keyword == KEYWORD_NULL)
        {
            i2 = 6;
            break;
        }
        else
        {
            *rule = 'e';
            return;
        }

    default:
        *rule = 'e';
        return;
    }

    *rule = table[i1][i2];
}

int reduce_parent()
{
    // get first three elements from stack and check if they are ( E )
    Stack_Pop(&expression_stack);
    if (Stack_IsEmpty(&expression_stack))
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    TStack_element *expression_ptr = Stack_Top(&expression_stack);
    TStack_element expression = *expression_ptr;
    if (expression.terminal)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    Stack_Pop(&expression_stack);
    if (Stack_IsEmpty(&expression_stack))
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    if (expression.next->token.type != LEFT_PARENTHESIS)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    Stack_Pop(&expression_stack);
    if (Stack_IsEmpty(&expression_stack))
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    // push reduced expression to stack
    return Stack_Push(&expression_stack, expression.token, false, false, expression.return_type);
}

int check_types(TStack_element left_op, TStack_element right_op, TStack_element *operator)
{
    if (operator->token.type == PLUS || operator->token.type == MINUS || operator->token.type == MULTIPLY)
    {
        if(left_op.return_type == KEYWORD && left_op.token.value.keyword == KEYWORD_NULL)
        {
            left_op.token.type = INTEGER;
            left_op.return_type = INTEGER;
            left_op.token.value.integer_value = 0;
        }
        if(right_op.return_type == KEYWORD && right_op.token.value.keyword == KEYWORD_NULL)
        {
            right_op.token.type = INTEGER;
            right_op.return_type = INTEGER;
            right_op.token.value.integer_value = 0;
        }
        if ((left_op.return_type == FLOAT && right_op.return_type == FLOAT) || (left_op.return_type == INTEGER && right_op.return_type == FLOAT) || (left_op.return_type == FLOAT && right_op.return_type == INTEGER))
        {
            gen_push(left_op.token);
            if (left_op.return_type == INTEGER)
            {
                gen_int2floats();
            }
            gen_push(right_op.token);
            if (right_op.return_type == INTEGER)
            {
                gen_int2floats();
            }
            operator->return_type = FLOAT;
            gen_instruction(operator->token);
            return ERROR_OK;
        }
        else if(left_op.return_type == INTEGER && right_op.return_type == INTEGER)
        {
            gen_push(left_op.token);
            gen_push(right_op.token);
            operator->return_type = INTEGER;
            gen_instruction(operator->token);
            return ERROR_OK;
        }
        else
        {
            return ERROR_SEMANTIC_TYPE_COMP;
        }
    }
    else if (operator->token.type == DIVIDE)
    {
        if(left_op.return_type == KEYWORD && left_op.token.value.keyword == KEYWORD_NULL)
        {
            left_op.token.type = INTEGER;
            left_op.return_type = INTEGER;
            left_op.token.value.integer_value = 0;
        }
        if(right_op.return_type == KEYWORD && right_op.token.value.keyword == KEYWORD_NULL)
        {
            right_op.token.type = INTEGER;
            right_op.return_type = INTEGER;
            right_op.token.value.integer_value = 0;
        }
        if ((left_op.return_type == INTEGER || left_op.return_type == FLOAT ) && (right_op.return_type == INTEGER || right_op.return_type == FLOAT ))
        {
            gen_push(left_op.token);
            if (left_op.return_type == INTEGER)
            {
                gen_int2floats();
            }
            gen_push(right_op.token);
            if (right_op.return_type == INTEGER)
            {
                gen_int2floats();
            }
            operator->return_type = FLOAT;
            gen_instruction(operator->token);
            return ERROR_OK;
        }
        else
        {
            return ERROR_SEMANTIC_TYPE_COMP;
        }
    }
    else if (operator->token.type == CONCAT)
    {
        if(left_op.return_type == KEYWORD && left_op.token.value.keyword == KEYWORD_NULL)
        {
            left_op.token.type = STRING;
            left_op.return_type = STRING;
            left_op.token.value.string.string = "";
        }
        if(right_op.return_type == KEYWORD && right_op.token.value.keyword == KEYWORD_NULL)
        {
            right_op.token.type = STRING;
            right_op.return_type = STRING;
            right_op.token.value.string.string = "";
        }
        gen_push(left_op.token);
        gen_push(right_op.token);
        operator->return_type = STRING;
        gen_instruction(operator->token);
        return ERROR_OK;
    }
    else if (operator->token.type == EQUALS || operator->token.type == NOT_EQUALS)
    {
        if (left_op.return_type != right_op.return_type)
        {
            gen_push_false();
        }
        else
        {
            gen_push(left_op.token);
            gen_push(right_op.token);
            gen_condition(operator->token.type);
        }
        operator->return_type = BOOL;
        return ERROR_OK;
    }
    else if (operator->token.type == LESS || operator->token.type == LESS_OR_EQUALS || operator->token.type == MORE || operator->token.type == MORE_OR_EQUALS)
    {
        gen_push(left_op.token);
        gen_push(right_op.token);
        gen_condition(operator->token.type);
        operator->return_type = BOOL;
        return ERROR_OK;
    }
    return ERROR_OK;
}

int reduce_bin_op()
{
    TStack_element left_op;
    TStack_element operator;
    TStack_element right_op;

    // get first three elements from stack
    TStack_element *right_op_ptr = Stack_Top(&expression_stack);
    right_op = *right_op_ptr;
    Stack_Pop(&expression_stack);
    if (Stack_IsEmpty(&expression_stack))
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    TStack_element *operator_ptr = Stack_Top(&expression_stack);
    operator= * operator_ptr;
    Stack_Pop(&expression_stack);
    if (Stack_IsEmpty(&expression_stack))
    {
        return ERROR_SYNTAX_ANALYSIS;
    }
    TStack_element *left_op_ptr = Stack_Top(&expression_stack);
    left_op = *left_op_ptr;
    Stack_Pop(&expression_stack);
    if (Stack_IsEmpty(&expression_stack))
    {
        return ERROR_SYNTAX_ANALYSIS;
    }

    // check if the top three elements from stack are E operator E
    if (left_op.terminal || right_op.terminal)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }

    // check type of operator and operands
    int err = check_types(left_op, right_op, &operator);
    if (err != ERROR_OK)
    {
        return err;
    }

    // push reduced expression to stack
    return Stack_Push(&expression_stack, operator.token, false, false, operator.return_type);
}

int rule_gt(ht_table_t *var_table, TStack_table *var_stack)
{
    int err = ERROR_OK;

    // get terminal from the top of the stack
    TStack_element *top_terminal = Stack_Top(&expression_stack);
    if (!top_terminal->terminal)
    {
        top_terminal = top_terminal->next;
    }

    // decide what rule to use to reduce
    if (top_terminal->handle && top_terminal->terminal)
    {
        Token_type tokenType = top_terminal->token.type;
        if (tokenType == INTEGER || tokenType == FLOAT || tokenType == STRING || tokenType == VAR_ID || (tokenType == KEYWORD && top_terminal->token.value.keyword == KEYWORD_NULL))
        {
            // change i on stack to E
            top_terminal->terminal = false;
            top_terminal->handle = false;
            if (top_terminal->return_type == VAR_ID)
            {
                if (!Stack_check(var_stack, top_terminal->token.value.string.string))
                {
                    return ERROR_SEMANTIC_UNDEF_VARIABLE;
                }
                ht_item_t *var = ht_search(var_table, top_terminal->token.value.string.string);
                top_terminal->return_type = var->param_type;
            }
        }
        else
        {
            err = reduce_bin_op();
        }
    }
    else if (top_terminal->token.type == RIGHT_PARENTHESIS)
    {
        err = reduce_parent();
    }
    else
    {
        err = ERROR_SYNTAX_ANALYSIS;
    }
    return err;
}

int apply_rule(char rule, Token *current_token, ht_table_t *var_table, TStack_table *var_stack)
{
    int err = ERROR_OK;
    switch (rule)
    {
    case '=':
        err = Stack_Push(&expression_stack, *current_token, false, true, current_token->type);
        return err;

    case '<':
        err = Stack_Push(&expression_stack, *current_token, true, true, current_token->type);
        return err;

    case '>':
        err = rule_gt(var_table, var_stack);
        return err;
    }
    return err;
}

int analysis(Token *token, ht_table_t *var_table, TStack_table *var_stack)
{

    int err = ERROR_OK;
    char rule;
    while (true)
    {
        find_rule(Stack_Top(&expression_stack), token, &rule);

        if (rule == 'e')
        {
            TStack_element *top = Stack_Top(&expression_stack);
            // reduce all remaining terminals into expressions
            if (top->token.type == DOLLAR)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
            while (top->next->token.type != DOLLAR)
            {
                err = rule_gt(var_table, var_stack);
                if (err != ERROR_OK)
                {
                    return err;
                }
                top = Stack_Top(&expression_stack);
            }
            // in case there is only $ and terminal in stack
            if (top->terminal)
            {
                err = rule_gt(var_table, var_stack);
                if (err != ERROR_OK)
                {
                    return err;
                }
            }
            top = Stack_Top(&expression_stack);
            if (top->token.type == INTEGER || top->token.type == FLOAT || top->token.type == STRING ||
                top->token.type == VAR_ID || (top->token.type == KEYWORD && top->token.value.keyword == KEYWORD_NULL))
            {
                gen_push(top->token);
            }
            return ERROR_OK;
        }

        err = apply_rule(rule, token, var_table, var_stack);
        if (err != ERROR_OK)
        {
            return err;
        }

        // we only want new token if previous rule was not '>'
        if (rule != '>')
        {
            err = get_token(token);
            if (err != ERROR_OK)
            {
                return err;
            }
        }
    }

    return err;
}

int exp_function_analysis(Token *token, ht_table_t *func_table, ht_item_t *var, ht_table_t *var_table, TStack_table *var_stack)
{
    ht_item_t *func = ht_search(func_table, token->value.string.string);

    // if next token is not '(' throw error
    int err = get_token(token);
    if (err != ERROR_OK)
    {
        return err;
    }
    if (token->type != LEFT_PARENTHESIS)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }

    // check number and type of parameters
    values_t *param = func->first_par;
    for (int i = 0; i < func->input_param_count; i++)
    {

        err = get_token(token);
        if (err != ERROR_OK)
        {
            return err;
        }

        // compare parameter type with record in hash table
        if (token->type == VAR_ID)
        {
            if (!Stack_check(var_stack, token->value.string.string))
            {
                return ERROR_SEMANTIC_UNDEF_VARIABLE;
            }
            ht_item_t *par = ht_search(var_table, token->value.string.string);
            if (par->param_type != param->item_type)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }
        }
        else if (token->type == INTEGER || token->type == FLOAT || token->type == STRING || (token->type == KEYWORD && token->value.keyword == KEYWORD_NULL))
        {
            if (token->type != param->item_type)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }
        }

        if(i == 0 && token->type == RIGHT_PARENTHESIS)
        {
            return ERROR_SEMANTIC_PARAMETERS;
        }

        gen_function_input_param(param->item_id, *token);

        err = get_token(token);
        if (err != ERROR_OK)
        {
            return err;
        }

        if (i == func->input_param_count - 1)
        {
            if (token->type != RIGHT_PARENTHESIS)
            {
                if (token->type == COMMA)
                {
                    err = get_token(token);
                    if (err != ERROR_OK)
                    {
                        return err;
                    }
                    if (token->type == VAR_ID || token->type == INTEGER || token->type == FLOAT || token->type == STRING || (token->type == KEYWORD && token->value.keyword == KEYWORD_NULL))
                    {
                        return ERROR_SEMANTIC_PARAMETERS;
                    }
                    else
                    {
                        return ERROR_SYNTAX_ANALYSIS;
                    }
                }
                else
                {
                    return ERROR_SYNTAX_ANALYSIS;
                }
            } // if we reached number of specified parameters check and ')' is not next token throw error
            break;
        }
        else
        {
            if (token->type != COMMA) {
                if (token->type == RIGHT_PARENTHESIS)
                {
                    return ERROR_SEMANTIC_PARAMETERS;
                }
                return ERROR_SYNTAX_ANALYSIS;
            }
        }
        param = param->next_item;
    }

    if(func->input_param_count == 0)
    {
        err = get_token(token);
        if(err != ERROR_OK)
        {
            return err;
        }
        if(token->type != RIGHT_PARENTHESIS)
        {
            if(token->type == VAR_ID || token->type == INTEGER || token->type == STRING || token->type == FLOAT || (token->type == KEYWORD && token->value.keyword == KEYWORD_NULL))
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }
            return ERROR_SYNTAX_ANALYSIS;
        }
    }

    gen_function_call(func->key);

    // generate pop to variable
    if (func->return_val_type != EMPTY_TOKEN)
    {
        if (var != NULL)
        {
            // store result in variable
            if ((ht_search(var_table, var->key) != NULL) && (var->return_val_type == EMPTY_TOKEN))
            {
                if (Stack_check(var_stack, var->key))
                {

                    var->param_type = Stack_Top(&expression_stack)->return_type;
                    gen_pops(*var);
                }
                else
                {
                    return ERROR_SEMANTIC_UNDEF_VARIABLE;
                }
            }
            // return at the end of function definition
            else
            {
                if ((func->return_val_type == var->return_val_type) || ((func->return_val_type == INTEGER) && (var->return_val_type == FLOAT)))
                {
                    gen_function_end(*var);
                }
                else
                {
                    return ERROR_SEMANTIC_TYPE_COMP;
                }
            }
        }
    }

    err = get_token(token);
    if (err != ERROR_OK)
    {
        return err;
    }
    return ERROR_OK;
}

int expression_analysis(Token *token, ht_table_t *func_table, ht_table_t *var_table, TStack_table *var_stack, ht_item_t *var)
{
    // if function found on input then analyze function
    if (token->type == FUNCTION_ID)
    {
        ht_item_t *item = ht_search(func_table, token->value.string.string);
        if (item != NULL)
        {
            return exp_function_analysis(token, func_table, var, var_table, var_stack);
        }
        return ERROR_SEMANTIC_FUNCTION;
    }

    // init of stack
    Stack_Init(&expression_stack);
 
    // add 'dollar' token on top
    Token dollar_token;
    dollar_token.type = DOLLAR;
    int err = Stack_Push(&expression_stack, dollar_token, false, true, dollar_token.type);
    if (err != ERROR_OK)
    {
        return ERROR_INTERN;
    }

    // do the analysis
    err = analysis(token, var_table, var_stack);

    if (err != ERROR_OK)
    {
        return err;
    }

    // generate pop to variable
    if (var != NULL)
    {
        // store result in variable
        if ((ht_search(var_table, var->key) != NULL) && (var->return_val_type == EMPTY_TOKEN))
        {
            if (Stack_check(var_stack, var->key))
            {
                var->param_type = Stack_Top(&expression_stack)->return_type;
                gen_pops(*var);
            }
            else
            {
                return ERROR_SEMANTIC_UNDEF_VARIABLE;
            }
        }
        // return at the end of function definition
        else
        {
            if ((Stack_Top(&expression_stack)->return_type == var->return_val_type) || ((Stack_Top(&expression_stack)->return_type == INTEGER) && (var->return_val_type == FLOAT)))
            {
                gen_function_end(*var);
            }
            else
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }
        }
    }

    // clear stack
    Stack_Dispose(&expression_stack);

    return err;
}
