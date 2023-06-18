/**
* Project - Compiler for IFJ22
*
* @brief Scanner for IFJ22 - Lexical analysis
*
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file scanner.h
*
**/

#ifndef _SCANNER_H
#define _SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include "dyn_string.h"
#include <ctype.h>



/**
 * @enum Keywords
 * @brief Keywords that have a specific meaning and therefore must not occur as identifiers
*/
typedef enum {
    NOT_A_KEYWORD,
    KEYWORD_IF,
    KEYWORD_ELSE,
    KEYWORD_WHILE,
    KEYWORD_FUNCTION,
    KEYWORD_RETURN,
    KEYWORD_FLOAT,
    KEYWORD_INT,
    KEYWORD_STRING,
    KEYWORD_VOID,
    KEYWORD_NULL,
    EMPTY_KEYWORD
} Keyword;

/**
 * @enum Token_types
 * @brief Token types, also used as states of finite automata
*/

typedef enum {
    PROLOG_PHP,         // prolog <?php
    PROLOG_DECLARE,     // "declare(strict_types=1);"
    EPILOG,             // epilog "?>"

    DEFAULT_STATE,      // default state of automata after prolog sequence

    COMMENT_PROLOG,     // comment section of prolog
    COMMENT_OR_DIVIDE,  // starting state for comments
    COMMENT_LINE,       // line comment
    COMMENT_BLOCK,      // block comment state
    COMMENT_BLOCK_END,  // ending block comment state

    ASSIGN_OR_EQUAL,
    ASSIGN,             // =
    PLUS,               // +
    MINUS,              // -
    MULTIPLY,           // *
    DIVIDE,             // /
    CONCAT,             // .

    MORE,               // >
    LESS,               // <
    LESS_OR_PROLOG,     // < or <?php, state only
    LESS_OR_EQUALS,     // <=
    MORE_OR_EQUALS,     // >=
    EQUALS,             // ===
    NOT,                // !, state only
    NOT_EQUALS,         // !==

    DOLLAR,             // $, state only
    VAR_ID,             // name/id of var after the $
    QUESTION_MARK,      // ?, state only
    KEYWORD_OR_FUNCTION,// either keyword or function
    KEYWORD,            // keyword type
    FUNCTION_ID,        // function ID

    // built-in functions:
    BUILT_IN_READS,
    BUILT_IN_READI,
    BUILT_IN_READF,
    BUILT_IN_WRITE,
    BUILT_IN_FLOATVAL,
    BUILT_IN_INTVAL,
    BUILT_IN_STRVAL,
    BUILT_IN_STRLEN,
    BUILT_IN_SUBSTRING,
    BUILT_IN_ORD,
    BUILT_IN_CHR,

    LEFT_PARENTHESIS,   // (
    RIGHT_PARENTHESIS,  // )
    LEFT_CURLY,         // {
    RIGHT_CURLY,        // }
    SEMICOLON,          // ;
    COMMA,              // ,
    COLON,              // :

    STRING,             // well, string
    INTEGER,            // integer
    FLOAT,              // float

    ESCAPE_SEQ,         // first state of escape sequences
    ESCAPE_SEQ_DDD,     // first state of ddd type of escape sequences
    ESCAPE_SEQ_XDD,     // first state of xdd type of escape sequences

    END_of_FILE,        // EOF
    EMPTY_TOKEN,        // initialized token

    BOOL                //bool
} Token_type;


/**
 * @union Token_value
 * @brief Definition of token values - word (id), keyword, integer or float
*/
typedef struct {
    Dyn_string string;
    Keyword keyword;
    int integer_value;
    double decimal_value;
} Token_value;


/**
 * @struct Token
 * @brief Definition of token with its value, type and other information
*/

typedef struct {
    Token_value value;
    Token_type type;
} Token;

/**
 * @brief Main function to get tokens
 * @param token token
 * @return function 'free_memory'
*/
int get_token(Token *token);

/**
 * @brief Function to set source for reading
 * @param FILE f
 * @return void
*/
void set_source(FILE *f) ;


/**
 * @brief Function to free allocated memory
 * @param int exit_code
 * @param Dyn_String string
 * @return exit_code
*/
int free_memory(int exit_code, Dyn_string *string);

/**
 * @brief Function to set token type to ID or KEYWORD
 * @param Dyn_string dynamic_sstring
 * @param Token token
 * @return Errors
*/
int identifier_check(Dyn_string * dynamic_string, Token *token);

/**
 * @brief Function to clear token (set default values)
 * @param Token token
 * @return Errors
*/
int clear_Token (Token * token);

/**
 * @brief Checks if given character is whitespace
 * @param ch Character
 * @return True if character is whitespace, otherwise false
 */
bool is_whitespace(char ch);

/**
 * @brief Checks if given dynamic string is keyword or function ID
 * @param string Dynamic string
 * @param token current token
 * @return ERROR_OK
 */
int check_keyword(Dyn_string string, Token *token);

/**
 * @brief Converts string value to integer
 * @param string
 * @param token
 * @return
 */
int process_integer(Dyn_string string, Token *token);

/**
 * @brief Converts string value to float
 * @param string
 * @param token
 * @return
 */
int process_float(Dyn_string string, Token *token);
#endif //_SCANNER_H
