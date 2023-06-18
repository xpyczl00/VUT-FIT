/**
 * Project - Compiler for IFJ22
 *
 * @brief Parser for IFJ22 - Syntax analysis
 *
 * @author Matej Koreň <xkoren10
 * @author Lukasz Pycz <xpyczl00>
 * @file parser.c
 *
 **/

#include "parser.h"

Token actToken;

Dyn_string token_1;
int input_counter = 0;
bool is_return;
bool is_program_body;
bool if_but_not_else = false;

ht_table_t variable_table[MAX_HT_SIZE];
ht_table_t function_table[MAX_HT_SIZE];

TStack_table program_body_stack;
TStack_table function_def_stack;

char *current_function_name;
char *current_var_name;

int Prolog_check()
{
    int ret;

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != PROLOG_PHP)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != FUNCTION_ID || dyn_string_compare(actToken.value.string, "declare") != 0)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != LEFT_PARENTHESIS)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != FUNCTION_ID || dyn_string_compare(actToken.value.string, "strict_types") != 0)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != ASSIGN)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != INTEGER && actToken.value.integer_value == 1)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != RIGHT_PARENTHESIS)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != SEMICOLON)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    return ERROR_OK;
}

int Define_function()
{
    int ret;

    Stack_Init_Table(&function_def_stack);

    if ((ret = Function_header()) != 0)
    {
        return ret;
    };

    ht_item_t *current = ht_search(function_table, current_function_name);

    gen_function_start(current_function_name, current->return_val_type);

    if ((ret = Command_sequence()) != 0)
    {
        return ret;
    };

    // missing return
    if (current->return_val_type != EMPTY_TOKEN && is_return == false)
    {

        return ERROR_SEMANTIC_PARAMETERS;
    }
    // if in program body
    if (current->return_val_type == EMPTY_TOKEN && is_return == true)
    {

        gen_function_end(*(ht_search(function_table, current_function_name)));
    }
    if (current->return_val_type == EMPTY_TOKEN && is_return == false)
    {
        gen_function_end(*(ht_search(function_table, current_function_name)));
    }

    current_function_name = NULL;
    Clear_frame(&function_def_stack);

    return ERROR_OK;
}

int Function_header()
{
    int ret;

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != FUNCTION_ID)
    {
        if (29 < actToken.type && actToken.type < 41)
            return ERROR_SEMANTIC_FUNCTION;

        return ERROR_SYNTAX_ANALYSIS;
    };

    // Check if its not function redefinition
    if (ht_search(function_table, actToken.value.string.string) != NULL)
    {
        // redefinition
        return ERROR_SEMANTIC_FUNCTION;
    }
    else
    {

        // add to symtable
        current_function_name = actToken.value.string.string;

        ht_item_t new_item;
        set_item_null(&new_item);
        new_item.key = actToken.value.string.string;
        ht_insert(function_table, &new_item);
    }

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != LEFT_PARENTHESIS)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    // delim for variable stack
    Stack_Push_Table(&function_def_stack, "-");
    function_def_stack.top->is_delim = true;

    if ((ret = List_of_parameters()) != 0)
    {
        return ret;
    };

    input_counter = 0;

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != COLON)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.value.keyword != KEYWORD_FLOAT && actToken.value.keyword != KEYWORD_INT && actToken.value.keyword != KEYWORD_STRING && actToken.value.keyword != KEYWORD_VOID)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    ht_item_t *clan = ht_search(function_table, current_function_name);
    switch (actToken.value.keyword)
    {
    case KEYWORD_INT:

        clan->return_val_type = INTEGER;

        break;
    case KEYWORD_STRING:

        clan->return_val_type = STRING;

        break;
    case KEYWORD_FLOAT:

        clan->return_val_type = FLOAT;
        break;
    case KEYWORD_VOID:
        clan->return_val_type = EMPTY_TOKEN;
        break;

    default:
        return ERROR_SYNTAX_ANALYSIS;
    }
    return ERROR_OK;
}

