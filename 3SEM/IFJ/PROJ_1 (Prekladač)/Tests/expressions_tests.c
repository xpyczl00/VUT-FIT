/**
* Project - Compiler for IFJ21
* 
* @brief Tests for parser for IFJ21 - Lexical analysis
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file parser_tests.c
*
**/
#include <stdlib.h>
#include "../error_codes.h"
#include "../dyn_string.h"
#include "../expressions.h"
#include "../scanner.h"
#include "../symtable.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

Token token;
Token token_janko;
FILE *file;
int res;

ht_table_t test_vartable;
ht_table_t *vartable_ptr=&test_vartable;

ht_table_t test_functable;
ht_table_t *functable_ptr=&test_functable;

TStack_table var_stack;


TStack_table func_stack;



ht_item_t * item;



int main() {

    Stack_Init_Table(&var_stack); 
    Stack_Init_Table(&func_stack); 
//fprintf(stdout,"%d type -- %d res -- %s --id %s string\n", token.type, res, sym.value_type, sym.id); prípadný test

    fprintf(stdout, "\x1B[31m""---------------- Expressions tests ----------------\n""\x1B[0m");

    file = fopen("Tests/basic_expression.ifj22", "r");
    set_source(file);

    get_token(&token);
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 3);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    fclose(file);


    fprintf(stdout, "--> Tests/Expressions/basic.txt => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");


    file = fopen("Tests/Expressions/basic.txt", "r");
    set_source(file);

    get_token(&token);
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 5);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    assert(token.type == INTEGER);
    assert(token.value.integer_value == 8);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    assert(token.type == INTEGER);
    assert(token.value.integer_value == 69);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    assert(token.type == INTEGER);
    assert(token.value.integer_value == 42);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    assert(token.type == INTEGER);
    assert(token.value.integer_value == 5);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    assert(token.type == LEFT_PARENTHESIS);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(token.type == END_of_FILE);
    assert(res == 0);

    fclose(file);
    fprintf(stdout, "--> Tests/Expressions/basic.txt => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

    
    file = fopen("Tests/Expressions/basic2.txt", "r");
    set_source(file);

    get_token(&token);
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 3);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 2);

    get_token(&token);
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 1);
    get_token(&token);
    assert(token.type == LEFT_PARENTHESIS);
    get_token(&token);
    assert(token.type == LEFT_PARENTHESIS);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 2);

    fclose(file);
    fprintf(stdout, "--> Tests/Expressions/basic2.txt => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");

    
    /*file = fopen("Tests/Expressions/concatenate.txt", "r");
    set_source(file);

    get_token(&token);
    assert(token.type == STRING);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,&func_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    get_token(&token);
    printf("%d <--\n",token.type);
    assert(token.type == STRING);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,&func_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    get_token(&token);
    assert(token.type == FUNCTION_ID);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,&func_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 7);

    get_token(&token);
    assert(token.type == STRING);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,&func_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 7);

    fclose(file);
    fprintf(stdout, "--> Tests/Expressions/concatenate.txt => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");*/

    file = fopen("Tests/Expressions/conditions.txt", "r");
    set_source(file);

    get_token(&token);
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 5);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    fclose(file);
    fprintf(stdout, "--> Tests/Expressions/conditions.txt => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");



    file = fopen("Tests/Expressions/conditions2.txt", "r");
    set_source(file);

    get_token(&token);
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 20);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    fclose(file);
    fprintf(stdout, "--> Tests/Expressions/conditions2.txt => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");


    file = fopen("Tests/Expressions/conditions3.txt", "r");
    set_source(file);

    get_token(&token);
    assert(token.type == STRING);
    assert(dyn_string_compare(token.value.string, "test")==0);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    fclose(file);
    fprintf(stdout, "--> Tests/Expressions/conditions3.txt => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");


    file = fopen("Tests/Expressions/logic_and_cond.txt", "r");
    set_source(file);

    get_token(&token);
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 5);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    //get_token(&token);
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 14);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    //get_token(&token);
    assert(token.type == INTEGER);
    assert(token.value.integer_value == 69);
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);

    fclose(file);
    fprintf(stdout, "--> Tests/Expressions/logic_and_cond.txt => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");


    // Function
    /*ht_item_t * item;
    set_item_null(item);
    item->key== "foo";
    item->input_param_count==1;
    values_t *new_par = malloc(sizeof(values_t));
    
    new_par->item_type = INTEGER;
    new_par->item_id = "$a";
    item->first_par=new_par;
    ht_insert(&test_functable, item);
    assert(ht_search(&test_functable,"foo")!=NULL);
    
    Stack_Push_Table(&func_stack, "foo");  

    file = fopen("Tests/Expressions/function.txt", "r");
    set_source(file);

    get_token(&token);
    assert(token.type == FUNCTION_ID); 
    res = expression_analysis(&token,functable_ptr,vartable_ptr,&var_stack,&func_stack,item);
    fprintf(stdout, "\nresult = %d\n", res);
    assert(res == 0);


    fclose(file);
    fprintf(stdout, "--> Tests/Expressions/function.txt => ");
    fprintf(stdout, "\x1B[32m"" PASSED \n""\x1B[0m");
*/
}