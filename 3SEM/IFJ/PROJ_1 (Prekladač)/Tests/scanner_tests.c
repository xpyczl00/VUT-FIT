/**
* Project - Compiler for IFJ22
*
* @brief Tests for scanner for IFJ22 - Lexical analysis
*
* @author Matej Koreň <xkoren10
* @file scanner_tests.c
*
**/

#include <stdlib.h>
#include "../error_codes.h"
#include "../dyn_string.h"
#include "../scanner.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

Dyn_string s;
Dyn_string z;
Token token;
FILE *file;

int main(){
    fprintf(stdout,"\x1B[31m""---------------- Scanner tests ----------------\n""\x1B[0m");
    fprintf(stdout,"-----------------------------------------------\n");
    fprintf(stdout,"------------- Dynamic string tests ------------\n");



    assert(dyn_string_init(&s)==0);

    fprintf(stdout, "--> DS Init       => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

    dyn_string_clear(&s);

    assert(s.length==0);
    assert(s.size==0);
    assert(s.string[0]=='\0');

    fprintf(stdout, "--> DS Clear      => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

    assert(dyn_string_add_char(&s, 'i')==true);
    assert(s.length==1);
    assert(s.string[0]=='i');

    fprintf(stdout, "--> DS Add        => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

    assert(dyn_string_add_const_str(&s, "ahoj")==true);
    assert(s.string[1]=='a');
    assert(s.length==5);


    fprintf(stdout, "--> DS Add Const. => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");


    assert(dyn_string_copy(&s, &z)==true);
    assert(s.length==z.length);


    fprintf(stdout, "--> DS Copy       => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

    assert(dyn_string_compare(z, "iahoj")==0);


    fprintf(stdout, "--> DS Compare    => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");


    fprintf(stdout,"-----------------------------------------------\n");
    fprintf(stdout,"------------ Lexical analysis tests -----------\n\n");


    file = fopen("Tests/basic.ifj22","r");
    set_source(file);

    get_token(&token);
    assert(token.type == PROLOG_PHP);
    get_token(&token);
    assert(token.type == FUNCTION_ID);
    assert(dyn_string_compare(token.value.string, "declare") == 0);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);
    assert(token.type == FUNCTION_ID);
    assert(dyn_string_compare(token.value.string, "strict_types") == 0);
    get_token(&token);  //(
    assert(token.type == ASSIGN);
    get_token(&token); // 1
    assert(token.type == INTEGER);
    assert(token.value.integer_value==1);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    /*get_token(&token);
    assert(token.type == EPILOG);*/
    get_token(&token);
    assert(token.type == END_of_FILE);
    fclose(file);

    fprintf(stdout, "--> IFJ21_codes/basic.ifj22     => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

    file = fopen("Tests/factorial.ifj22", "r");
    set_source(file);

    get_token(&token);
    assert(token.type == PROLOG_PHP);
    get_token(&token);
    assert(token.type == FUNCTION_ID);
    assert(dyn_string_compare(token.value.string, "declare") == 0);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);
    assert(token.type == FUNCTION_ID);
    assert(dyn_string_compare(token.value.string, "strict_types") == 0);
    get_token(&token);  //(
    assert(token.type == ASSIGN);
    get_token(&token); // 1
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 1);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    get_token(&token);  
    assert(token.type == BUILT_IN_WRITE);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);  //(
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, "Zadejte cislo pro vypocet faktorialu") == 0); // escape na \n???
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    get_token(&token);  //;
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "a") == 0);
    get_token(&token);  //(
    assert(token.type == ASSIGN);
    get_token(&token);
    assert(token.type == BUILT_IN_READI);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    get_token(&token);  //if
    assert(token.type == KEYWORD);
    assert(token.value.keyword == KEYWORD_IF);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "a") == 0);
    get_token(&token);  //===
    assert(token.type == EQUALS);
    get_token(&token);  //null
    assert(token.type == KEYWORD);
    assert(token.value.keyword == KEYWORD_NULL);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //{
    assert(token.type == LEFT_CURLY);
    get_token(&token);
    assert(token.type == BUILT_IN_WRITE);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);  //(
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, "Faktorial nelze spocitat") == 0); // escape na \n???
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    get_token(&token);  //{
    assert(token.type == RIGHT_CURLY);
    get_token(&token);  //else
    assert(token.type == KEYWORD);
    assert(token.value.keyword == KEYWORD_ELSE);
    get_token(&token);  //{
    assert(token.type == LEFT_CURLY);
    get_token(&token);  //}
    assert(token.type == RIGHT_CURLY);
    get_token(&token);  //if
    assert(token.type == KEYWORD);
    assert(token.value.keyword == KEYWORD_IF);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "a") == 0);
    get_token(&token);  //<
    assert(token.type == LESS);
    get_token(&token); // 1
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 0);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //}
    assert(token.type == LEFT_CURLY);
    get_token(&token);
    assert(token.type == BUILT_IN_WRITE);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);  //(
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, "Faktorial nelze spocitat") == 0); // escape na \n???
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    get_token(&token);  //{
    assert(token.type == RIGHT_CURLY);
    get_token(&token);  //else
    assert(token.type == KEYWORD);
    assert(token.value.keyword == KEYWORD_ELSE);
    get_token(&token);  //{
    assert(token.type == LEFT_CURLY);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "vysl") == 0);
    get_token(&token);  //=
    assert(token.type == ASSIGN);
    get_token(&token); // 1
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 1);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    get_token(&token);  //else
    assert(token.type == KEYWORD);
    assert(token.value.keyword == KEYWORD_WHILE);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "a") == 0);
    get_token(&token);  //<
    assert(token.type == MORE);
    get_token(&token); // 1
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 0);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //}
    assert(token.type == LEFT_CURLY);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "vysl") == 0);
    get_token(&token);  //=
    assert(token.type == ASSIGN);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "vysl") == 0);
    get_token(&token); //*
    assert(token.type == MULTIPLY);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "a") == 0);
    get_token(&token); //;
    assert(token.type == SEMICOLON);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "a") == 0);
    get_token(&token);  //=
    assert(token.type == ASSIGN);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "a") == 0);
    get_token(&token); //*
    assert(token.type == MINUS);
    get_token(&token); // 1
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 1);
    get_token(&token); //;
    assert(token.type == SEMICOLON);
    get_token(&token);  //}
    assert(token.type == RIGHT_CURLY);
    get_token(&token);
    assert(token.type == BUILT_IN_WRITE);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);  
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, "Vysledek je: ") == 0); // escape na \n???
    get_token(&token);  //)
    assert(token.type == COMMA);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "vysl") == 0);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    get_token(&token);  //{
    assert(token.type == RIGHT_CURLY);
    get_token(&token);
    assert(token.type == END_of_FILE);

    fclose(file);

    fprintf(stdout, "--> IFJ21_codes/factorial.ifj22 => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

    file = fopen("Tests/string.ifj22", "r");
    set_source(file);

    get_token(&token);
    assert(token.type == PROLOG_PHP);
    get_token(&token);
    assert(token.type == FUNCTION_ID);
    assert(dyn_string_compare(token.value.string, "declare") == 0);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);
    assert(token.type == FUNCTION_ID);
    assert(dyn_string_compare(token.value.string, "strict_types") == 0);
    get_token(&token);  //(
    assert(token.type == ASSIGN);
    get_token(&token); // 1
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 1);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);

    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "str1") == 0);
    get_token(&token);  //=
    assert(token.type == ASSIGN);
    get_token(&token);  //(
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, "Toto je nejaky text v programu jazyka IFJ22\n") == 0);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "str2") == 0);
    get_token(&token);  //=
    assert(token.type == ASSIGN);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "str1") == 0);
    get_token(&token);  //.
    assert(token.type == CONCAT);
    get_token(&token);
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, ", ktery jeste trochu obohatime") == 0);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);

    get_token(&token);
    assert(token.type == BUILT_IN_WRITE);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);  //
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "str1") == 0);
    get_token(&token);  //,
    assert(token.type == COMMA);
    get_token(&token);
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, "\\999") == 0);
    get_token(&token);  //,
    assert(token.type == COMMA);
    get_token(&token);  //
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "str2") == 0);
    get_token(&token);  //,
    assert(token.type == COMMA);
    get_token(&token);
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, "\n") == 0);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);

    //huh
    get_token(&token);
    assert(token.type == BUILT_IN_WRITE);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);  //(
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, "Pozice retezce text v str2: ") == 0);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);


    get_token(&token);
    assert(token.type == BUILT_IN_WRITE);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);  //(
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, "Zadejte serazenou posloupnost vsech malych pismen a-h, ") == 0);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    get_token(&token);  //)
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "str1") == 0);
    get_token(&token);  //=
    assert(token.type == ASSIGN);
    get_token(&token);
    assert(token.type == BUILT_IN_READS); 
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    get_token(&token);  //if
    assert(token.type == KEYWORD);
    assert(token.value.keyword == KEYWORD_IF);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "str1") == 0);
    get_token(&token);  //!==
    assert(token.type == NOT_EQUALS);
    get_token(&token);  //if
    assert(token.type == KEYWORD);
    assert(token.value.keyword == KEYWORD_NULL);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);
    assert(token.type == LEFT_CURLY);
    get_token(&token);  
    assert(token.type == KEYWORD);
    assert(token.value.keyword == KEYWORD_WHILE);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "str1") == 0);
    get_token(&token);  //!==
    assert(token.type == NOT_EQUALS);
    get_token(&token);  
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, "abcdefgh") == 0);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);
    assert(token.type == LEFT_CURLY);
    get_token(&token);
    assert(token.type == BUILT_IN_WRITE);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);  //(
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, "Spatne zadana posloupnost, zkuste znovu:\n") == 0);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    get_token(&token);  //)
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "str1") == 0);
    get_token(&token);  //=
    assert(token.type == ASSIGN);
    get_token(&token);
    assert(token.type == BUILT_IN_READS);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);
    get_token(&token);
    assert(token.type == RIGHT_CURLY);
    get_token(&token);
    assert(token.type == RIGHT_CURLY);
    get_token(&token);  //else
    assert(token.type == KEYWORD);
    assert(token.value.keyword == KEYWORD_ELSE);
    get_token(&token);
    assert(token.type == LEFT_CURLY);
    get_token(&token);
    assert(token.type == RIGHT_CURLY);
    get_token(&token);
    assert(token.type == EPILOG);
    get_token(&token);
    assert(token.type == END_of_FILE);

    fclose(file);

    fprintf(stdout, "--> IFJ21_codes/string.ifj22    => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");


    file = fopen("Tests/missing_exp.ifj22","r");
    set_source(file);

    get_token(&token);
    assert(token.type == PROLOG_PHP);
    get_token(&token);
    assert(token.type == FUNCTION_ID);
    assert(dyn_string_compare(token.value.string, "declare") == 0);
    get_token(&token);  //(
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);
    assert(token.type == FUNCTION_ID);
    assert(dyn_string_compare(token.value.string, "strict_types") == 0);
    get_token(&token);  //(
    assert(token.type == ASSIGN);
    get_token(&token); // 1
    assert(token.type == INTEGER);
    assert(token.value.integer_value==1);
    get_token(&token);  //)
    assert(token.type == RIGHT_PARENTHESIS);
    get_token(&token);  //;
    assert(token.type == SEMICOLON);

    get_token(&token);
    assert(token.type == VAR_ID);
    assert(dyn_string_compare(token.value.string, "x") == 0);
    get_token(&token);  //!==
    assert(token.type == ASSIGN);

    get_token(&token);  //!==
    printf("%d jebbe\n",token.type);
    assert(token.type == EMPTY_TOKEN);
    assert(token.value.decimal_value==0.0);

    get_token(&token);  //;
    printf("%d jebbe\n",token.type);
    //assert(token.type == SEMICOLON);

    get_token(&token);
    assert(token.type == END_of_FILE);
    fclose(file);

    fprintf(stdout, "--> IFJ21_codes/missing_exp.ifj22     => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");


    fprintf(stdout,"-----------------------------------------------\n");
    return ERROR_OK;
}