int Command_sequence()
{
    int ret;

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != LEFT_CURLY)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    // loop for command sequence
    do
    {

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type == RIGHT_CURLY)
            return ERROR_OK;

        if ((ret = Command()) != ERROR_OK && ret != 69)
        {
            return ret;
        }

        if (ret != 69)
        {
            if (actToken.type != SEMICOLON)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
        }

    } while (actToken.type != RIGHT_CURLY);

    return ERROR_OK;
}

int Command()
{
    int ret;

    ht_item_t *current = ht_search(function_table, current_function_name);

    // assign command x = Expression alebo x = funkcia
    if (actToken.type == VAR_ID)
    {

        if (is_program_body)
        {

            if (!Stack_check(&program_body_stack, actToken.value.string.string))
            {
                Stack_Push_Table(&program_body_stack, actToken.value.string.string);
                program_body_stack.top->is_delim = false;
                ht_item_t new_item;
                set_item_null(&new_item);
                new_item.key = actToken.value.string.string;
                new_item.return_val_type = EMPTY_TOKEN;
                ht_insert(variable_table, &new_item);
            }
        }
        else
        {
            if (!Stack_check(&function_def_stack, actToken.value.string.string))
            {
                Stack_Push_Table(&function_def_stack, actToken.value.string.string);
                function_def_stack.top->is_delim = false;
                ht_item_t new_item;
                set_item_null(&new_item);
                new_item.key = actToken.value.string.string;
                new_item.return_val_type = EMPTY_TOKEN;
                ht_insert(variable_table, &new_item);
            }
        }
        // check if variable exists

        current_var_name = actToken.value.string.string;
        gen_var_def(current_var_name);

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != ASSIGN)
        {
            return ERROR_SYNTAX_ANALYSIS;
        };

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type == BUILT_IN_CHR)
        {
            if (ht_search(variable_table, current_var_name) != NULL)
                ht_search(variable_table, current_var_name)->param_type = STRING;
            else
            {
                ht_item_t tmp;
                set_item_null(&tmp);
                tmp.param_type = STRING;
            }
            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != LEFT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != VAR_ID)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            if (ht_search(variable_table, actToken.value.string.string) == NULL)
            {
                return ERROR_SEMANTIC_UNDEF_VARIABLE;
            }

            if (ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != RIGHT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
            ret = get_token(&actToken);
            if (ret != 0)
                return ret;
        }

        // built in funkcie
        else if (actToken.type == BUILT_IN_FLOATVAL)
        {
            if (ht_search(variable_table, current_var_name) != NULL)
                ht_search(variable_table, current_var_name)->param_type = FLOAT;
            else
            {
                ht_item_t tmp;
                set_item_null(&tmp);
                tmp.param_type = FLOAT;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != LEFT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type == RIGHT_PARENTHESIS)
            {
                return ERROR_OK;
            }
            else if (actToken.type == VAR_ID)
            {

                if (ht_search(variable_table, actToken.value.string.string) == NULL)
                {
                    return ERROR_SEMANTIC_UNDEF_VARIABLE;
                }

                if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING && ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER && ht_search(variable_table, actToken.value.string.string)->param_type != FLOAT)
                {
                    return ERROR_SEMANTIC_PARAMETERS;
                }

                // check if item exists and if is type int string or float
            }
            else if (actToken.type != INTEGER && actToken.type != FLOAT && actToken.type != STRING)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != RIGHT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
            ret = get_token(&actToken);
            if (ret != 0)
                return ret;
        }

        // built in funkctions
        else if (actToken.type == BUILT_IN_INTVAL)
        {

            if (ht_search(variable_table, current_var_name) != NULL)
                ht_search(variable_table, current_var_name)->param_type = INTEGER;
            else
            {
                ht_item_t tmp;
                set_item_null(&tmp);
                tmp.param_type = INTEGER;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != LEFT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type == RIGHT_PARENTHESIS)
            {
                return ERROR_OK;
            }
            else if (actToken.type == VAR_ID)
            {

                if (ht_search(variable_table, actToken.value.string.string) == NULL)
                {
                    return ERROR_SEMANTIC_UNDEF_VARIABLE;
                }

                if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING && ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER && ht_search(variable_table, actToken.value.string.string)->param_type != FLOAT)
                {
                    return ERROR_SEMANTIC_PARAMETERS;
                }

                // check if var exists and if is type int string or float
            }
            else if (actToken.type != INTEGER && actToken.type != FLOAT && actToken.type != STRING)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != RIGHT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
            ret = get_token(&actToken);
            if (ret != 0)
                return ret;
        }

        // built in funkcie
        else if (actToken.type == BUILT_IN_ORD)
        {

            if (ht_search(variable_table, current_var_name) != NULL)
                ht_search(variable_table, current_var_name)->param_type = INTEGER;
            else
            {
                ht_item_t tmp;
                set_item_null(&tmp);
                tmp.param_type = INTEGER;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != LEFT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != VAR_ID)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            if (ht_search(variable_table, actToken.value.string.string) == NULL)
            {
                return ERROR_SEMANTIC_UNDEF_VARIABLE;
            }

            if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != RIGHT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
            ret = get_token(&actToken);
            if (ret != 0)
                return ret;
        }

        // built in funkcie
        else if (actToken.type == BUILT_IN_READF)
        {

            if (ht_search(variable_table, current_var_name) != NULL)
                ht_search(variable_table, current_var_name)->param_type = FLOAT;
            else
            {
                ht_item_t tmp;
                set_item_null(&tmp);
                tmp.param_type = FLOAT;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != LEFT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != RIGHT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            gen_read(current_var_name, "float");
            ret = get_token(&actToken);
            if (ret != 0)
                return ret;
        }

        // built in funkcie
        else if (actToken.type == BUILT_IN_READI)
        {

            if (ht_search(variable_table, current_var_name) != NULL)
                ht_search(variable_table, current_var_name)->param_type = INTEGER;
            else
            {
                ht_item_t tmp;
                set_item_null(&tmp);
                tmp.param_type = INTEGER;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != LEFT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != RIGHT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            gen_read(current_var_name, "int");
            ret = get_token(&actToken);
            if (ret != 0)
                return ret;
        }

        // built in funkcie
        else if (actToken.type == BUILT_IN_READS)
        {

            if (ht_search(variable_table, current_var_name) != NULL)
                ht_search(variable_table, current_var_name)->param_type = STRING;
            else
            {
                ht_item_t tmp;
                set_item_null(&tmp);
                tmp.param_type = STRING;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != LEFT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != RIGHT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            gen_read(current_var_name, "string");
            ret = get_token(&actToken);
            if (ret != 0)
                return ret;
        }

        // built in funkcie
        else if (actToken.type == BUILT_IN_STRLEN)
        {

            if (ht_search(variable_table, current_var_name) != NULL)
                ht_search(variable_table, current_var_name)->param_type = INTEGER;
            else
            {
                ht_item_t tmp;
                set_item_null(&tmp);
                tmp.param_type = INTEGER;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != LEFT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type == RIGHT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
            else if (actToken.type == VAR_ID)
            {

                if (ht_search(variable_table, actToken.value.string.string) == NULL)
                {
                    return ERROR_SEMANTIC_UNDEF_VARIABLE;
                }

                if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING)
                {
                    return ERROR_SEMANTIC_PARAMETERS;
                }

                // check if exists and if is type int string or float
            }
            else if (actToken.type != STRING)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }

            gen_push(actToken);

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != RIGHT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            gen_strlen(current_var_name);
            ret = get_token(&actToken);
            if (ret != 0)
                return ret;
        }

        else if (actToken.type == BUILT_IN_STRVAL)
        {

            if (ht_search(variable_table, current_var_name) != NULL)
                ht_search(variable_table, current_var_name)->param_type = STRING;
            else
            {
                ht_item_t tmp;
                set_item_null(&tmp);
                tmp.param_type = STRING;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != LEFT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type == RIGHT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
            else if (actToken.type == VAR_ID)
            {

                if (ht_search(variable_table, actToken.value.string.string) == NULL)
                {
                    return ERROR_SEMANTIC_UNDEF_VARIABLE;
                }

                if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING && ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER && ht_search(variable_table, actToken.value.string.string)->param_type != FLOAT)
                {
                    return ERROR_SEMANTIC_PARAMETERS;
                }

                // check if exists and if is type int string or float
            }
            else if (actToken.type != INTEGER && actToken.type != FLOAT && actToken.type != STRING)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != RIGHT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
            ret = get_token(&actToken);
            if (ret != 0)
                return ret;
        }

        else if (actToken.type == BUILT_IN_SUBSTRING)
        {

            if (ht_search(variable_table, current_var_name) != NULL)
                ht_search(variable_table, current_var_name)->param_type = STRING;
            else
            {
                ht_item_t tmp;
                set_item_null(&tmp);
                tmp.param_type = STRING;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != LEFT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != VAR_ID)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            if (ht_search(variable_table, actToken.value.string.string) == NULL)
            {
                return ERROR_SEMANTIC_UNDEF_VARIABLE;
            }

            if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != VAR_ID)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            if (ht_search(variable_table, actToken.value.string.string) == NULL)
            {
                return ERROR_SEMANTIC_UNDEF_VARIABLE;
            }

            if (ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != VAR_ID)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            if (ht_search(variable_table, actToken.value.string.string) == NULL)
            {
                return ERROR_SEMANTIC_UNDEF_VARIABLE;
            }

            if (ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != RIGHT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }
            ret = get_token(&actToken);
            if (ret != 0)
                return ret;
        }

        else if (actToken.type == BUILT_IN_WRITE)
        {
            if (ht_search(variable_table, current_var_name) != NULL)
                ht_search(variable_table, current_var_name)->param_type = EMPTY_TOKEN;
            else
            {
                ht_item_t tmp;
                set_item_null(&tmp);
                tmp.param_type = EMPTY_TOKEN;
            }
            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != LEFT_PARENTHESIS)
            {
                return ERROR_SYNTAX_ANALYSIS;
            }

            do
            {

                ret = get_token(&actToken);
                if (ret != 0)
                    return ret;

                if (actToken.type == RIGHT_PARENTHESIS)
                {
                    return ERROR_OK;
                }
                else if (actToken.type == COMMA)
                {
                    continue;
                }
                else if (actToken.type == VAR_ID)
                {

                    if (ht_search(variable_table, actToken.value.string.string) == NULL)
                    {
                        return ERROR_SEMANTIC_UNDEF_VARIABLE;
                    }

                    if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING && ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER && ht_search(variable_table, actToken.value.string.string)->param_type != FLOAT)
                    {
                        return ERROR_SEMANTIC_PARAMETERS;
                    }

                    // check if exists and if is type int string or float
                }
                else if (actToken.type == INTEGER || actToken.type == FLOAT || actToken.type == STRING)
                {
                    continue;
                }
                else
                {
                    return ERROR_SYNTAX_ANALYSIS;
                }
            } while (actToken.type != COMMA && actToken.type != RIGHT_PARENTHESIS);

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;
        }

        else
        {

            if (is_program_body)
                ret = expression_analysis(&actToken, function_table, variable_table, &program_body_stack, ht_search(variable_table, current_var_name));
            else
                ret = expression_analysis(&actToken, function_table, variable_table, &function_def_stack, ht_search(variable_table, current_var_name));
            return ret;
        }
    }

    // function call
    else if (actToken.type == FUNCTION_ID)
    {

        if (is_program_body)
            ret = expression_analysis(&actToken, function_table, variable_table, &program_body_stack, NULL);
        else
            ret = expression_analysis(&actToken, function_table, variable_table, &function_def_stack, NULL);

        if (ret != 0)
        {

            return ret;
        }

        return ERROR_OK;
    }
    // built in function ch
    else if (actToken.type == BUILT_IN_CHR)
    {
        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != LEFT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != VAR_ID)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        if (ht_search(variable_table, actToken.value.string.string) == NULL)
        {
            return ERROR_SEMANTIC_UNDEF_VARIABLE;
        }

        if (ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER)
        {
            return ERROR_SEMANTIC_PARAMETERS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != RIGHT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    }

    else if (actToken.type == BUILT_IN_FLOATVAL)
    {
        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != LEFT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type == RIGHT_PARENTHESIS)
        {
            return ERROR_OK;
        }
        else if (actToken.type == VAR_ID)
        {

            if (ht_search(variable_table, actToken.value.string.string) == NULL)
            {
                return ERROR_SEMANTIC_UNDEF_VARIABLE;
            }

            if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING && ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER && ht_search(variable_table, actToken.value.string.string)->param_type != FLOAT)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }

            // check if exists and if is type int string or float
        }
        else if (actToken.type != INTEGER && actToken.type != FLOAT && actToken.type != STRING)
        {
            return ERROR_SEMANTIC_PARAMETERS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != RIGHT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    }

    else if (actToken.type == BUILT_IN_INTVAL)
    {
        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != LEFT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type == RIGHT_PARENTHESIS)
        {
            return ERROR_OK;
        }
        else if (actToken.type == VAR_ID)
        {

            if (ht_search(variable_table, actToken.value.string.string) == NULL)
            {
                return ERROR_SEMANTIC_UNDEF_VARIABLE;
            }

            if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING && ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER && ht_search(variable_table, actToken.value.string.string)->param_type != FLOAT)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }

            // check if exists and if is type int string or float
        }
        else if (actToken.type != INTEGER && actToken.type != FLOAT && actToken.type != STRING)
        {
            return ERROR_SEMANTIC_PARAMETERS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != RIGHT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    }

    else if (actToken.type == BUILT_IN_ORD)
    {

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != LEFT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != VAR_ID)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        if (ht_search(variable_table, actToken.value.string.string) == NULL)
        {
            return ERROR_SEMANTIC_UNDEF_VARIABLE;
        }

        if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING)
        {
            return ERROR_SEMANTIC_PARAMETERS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != RIGHT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    }

    else if (actToken.type == BUILT_IN_READF)
    {

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != LEFT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != RIGHT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    }

    else if (actToken.type == BUILT_IN_READI)
    {

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != LEFT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != RIGHT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    }

    else if (actToken.type == BUILT_IN_READS)
    {

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != LEFT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != RIGHT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    }

    // built in functions
    else if (actToken.type == BUILT_IN_STRLEN)
    {
        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != LEFT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != VAR_ID)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        if (ht_search(variable_table, actToken.value.string.string) == NULL)
        {
            return ERROR_SEMANTIC_UNDEF_VARIABLE;
        }

        if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING)
        {
            return ERROR_SEMANTIC_PARAMETERS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != RIGHT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    }

    
    else if (actToken.type == BUILT_IN_STRVAL)
    {
        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != LEFT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type == RIGHT_PARENTHESIS)
        {
            return ERROR_OK;
        }
        else if (actToken.type == VAR_ID)
        {

            if (ht_search(variable_table, actToken.value.string.string) == NULL)
            {
                return ERROR_SEMANTIC_UNDEF_VARIABLE;
            }

            if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING && ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER && ht_search(variable_table, actToken.value.string.string)->param_type != FLOAT)
            {
                return ERROR_SEMANTIC_PARAMETERS;
            }

            // check if exists and if is type int string or float
        }
        else if (actToken.type != INTEGER && actToken.type != FLOAT && actToken.type != STRING)
        {
            return ERROR_SEMANTIC_PARAMETERS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != RIGHT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    }

    
    else if (actToken.type == BUILT_IN_SUBSTRING)
    {

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != LEFT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != VAR_ID)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        if (ht_search(variable_table, actToken.value.string.string) == NULL)
        {
            return ERROR_SEMANTIC_UNDEF_VARIABLE;
        }

        if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING)
        {
            return ERROR_SEMANTIC_PARAMETERS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != VAR_ID)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        if (ht_search(variable_table, actToken.value.string.string) == NULL)
        {
            return ERROR_SEMANTIC_UNDEF_VARIABLE;
        }

        if (ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER)
        {
            return ERROR_SEMANTIC_PARAMETERS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != VAR_ID)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        if (ht_search(variable_table, actToken.value.string.string) == NULL)
        {
            return ERROR_SEMANTIC_UNDEF_VARIABLE;
        }

        if (ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER)
        {
            return ERROR_SEMANTIC_PARAMETERS;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != RIGHT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    }

    // TODO
    else if (actToken.type == BUILT_IN_WRITE)
    {
        int comma_cnt = 0;
        int arg_count = 0;

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != LEFT_PARENTHESIS)
        {
            return ERROR_SYNTAX_ANALYSIS;
        }

        do
        {

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type == RIGHT_PARENTHESIS)
            {
                if (arg_count > 0)
                {
                    if (comma_cnt + 1 > arg_count)
                    {
                        return ERROR_SYNTAX_ANALYSIS;
                    }
                }

                ret = get_token(&actToken);
                if (ret != 0)
                    return ret;
                return ERROR_OK;
            }
            else if (actToken.type == COMMA)
            {
                comma_cnt++;
                continue;
            }
            else if (actToken.type == VAR_ID)
            {
                arg_count++;

                if (ht_search(variable_table, actToken.value.string.string) == NULL)
                {
                    return ERROR_SEMANTIC_UNDEF_VARIABLE;
                }

                if (ht_search(variable_table, actToken.value.string.string)->param_type != STRING && ht_search(variable_table, actToken.value.string.string)->param_type != INTEGER && ht_search(variable_table, actToken.value.string.string)->param_type != FLOAT)
                {
                    return ERROR_SEMANTIC_PARAMETERS;
                }

                // check if exists and if is type int string or float
                gen_write(actToken);
            }
            else if (actToken.type == INTEGER || actToken.type == FLOAT || actToken.type == STRING)
            {
                arg_count++;

                gen_write(actToken);
            }
            else
            {

                return ERROR_SYNTAX_ANALYSIS;
            }
        } while (actToken.type != RIGHT_PARENTHESIS);
    }

    // Command if
    else if (actToken.value.keyword == KEYWORD_IF)
    {

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if ((ret = Command_IF()) != 0)
        {
            return ret;
        };
    }

    // Command while
    else if (actToken.value.keyword == KEYWORD_WHILE)
    {

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if ((ret = Command_WHILE()) != 0)
        {
            return ret;
        };
    }

    else if (actToken.type == RIGHT_CURLY)
    {

        return ERROR_OK;
    }
    else if (!is_program_body && (actToken.value.keyword == KEYWORD_RETURN))
    {

        is_return = true;
        if (current->return_val_type == EMPTY_TOKEN)
        {

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != SEMICOLON)
                return ERROR_SEMANTIC_EXPRESSION;

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;

            if (actToken.type != RIGHT_CURLY)
            {
                return ERROR_SEMANTIC_EXPRESSION;
            }

            return 69;
        }

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (is_program_body)
        {
            if ((ret = expression_analysis(&actToken, function_table, variable_table, &function_def_stack, ht_search(function_table, current_function_name))) != 0)
            {
                return ret;
            };
        }
        else
        {
            if ((ret = expression_analysis(&actToken, function_table, variable_table, &function_def_stack, ht_search(function_table, current_function_name))) != 0)
            {
                return ret;
            };
        }

        gen_function_end(*current);

        return ERROR_OK;
    }

    else if (is_program_body && actToken.value.keyword == KEYWORD_RETURN)
    {
        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if (actToken.type != SEMICOLON)
        {

            // check expression after return
            if ((ret = expression_analysis(&actToken, function_table, variable_table, &program_body_stack, NULL)) != 0)
            {
                return ret;
            }
        }
        else
        {
            gen_push(actToken);
        }

        gen_exit();
    }

    // Prikaz Expression
    else
    {
        if (is_program_body)
        {
            if ((ret = expression_analysis(&actToken, function_table, variable_table, &program_body_stack, NULL)) != 0)
            {
                return ret;
            };
        }
        else
        {
            if ((ret = expression_analysis(&actToken, function_table, variable_table, &function_def_stack, NULL)) != 0)
            {
                return ret;
            };
        }
    }

    return ERROR_OK;
}

