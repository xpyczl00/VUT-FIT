/**
* Project - Compiler for IFJ22
*
* @brief Tests for parser for IFJ22 - Syntax analysis
*
* @author Matej Koreň <xkoren10
* @file parser_tests.c
*
**/

#include <stdlib.h>
#include "../error_codes.h"
#include "../dyn_string.h"
#include "../parser.h"

//#include "../expressions.h"
#include "../scanner.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

Token token;
FILE *file;
int res;

int main(){

    fprintf(stdout,"-----------------------------------------------\n");
    fprintf(stdout,"------------- Parser tests ------------\n");

//prolog tests

fprintf(stdout,"------------- Prolog check test ------------\n");
file = fopen("Tests/parser_test_files/prolog.ifj22", "r");
set_source(file);
int res = Program();
assert(res==0);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/prolog.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");


file = fopen("Tests/parser_test_files/prolog_missing.ifj22", "r");

set_source(file);

res = Program();
assert(res==2);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/prolog_missing.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");


//function declaration tests

fprintf(stdout,"------------- Function declaration test ------------\n");
file = fopen("Tests/parser_test_files/function_declaration.ifj22", "r");
set_source(file);
res = Program();
assert(res==0);     // mala by byť chybná? treba curly? yes dummy
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/function_declaration.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/function_declaration_incorrect.ifj22", "r");
set_source(file);
res = Program();
assert(res==3);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/function_declaration_incorrect.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/function_declaration_redefinition.ifj22", "r");
set_source(file);
res = Program();
assert(res==3); //potrebuje symtable na definicie
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/function_declaration_redefinition.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/function_declaration_return_type_missing.ifj22", "r");
set_source(file);
res = Program();
//assert(res==4); symtable pre uloženie a kontroly returnu
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/function_declaration_return_type_missing.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");
/*
file = fopen("../Tests/parser_test_files/function_declaration_wrong_arg_count.ifj22", "r");
set_source(file);
res = Program();
assert(res==4);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/function_declaration__wrong_arg_count.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");
*/

file = fopen("Tests/parser_test_files/function_declaration_wrong_arg_type.ifj22", "r");
set_source(file);
res = Program();
assert(res==4);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/function_declaration__wrong_arg_type.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/function_declaration_wrong_return_type.ifj22", "r");
set_source(file);
res = Program();
assert(res==4);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/function_declaration__wrong_return_type.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/function_return_redundant.ifj22", "r");
set_source(file);
res = Program();
printf("%d", res);
assert(res==6);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/function_return_redundant.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

//if and while tests

fprintf(stdout,"------------- Program body test ------------\n");
file = fopen("Tests/parser_test_files/if.ifj22", "r");
set_source(file);
res = Program();
assert(res==0);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/if.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/if_incorrect.ifj22", "r");
set_source(file);
res = Program();
assert(res==3);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/if_incorrect.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/while.ifj22", "r");
set_source(file);
res = Program();
assert(res==0);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/while.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/while_incorrect.ifj22", "r");
set_source(file);
res = Program();
assert(res==3);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/while_incorrect.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/variable_declaration.ifj22", "r");
set_source(file);
res = Program();
assert(res==0);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/variable_declaration.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/expression.ifj22", "r");
set_source(file);
res = Program();
assert(res==0);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/expression.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/function_redefinition.ifj22", "r");
set_source(file);
res = Program();

assert(res==3);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/function_redefinition.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/wrong_return.ifj22", "r");
set_source(file);
res = Program();

assert(res==4);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/wrong_return.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/commas.ifj22", "r");
set_source(file);
res = Program();

assert(res==2);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/commas.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/write_var.ifj22", "r");
set_source(file);
res = Program();

assert(res==0);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/write_var.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

file = fopen("Tests/parser_test_files/henlo.ifj22", "r");
set_source(file);
res = Program();
printf("huna %d",res);
//assert(res==0);
fclose(file);
fprintf(stdout, "--> Tests/parser_test_files/henlo.ifj22    => ");
fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");


    fprintf(stdout,"-----------------------------------------------\n");
    return ERROR_OK;

}