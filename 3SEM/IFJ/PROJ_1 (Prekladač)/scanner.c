/**
 * Project - Compiler for IFJ22
 *
 * @brief Scanner for IFJ22 - Lexical analysis
 *
 * @author Matej Koreň <xkoren10
 * @author Matej Hložek <xhloze02>
 * @file scanner.c
 *
 **/

#include "scanner.h"
FILE *source_file;

void set_source(FILE *f)
{
    source_file = f;
}

int clear_Token(Token *token)
{

    token->type = EMPTY_TOKEN;
    token->value.decimal_value = 0.0;
    token->value.integer_value = 0;
    token->value.keyword = EMPTY_KEYWORD;
    token->value.string.length = 0;
    token->value.string.size = 1;
    token->value.string.string = '\0';

    return 0;
}

int free_memory(int exit_code, Dyn_string *string)
{
    dyn_string_free(string);
    return exit_code;
}

bool is_whitespace(char ch)
{
    if (ch == '\n' || ch == '\t' || isspace(ch))
    {
        return true;
    }
    return false;
}

int process_integer(Dyn_string string, Token *token)
{
    char *ptr;
    int integer = (int)strtol(string.string, &ptr, 10);

    if (*ptr)
    {
        return free_memory(ERROR_LEXICAL_ANALISYS, &string);
    }

    (*token).value.integer_value = integer;
    (*token).type = INTEGER;

    return ERROR_OK;
}

int process_float(Dyn_string string, Token *token)
{
    char *ptr;
    double double_number = strtod(string.string, &ptr);

    if (*ptr)
    {
        return free_memory(ERROR_LEXICAL_ANALISYS, &string);
        
    }

    if (double_number == 0.0 && strcmp(string.string, "0.0") != 0)
    {
        return ERROR_LEXICAL_ANALISYS;
    }

    (*token).value.decimal_value = double_number;
    (*token).type = FLOAT;
    return ERROR_OK;
}

int check_keyword(Dyn_string string, Token *token)
{

    // KEYWORDS:
    if (strcmp(string.string, "if") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_IF;
    }
    else if (strcmp(string.string, "else") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_ELSE;
    }
    else if (strcmp(string.string, "while") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_WHILE;
    }
    else if (strcmp(string.string, "function") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_FUNCTION;
    }
    else if (strcmp(string.string, "return") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_RETURN;
    }
    else if (strcmp(string.string, "float") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_FLOAT;
    }
    else if (strcmp(string.string, "int") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_INT;
    }
    else if (strcmp(string.string, "string") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_STRING;
    }
    else if (strcmp(string.string, "void") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_VOID;
    }
    else if (strcmp(string.string, "null") == 0)
    {
        (*token).type = KEYWORD;
        (*token).value.keyword = KEYWORD_NULL;
    }

    // BUILT-INS:
    else if (strcmp(string.string, "reads") == 0)
    {
        (*token).type = BUILT_IN_READS;
    }
    else if (strcmp(string.string, "readi") == 0)
    {
        (*token).type = BUILT_IN_READI;
    }
    else if (strcmp(string.string, "readf") == 0)
    {
        (*token).type = BUILT_IN_READF;
    }
    else if (strcmp(string.string, "write") == 0)
    {
        (*token).type = BUILT_IN_WRITE;
    }
    else if (strcmp(string.string, "floatval") == 0)
    {
        (*token).type = BUILT_IN_FLOATVAL;
    }
    else if (strcmp(string.string, "intval") == 0)
    {
        (*token).type = BUILT_IN_INTVAL;
    }
    else if (strcmp(string.string, "strval") == 0)
    {
        (*token).type = BUILT_IN_STRVAL;
    }
    else if (strcmp(string.string, "strlen") == 0)
    {
        (*token).type = BUILT_IN_STRLEN;
    }
    else if (strcmp(string.string, "substring") == 0)
    {
        (*token).type = BUILT_IN_SUBSTRING;
    }
    else if (strcmp(string.string, "ord") == 0)
    {
        (*token).type = BUILT_IN_ORD;
    }
    else if (strcmp(string.string, "chr") == 0)
    {
        (*token).type = BUILT_IN_CHR;
    }

    // else it is a function ID
    else
    {
        (*token).type = FUNCTION_ID;
        if (!dyn_string_copy(&string, &token->value.string))
        {
            return free_memory(ERROR_INTERN, &string);
        }
    }
    return ERROR_OK;
}