int Command_IF()
{
    int ret;
    if (actToken.type != LEFT_PARENTHESIS)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (is_program_body)
    {
        if ((ret = expression_analysis(&actToken, function_table, variable_table, &program_body_stack, NULL)) != 0)
        {
            return ret;
        };
    }
    else
    {
        if ((ret = expression_analysis(&actToken, function_table, variable_table, &function_def_stack, NULL)) != 0)
        {
            return ret;
        };
    }

    gen_if_start();

    if (actToken.type != RIGHT_PARENTHESIS)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }

    if (is_program_body)
    {
        Stack_Push_Table(&program_body_stack, "-");
        program_body_stack.top->is_delim = true;
    }
    else
    {
        Stack_Push_Table(&function_def_stack, "-");
        function_def_stack.top->is_delim = true;
    }

    if ((ret = Command_sequence()) != 0)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };
    if (is_program_body)
        Clear_frame(&program_body_stack);
    else
        Clear_frame(&function_def_stack);

    if (actToken.type != RIGHT_CURLY)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    gen_if_else();
    if (actToken.value.keyword == KEYWORD_ELSE)
    {
        if (is_program_body)
        {
            Stack_Push_Table(&program_body_stack, "-");
            program_body_stack.top->is_delim = true;
        }
        else
        {
            Stack_Push_Table(&function_def_stack, "-");
            function_def_stack.top->is_delim = true;
        }
        if ((ret = Command_sequence()) != 0)
        {
            return ret;
        };
    }
    else
    {
        if_but_not_else = true;
    }
    gen_if_end();

    return 69;
}

int Command_WHILE()
{
    int ret;
    if (actToken.type != LEFT_PARENTHESIS)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (is_program_body)
    {
        if ((ret = expression_analysis(&actToken, function_table, variable_table, &program_body_stack, NULL)) != 0)
        {
            return ret;
        };
    }
    else
    {
        if ((ret = expression_analysis(&actToken, function_table, variable_table, &function_def_stack, NULL)) != 0)
        {
            return ret;
        };
    }
    gen_loop_start();
    if (actToken.type != RIGHT_PARENTHESIS)
    {
        return ERROR_SYNTAX_ANALYSIS;
    }

    if (is_program_body)
    {
        Stack_Push_Table(&program_body_stack, "-");
        program_body_stack.top->is_delim = true;
    }
    else
    {
        Stack_Push_Table(&function_def_stack, "-");
        function_def_stack.top->is_delim = true;
    }

    if ((ret = Command_sequence()) != 0)
    {
        return ret;
    };

    gen_loop_end();

    if (is_program_body)
        Clear_frame(&program_body_stack);
    else
        Clear_frame(&function_def_stack);

    return 69;
}

int List_of_parameters()
{
    int ret;
    do
    {

        ret = get_token(&actToken);
        if (ret != 0)
            return ret;

        if ((ret = Parameter_definition()) != 0)
        {
            return ret;
        };

    } while (actToken.type != RIGHT_PARENTHESIS);

    return ERROR_OK;
}