int get_token(Token *token)
{
    if (source_file == NULL)
    {
        return ERROR_INTERN;
    }

    int state = DEFAULT_STATE;
    int esc_cnt = 0;
    int esc_val = 0;

    Dyn_string string;
    Dyn_string escape_string;

    // set dynamic strings and token to default values
    if (dyn_string_init(&string) != 0)
        return ERROR_INTERN;
    if (dyn_string_init(&escape_string) != 0)
        return ERROR_INTERN;
    if (clear_Token(token) != 0)
        return ERROR_INTERN;

    char ch;

    while (true)
    {

        ch = getc(source_file);

        // EOF during other states then default means error
        if (state != DEFAULT_STATE && ch == EOF && state != COMMENT_LINE)
        {

            return free_memory(ERROR_LEXICAL_ANALISYS, &string);
        }

        switch (state)
        {
        // state changes depending on current character
        case (DEFAULT_STATE):

            // EOF
            if (ch == EOF)
            {
                token->type = END_of_FILE;
                return free_memory(ERROR_OK, &string);
            }

            // WHITESPACE
            else if (is_whitespace(ch))
            {
                continue;
            }

            // alpha characters
            else if (isalpha(ch) != 0)
            {
                //  if it is a-zA-Z
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
                state = KEYWORD_OR_FUNCTION;
            }

            // numeric characters
            else if (isdigit(ch) != 0)
            {
                // if digit
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
                state = INTEGER;
            }

            // other characters
            else
            {
                switch (ch)
                {
                case ('$'):
                    state = DOLLAR;
                    break;
                case ('?'):
                    state = QUESTION_MARK;
                    break;
                case ('"'):
                    state = STRING;
                    break;
                case ('/'):
                    state = COMMENT_OR_DIVIDE;
                    break;
                case ('+'):
                    token->type = PLUS;
                    return free_memory(ERROR_OK, &string);
                case ('-'):
                    token->type = MINUS;
                    return free_memory(ERROR_OK, &string);
                case ('*'):
                    token->type = MULTIPLY;
                    return free_memory(ERROR_OK, &string);
                case ('.'):
                    token->type = CONCAT;
                    return free_memory(ERROR_OK, &string);
                case ('<'):
                    state = LESS_OR_PROLOG;
                    break;
                case ('>'):
                    token->type = MORE;
                    return free_memory(ERROR_OK, &string);
                case ('='):
                    state = ASSIGN_OR_EQUAL;
                    break;
                case ('!'):
                    state = NOT;
                    break;
                case (';'):
                    token->type = SEMICOLON;
                    return free_memory(ERROR_OK, &string);
                case (':'):
                    token->type = COLON;
                    return free_memory(ERROR_OK, &string);
                case (','):
                    token->type = COMMA;
                    return free_memory(ERROR_OK, &string);
                case ('('):
                    token->type = LEFT_PARENTHESIS;
                    return free_memory(ERROR_OK, &string);
                case (')'):
                    token->type = RIGHT_PARENTHESIS;
                    return free_memory(ERROR_OK, &string);
                case ('{'):
                    token->type = LEFT_CURLY;
                    return free_memory(ERROR_OK, &string);
                case ('}'):
                    token->type = RIGHT_CURLY;
                    return free_memory(ERROR_OK, &string);
                // unknown character means error
                default:
                    return free_memory(ERROR_LEXICAL_ANALISYS, &string);
                }
            }
            break;

        case (LESS_OR_PROLOG):
            if (ch == '?')
            {
                state = PROLOG_PHP;
            }
            else if (ch == '=')
            {
                state = DEFAULT_STATE;
                token->type = LESS_OR_EQUALS;
                return free_memory(ERROR_OK, &string);
            }
            else
            {
                token->type = LESS;
                return free_memory(ERROR_OK, &string);
            }
            break;

        case (PROLOG_PHP):
            if (isalpha(ch))
            {
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
            }
            else
            {
                if (ch == EOF)
                {
                    return free_memory(ERROR_LEXICAL_ANALISYS, &string);
                }
                if (dyn_string_compare(string, "php") == 0)
                {
                    token->type = PROLOG_PHP;
                    return free_memory(ERROR_OK, &string);
                }
                return free_memory(ERROR_LEXICAL_ANALISYS, &string);
            }
            break;

        case (DOLLAR):
            // first character of var id should be either alpha char or underscore
            if (isalpha(ch) || ch == '_')
            {
                state = VAR_ID;
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
            }
            else
            {
                return free_memory(ERROR_LEXICAL_ANALISYS, &string);
            }
            break;

        case (VAR_ID):
            if (isalnum(ch) || ch == '_')
            {
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
            }
            else
            {
                ungetc(ch, source_file);
                token->type = VAR_ID;
                if (!dyn_string_copy(&string, &(token->value.string)))
                    return ERROR_INTERN;
                return free_memory(ERROR_OK, &string);
            }
            break;

        case (QUESTION_MARK):
            if (isalpha(ch))
            {
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
            }
            else if (ch == '>')
            {
                token->type = EPILOG;
                ch = getc(source_file);
                if (ch != EOF){
                    return free_memory(ERROR_LEXICAL_ANALISYS, &string);
                }
                ungetc(ch, source_file);
                return free_memory(ERROR_OK, &string);
            }
            else
            {
                ungetc(ch, source_file);
                check_keyword(string, token);

                // there should be only FLOAT, INT or STRING after ?
                // so token type needs to be KEYWORD and its type either FLOAT, INT or STRING
                if (token->type == KEYWORD)
                {
                    // get keyword type
                    Keyword type = token->value.keyword;

                    if (type == KEYWORD_FLOAT || type == KEYWORD_INT || type == KEYWORD_STRING)
                    {
                        return free_memory(ERROR_OK, &string);
                    }
                    else
                    {
                        return free_memory(ERROR_LEXICAL_ANALISYS, &string);
                    }
                }
                else
                {
                    return free_memory(ERROR_LEXICAL_ANALISYS, &string);
                }
            }
            break;

        case (ASSIGN_OR_EQUAL):
            // second '=', first was in default state
            if (ch == '=')
            {
                state = EQUALS;
            }
            else
            {
                ungetc(ch, source_file);
                token->type = ASSIGN;
                return free_memory(ERROR_OK, &string);
            }
            break;

        case (EQUALS):
            // third and last '='
            if (ch == '=')
            {
                token->type = EQUALS;
                return free_memory(ERROR_OK, &string);
            }
            else
            {
                return free_memory(ERROR_LEXICAL_ANALISYS, &string);
            }
            break;

        case (NOT):
            // first '=' after '!'
            if (ch == '=')
            {
                state = NOT_EQUALS;
            }
            else
            {
                return free_memory(ERROR_LEXICAL_ANALISYS, &string);
            }
            break;

        case (NOT_EQUALS):
            // second and last '='
            if (ch == '=')
            {
                token->type = NOT_EQUALS;
                return free_memory(ERROR_OK, &string);
            }
            else
            {
                return free_memory(ERROR_LEXICAL_ANALISYS, &string);
            }
            break;

        case (KEYWORD_OR_FUNCTION):
            if (isalpha(ch))
            {
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
            }
            else if (isdigit(ch) || ch == '_')
            {
                state = FUNCTION_ID;
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
            }
            else
            {
                ungetc(ch, source_file);
                check_keyword(string, token);
                return free_memory(ERROR_OK, &string);
            }
            break;

        case (FUNCTION_ID):
            if (isalnum(ch) || ch == '_')
            {
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
            }
            else
            {
                ungetc(ch, source_file);
                token->type = FUNCTION_ID;
                if (!dyn_string_copy(&string, &token->value.string))
                {
                    return free_memory(ERROR_INTERN, &string);
                }
                return free_memory(ERROR_OK, &string);
            }
            break;

        case (INTEGER):
            if (isdigit(ch))
            {
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
            }
            else if (ch == '.' || ch == 'e' || ch == 'E')
            {
                state = FLOAT;
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
            }
            else
            {
                ungetc(ch, source_file);
                return process_integer(string, token);
            }
            break;

        case (FLOAT):

            if (isdigit(ch) || ch == 'e')
            {
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
            }
            else
            {
                ungetc(ch, source_file);
                return process_float(string, token);
            }
            break;

        case (STRING):
            if (ch == '"')
            {
                token->type = STRING;
                if (!dyn_string_copy(&string, &token->value.string))
                {
                    return free_memory(ERROR_INTERN, &string);
                }
                return free_memory(ERROR_OK, &string);
            }
            else if (ch == '\\')
            {
                state = ESCAPE_SEQ;
            }
            else if(ch == ' '){
                if (!dyn_string_add_char(&string, '\\'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '0'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '3'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '2'))
                    return ERROR_INTERN;
            }
            else if (ch == EOF)
            {
                return free_memory(ERROR_LEXICAL_ANALISYS, &string);
            }
            else
            {
                if (!dyn_string_add_char(&string, ch))
                    return ERROR_INTERN;
            }
            break;

        case (COMMENT_OR_DIVIDE):
            if (ch == '/')
            {
                state = COMMENT_LINE;
            }
            else if (ch == '*')
            {
                state = COMMENT_BLOCK;
            }
            else
            {
                ungetc(ch, source_file);
                token->type = DIVIDE;
                return free_memory(ERROR_OK, &string);
            }
            break;

        case (COMMENT_LINE):
            if (ch == '\n')
            {

                state = DEFAULT_STATE;
            }
            else if (ch == EOF)
            {
                token->type = END_of_FILE;
                return free_memory(ERROR_OK, &string);
            }
            else
            {

                continue;
            }
            break;

        case (COMMENT_BLOCK):
            if (ch == '*')
            {
                state = COMMENT_BLOCK_END;
            }
            else if (ch == EOF)
            {
                token->type = END_of_FILE;
                return free_memory(ERROR_OK, &string);
            }
            else
            {
                continue;
            }
            break;

        case (COMMENT_BLOCK_END):
            if (ch == '/')
            {
                state = DEFAULT_STATE;
            }
            else
            {
                state = COMMENT_BLOCK;
            }
            break;

        case ESCAPE_SEQ:

            if (ch == '\\')
            {
                if (!dyn_string_add_char(&string, '\\'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '0'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '9'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '2'))
                    return ERROR_INTERN;
                state = STRING;
            }
            else if (ch == 'n')
            {
                if (!dyn_string_add_char(&string, '\\'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '0'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '1'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '0'))
                    return ERROR_INTERN;
                state = STRING;
            }
            else if (ch == 't')
            {
                if (!dyn_string_add_char(&string, '\\'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '0'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '0'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '9'))
                    return ERROR_INTERN;
                state = STRING;
            }
            else if (ch == '"')
            {
                if (!dyn_string_add_char(&string, '\\'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '0'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '3'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '4'))
                    return ERROR_INTERN;
                state = STRING;
            }
            else if (ch == '$'){
                if (!dyn_string_add_char(&string, '\\'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '0'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '3'))
                    return ERROR_INTERN;
                if (!dyn_string_add_char(&string, '6'))
                    return ERROR_INTERN;
                state = STRING;
            }

            else if (ch == 'x')
            {
                dyn_string_add_char(&string, ch);
                // pokračovanie příště
                esc_cnt = 1;
            }

            else if (isdigit(ch))
            {
                switch (esc_cnt)

                {
                case 0:
                    esc_val += ((int)ch - 48) * 100;
                    esc_cnt++;
                    break;

                case 1:
                    esc_val += ((int)ch - 48) * 10;
                    esc_cnt++;

                    break;

                case 2:
                    esc_val += ((int)ch - 48);

                    if ((0 < esc_val) && (esc_val < 255))
                    {
                        dyn_string_add_char(&string, (char)esc_val);
                        esc_cnt = 0;
                        state = STRING;
                    }
                    else
                    {
                        char str[3];
                        sprintf(str, "%d", esc_val);
                        dyn_string_add_char(&string, '\\');
                        dyn_string_add_const_str(&string, str);
                        esc_cnt = 0;
                        state = STRING;
                    }
                    break;

                default:
                    return free_memory(ERROR_LEXICAL_ANALISYS, &string);
                }
            }

            else
                return free_memory(ERROR_LEXICAL_ANALISYS, &string);
            break;
        default:
            return free_memory(ERROR_LEXICAL_ANALISYS, &string);
        }
    }

    return ERROR_OK;
}