int Parameter_definition()
{
    int ret;

    if (actToken.type == RIGHT_PARENTHESIS)
    {

        ht_item_t *empty_func = ht_search(function_table, current_function_name);

        empty_func->input_param_count = 0;
        empty_func->first_par = NULL;
        empty_func->content = NULL;

        return ERROR_OK;
    }

    if (actToken.value.keyword != KEYWORD_STRING && actToken.value.keyword != KEYWORD_INT && actToken.value.keyword != KEYWORD_FLOAT)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    int current_type = actToken.value.keyword;

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != VAR_ID)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    current_var_name = actToken.value.string.string;

    // adding var to function table
    if ((ht_search(variable_table, current_var_name)) != NULL)
    {
        // redefinition
        return ERROR_SEMANTIC_EXPRESSION;
    }
    else
    {

        // add to symtable
        ht_item_t new_item;
        set_item_null(&new_item);
        new_item.key = current_var_name;
        switch (current_type)
        {
        case KEYWORD_STRING:
            new_item.param_type = STRING;
            break;
        case KEYWORD_FLOAT:
            new_item.param_type = FLOAT;
            break;
        case KEYWORD_INT:
            new_item.param_type = INTEGER;
            break;

        default:
            return ERROR_SEMANTIC_OTHER;
        }

        // adding to table
        new_item.return_val_type = EMPTY_TOKEN;

        ht_insert(variable_table, &new_item);

        Stack_Push_Table(&function_def_stack, current_var_name);

        function_def_stack.top->is_delim = false;
    }
    // adding param to function table

    ht_item_t *new = ht_search(function_table, current_function_name);

    input_counter++;
    new->input_param_count = input_counter;

    values_t *new_par = malloc(sizeof(values_t));
    new_par->item_id = current_var_name;

    switch (current_type)
    {
    case KEYWORD_STRING:
        new_par->item_type = STRING;
        break;
    case KEYWORD_FLOAT:
        new_par->item_type = FLOAT;
        break;
    case KEYWORD_INT:
        new_par->item_type = INTEGER;
        break;
    }

    if (new->input_param_count == 1)
    {
        new->first_par = new_par;
        new->content = new_par;
        new->content->next_item = NULL;
        new->first_par->next_item = NULL;
    }
    else
    {
        new->content->next_item = new_par;
        new->content = new->content->next_item;
    }

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != COMMA && actToken.type != RIGHT_PARENTHESIS)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    return ERROR_OK;
}

int Epilog()
{
    int ret;
    if (actToken.type != EPILOG)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    ret = get_token(&actToken);
    if (ret != 0)
        return ret;

    if (actToken.type != END_of_FILE)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };

    return ERROR_OK;
}

int Program()
{
    int ret;
    is_return = false;

    // Initialization of tables and stacks for function and variables
    ht_init(variable_table);
    ht_init(function_table);
    Stack_Init_Table(&function_def_stack);
    Stack_Init_Table(&program_body_stack);

    Stack_Push_Table(&function_def_stack, "-");
    function_def_stack.top->is_delim = true;
    Stack_Push_Table(&program_body_stack, "-");
    program_body_stack.top->is_delim = true;

    if ((ret = Prolog_check()) != 0)
    {
        return ret;
    };

    // generating prolog header
    gen_header();

    // main loop for function definitions and program body

    do
    {

        if (!if_but_not_else)
        {

            ret = get_token(&actToken);
            if (ret != 0)
                return ret;
        }
        else
        {
            if_but_not_else = false;
        }

        // DEFINE_FUNC
        if (actToken.value.keyword == KEYWORD_FUNCTION)
        {
            is_program_body = false;
            if ((ret = Define_function()) != 0)
            {
                return ret;
            };

            
        }

        // program body

        else if (actToken.type != END_of_FILE && actToken.type != EPILOG)
        {
            is_program_body = true;

            if ((ret = Command()) != 0 && (ret != 69))
            {
                return ret;
            };

            if (ret != 69)
            {
                if (actToken.type != SEMICOLON)
                    return ERROR_SYNTAX_ANALYSIS;
            }
        }
        else if (actToken.type == EPILOG || actToken.type == END_of_FILE)
        {
            break;
        }
        else
        {
            return ERROR_SYNTAX_ANALYSIS;
        }
    } while (actToken.type != END_of_FILE && actToken.type != EPILOG);

    // check epilog - voluntary
    if (actToken.type == EPILOG)
    {

        if ((ret = Epilog()) != 0)
        {
            return ret;
        };
    }
    if (actToken.type != END_of_FILE)
    {
        return ERROR_SYNTAX_ANALYSIS;
    };
    Stack_Dispose_Table(&program_body_stack);
    Stack_Dispose_Table(&function_def_stack);

    return ERROR_OK;
}