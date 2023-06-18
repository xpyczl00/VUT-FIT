/**
* Project - Compiler for IFJ22
* 
* @brief Code generator for IFJ22
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @author Marek Križan <xkriza08>
* @file code_gen.c
*
**/

#include "code_gen.h"

// counters for function, if and loop labels
unsigned if_i = 0;
unsigned loop_i = 0;
unsigned func_i = 0;

// generates header and other global variables
void gen_header(){
    // header
    fprintf(stdout, ".IFJcode22\n");
    // definition of global variables used in some instructions
    fprintf(stdout, "DEFVAR GF@RESULT_L\n");
    fprintf(stdout, "DEFVAR GF@RESULT_R\n");
    fprintf(stdout, "DEFVAR GF@EXP_L\n");
    fprintf(stdout, "DEFVAR GF@EXP_R\n");
    fprintf(stdout, "DEFVAR GF@VARstring\n");
    // creation of first frame
    fprintf(stdout, "CREATEFRAME\n");
    fprintf(stdout, "PUSHFRAME\n");
}

// generates pushs instruction
void gen_push(Token op){
    switch (op.type) {
        // with integer constant as parameter
        case (INTEGER):
            fprintf(stdout, "PUSHS int@%d\n", op.value.integer_value);
            break;
        // with float constant as parameter
        case (FLOAT):
            fprintf(stdout, "PUSHS float@%a\n", op.value.decimal_value);  // float to string? how many digits?
            break;
        // with string constant as parameter
        case (STRING):
            fprintf(stdout, "PUSHS string@%s\n", op.value.string.string);
            break;
        // with variable from local frame as parameter
        case (VAR_ID):
            fprintf(stdout, "PUSHS LF@%s\n", op.value.string.string);
            break;
        // with nil constant as parameter
        case (KEYWORD):
            fprintf(stdout, "PUSHS nil@nil\n");
            break;
        // with integer 0 as parameter
        case (SEMICOLON):
            fprintf(stdout, "PUSHS int@0\n");
            break;
        // else generates nothing
        default:
            return;
    }
}

// generates just pushs with bool false
void gen_push_false(){
    fprintf(stdout, "PUSHS bool@false\n");
}

// generate pops instruction with given variable
void gen_pops(ht_item_t var){
    fprintf(stdout, "POPS LF@%s\n", var.key);
}

// generates arithmetic stack instruction
void gen_instruction(Token op){
    switch (op.type) {
        case (PLUS):
            fprintf(stdout, "ADDS\n");
            break;
        case (MINUS):
            fprintf(stdout, "SUBS\n");
            break;
        case (MULTIPLY):
            fprintf(stdout, "MULS\n");
            break;
        case (DIVIDE):
            fprintf(stdout, "DIVS\n");
            break;
        // with concat parameters will be firstly popped from stack, then concatenated and result will be pushed back to stack
        case (CONCAT):
            fprintf(stdout, "POPS GF@RESULT_R\n");
            fprintf(stdout, "POPS GF@RESULT_L\n");
            fprintf(stdout, "CONCAT GF@VARstring GF@RESULT_L GF@RESULT_R\n");
            fprintf(stdout, "PUSHS GF@VARstring\n");
        default:
            break;
    }
}

// generates variable definition
void gen_var_def(char* id){
    fprintf(stdout, "DEFVAR LF@%s\n", id);
}

// generates move instruction
// redundant, stack (POPS) is used instead
void gen_var_setval(Token var, Token val){
    switch (var.type) {
        // set variable as integer
        case (INTEGER):
            fprintf(stdout, "MOVE LF@%s int@%d\n", var.value.string.string, val.value.integer_value);
            break;
        // set variable as float
        case (FLOAT):
            fprintf(stdout, "MOVE LF@%s float@%a\n", var.value.string.string, val.value.decimal_value);
            break;
        // set variable as string
        case (STRING):
            fprintf(stdout, "MOVE LF@%s string@%s\n", var.value.string.string, val.value.string.string);
            break;
        // set variable value as value from other variable
        default:
            // might be error here
            fprintf(stdout, "MOVE LF@%s LF@%s\n", var.value.string.string, val.value.string.string);
            return;
    }
}

// generates type instruction
void gen_type(Token var, Token symbol){
    switch (symbol.type) {
        case (INTEGER):
            fprintf(stdout, "TYPE LF@%s int@%d\n", var.value.string.string, symbol.value.integer_value);
            break;
        case (FLOAT):
            fprintf(stdout, "TYPE LF@%s float@%a\n", var.value.string.string, symbol.value.decimal_value);
            break;
        case (STRING):
            fprintf(stdout, "TYPE LF@%s string@%s\n", var.value.string.string, symbol.value.string.string);
            break;
        case (VAR_ID):
            fprintf(stdout, "TYPE LF@%s LF@%s\n", var.value.string.string, symbol.value.string.string);
            break;
        default:
            return;
    }
}

// generates built-in strlen function
void gen_strlen(char* var_name){
    // set value from stack to variable (var)
    fprintf(stdout, "POPS GF@VARstring\n");
    fprintf(stdout, "STRLEN LF@%s GF@VARstring\n", var_name);
}

// generates built-in getchar function
void gen_getchar(Token var, Token left_symbol, Token right_symbol){
    switch (left_symbol.type) {
        // if 1st symbol is var
        case(VAR_ID):
            // 2nd symbol is var
            if (right_symbol.type == VAR_ID){
                fprintf(stdout, "GETCHAR LF@%s LF@%s LF@%s\n", var.value.string.string, left_symbol.value.string.string, right_symbol.value.string.string);
            }
            // 2nd symbol is integer
            else if(right_symbol.type == INTEGER){
                fprintf(stdout, "GETCHAR LF@%s LF@%s int@%d\n", var.value.string.string, left_symbol.value.string.string, right_symbol.value.integer_value);
            }
            else{
                return;
            }
            break;
        // if 1st symbol string
        case(STRING):
            // 2nd symbol is var
            if (right_symbol.type == VAR_ID){
                fprintf(stdout, "GETCHAR LF@%s string@%s LF@%s\n", var.value.string.string, left_symbol.value.string.string, right_symbol.value.string.string);
            }
            // 2nd symbol is integer
            else if(right_symbol.type == INTEGER){
                fprintf(stdout, "GETCHAR LF@%s string@%s int@%d\n", var.value.string.string, left_symbol.value.string.string, right_symbol.value.integer_value);
            }
            else{
                return;
            }
            break;
        default:
            return;
    }
}

// generates built-in setchar function
void gen_setchar(Token var, Token left_symbol, Token right_symbol){
    switch (left_symbol.type) {
        // if 1st symbol is var
        case(VAR_ID):
            // 2nd symbol is var
            if (right_symbol.type == VAR_ID){
                fprintf(stdout, "SETCHAR LF@%s LF@%s LF@%s\n", var.value.string.string, left_symbol.value.string.string, right_symbol.value.string.string);
            }
            // 2nd symbol is integer
            else if(right_symbol.type == INTEGER){
                fprintf(stdout, "SETCHAR LF@%s LF@%s int@%d\n", var.value.string.string, left_symbol.value.string.string, right_symbol.value.integer_value);
            }
            else{
                return;
            }
            break;
        // if 1st symbol string
        case(STRING):
            // 2nd symbol is var
            if (right_symbol.type == VAR_ID){
                fprintf(stdout, "SETCHAR LF@%s string@%s LF@%s\n", var.value.string.string, left_symbol.value.string.string, right_symbol.value.string.string);
            }
            // 2nd symbol is integer
            else if(right_symbol.type == INTEGER){
                fprintf(stdout, "SETCHAR LF@%s string@%s int@%d\n", var.value.string.string, left_symbol.value.string.string, right_symbol.value.integer_value);
            }
            else{
                return;
            }
            break;
        default:
            return;
    }
}

// generates relational instructions as part of condition
void gen_condition(Token_type type){
    
    switch (type) {
        case(MORE):
            fprintf(stdout, "GTS\n");
            break;
        case(LESS):
            fprintf(stdout, "LTS\n");
            break;
        case(EQUALS):
            fprintf(stdout, "EQS\n");
            break;
        case(NOT_EQUALS):
            fprintf(stdout, "EQS\n");
            fprintf(stdout, "NOTS\n");
            break;
        // MORE/LESS OR EQUALS needs to be done with 3-address code
        case(MORE_OR_EQUALS):
            fprintf(stdout, "POPS GF@EXP_R\n");
            fprintf(stdout, "POPS GF@EXP_L\n");
            fprintf(stdout, "EQ GF@RESULT_L GF@EXP_L GF@EXP_R\n");
            fprintf(stdout, "GT GF@RESULT_R GF@EXP_L GF@EXP_R\n");
            fprintf(stdout, "OR GF@RESULT_L GF@RESULT_L GF@RESULT_R\n");
            fprintf(stdout, "PUSHS GF@RESULT_L\n");
            break;
        case(LESS_OR_EQUALS):
            fprintf(stdout, "POPS GF@EXP_L\n");
            fprintf(stdout, "POPS GF@EXP_R\n");
            fprintf(stdout, "EQ GF@RESULT_L GF@EXP_L GF@EXP_R\n");
            fprintf(stdout, "LT GF@RESULT_R GF@EXP_L GF@EXP_R\n");
            fprintf(stdout, "OR GF@RESULT_L GF@RESULT_L GF@RESULT_R\n");
            fprintf(stdout, "PUSHS GF@RESULT_L\n");
            break;
        default:
            fprintf(stderr, "\n!\nPRINT CONDITION ERROR - WRONG TYPE\n!\n");
            return;
    }
}

// generates start of loop
void gen_loop_start(){
    // condition should be generated before
    // stack should contain one value so one more is needed
    fprintf(stdout, "PUSHS bool@true\n");
    fprintf(stdout, "JUMPIFNEQS $END_LOOP%d\n", loop_i);
    fprintf(stdout, "LABEL $LOOP%d\n", loop_i);
}

// generates end of loop
void gen_loop_end(){
    fprintf(stdout, "JUMP $LOOP%d\n", loop_i);
    fprintf(stdout, "LABEL $END_LOOP%d\n", loop_i);
    // clear stack
    fprintf(stdout, "CLEARS\n");
    // iterate loop counter for label
    loop_i++;
}

// generates start of if
void gen_if_start(){
    fprintf(stdout, "PUSHS bool@true\n");
    fprintf(stdout, "JUMPIFEQS $IF%d\n", if_i);
    fprintf(stdout, "JUMP $ELSE%d\n", if_i);
    fprintf(stdout, "LABEL $IF%i\n", if_i);
    // create new local frame
    fprintf(stdout, "CREATEFRAME\n");
    fprintf(stdout, "PUSHFRAME\n");
}

// generates else
void gen_if_else(){
    fprintf(stdout, "JUMP $END_IF%d\n", if_i);
    fprintf(stdout, "LABEL $ELSE%d\n", if_i);
    // create new local frame
    fprintf(stdout, "CREATEFRAME\n");
    fprintf(stdout, "PUSHFRAME\n");
}

// generates end of if
void gen_if_end(){
    fprintf(stdout, "LABEL $END_IF%d\n", if_i);
    fprintf(stdout, "POPFRAME\n");
    // clear stack
    fprintf(stdout, "CLEARS\n");
    // iterate if counter for label
    if_i++;
}

// generates built-in write function
void gen_write(Token var){
    switch (var.type) {
        case(STRING):
            fprintf(stdout, "WRITE string@%s\n", var.value.string.string);
            break;
        case(VAR_ID):
            fprintf(stdout, "WRITE LF@%s\n", var.value.string.string);
            break;
        case(INTEGER):
            fprintf(stdout, "WRITE int@%d\n", var.value.integer_value);
            break;
        case(FLOAT):
            fprintf(stdout, "WRITE float@%a\n", var.value.decimal_value);
            break;
        default:
            fprintf(stderr, "\n!\nWRITE ERROR - WRONG TYPE\n!\n");
            return;
    }
}

// generates built-in read function
void gen_read(char* id, char* type){
    fprintf(stdout, "READ LF@%s %s\n", id, type);
}

// generates built-in float2int function
void gen_float2int(Token var){
    switch (var.type) {
        case(VAR_ID):
            fprintf(stdout, "PUSHS LF@%s\n", var.value.string.string);
            fprintf(stdout, "FLOAT2INTS\n");
            break;
        case(FLOAT):
            fprintf(stdout, "PUSHS float@%a\n", var.value.decimal_value);
            fprintf(stdout, "FLOAT2INTS\n");
            break;
        default:
            return;
    }
}

// generates built-in float2ints stack function
void gen_float2ints(){
    fprintf(stdout, "FLOAT2INTS\n");
}

// generates built-in int2float function
void gen_int2float(Token var){
    switch (var.type) {
        case(VAR_ID):
            fprintf(stdout, "PUSHS LF%s\n", var.value.string.string);
            fprintf(stdout, "INT2FLOATS\n");
            break;
        case(INTEGER):
            fprintf(stdout, "PUSHS float%a", var.value.decimal_value);
            fprintf(stdout, "INT2FLOATS\n");
            break;
        default:
            return;
    }
}

// generates built-in int2floats stack function
void gen_int2floats(){
    fprintf(stdout, "INT2FLOATS\n");
}

// does not work
void gen_substring(Token var, Token index_from, Token index_to){
    fprintf(stdout, "lol substring\n");
    fprintf(stdout, "%s\n", var.value.string.string);
    fprintf(stdout, "%s\n", index_from.value.string.string);
    fprintf(stdout, "%s\n", index_to.value.string.string);
    fprintf(stdout, "lol\n");
}

// generates start of function
void gen_function_start(char* func_name, Token_type returns){
    // if code gets to the function without calling it just jump to its end
    fprintf(stdout, "JUMP $FUNC_JUMP%d\n", func_i);
    // soo, label is function name and anchor where IFJcode22 interpret start function
    fprintf(stdout, "LABEL $%s\n", func_name);
    // we start by pushing Temporary Frame to Local Frame thus accessing input arguments
    fprintf(stdout, "PUSHFRAME\n");
    // now we need to define return variables from this piece of garbage structure
    if (returns != EMPTY_TOKEN){
        fprintf(stdout, "DEFVAR LF@func_ret_val\n");
    }
    // and that should be it
    // function body code should be generated by other gen_ functions
}

// generates end of function
void gen_function_end(ht_item_t symbol){
    // if function has return value, generate its return
    if (symbol.return_val_type != EMPTY_TOKEN){
        fprintf(stdout, "POPS LF@func_ret_val\n");
    }
    // after function body we just clean up by
    fprintf(stdout, "POPFRAME\n");
    fprintf(stdout, "CLEARS\n");
    fprintf(stdout, "RETURN\n");
    // and create jump label
    fprintf(stdout, "LABEL $FUNC_JUMP%d\n", func_i);
    func_i++;
}

// generates header of user function
void gen_function_input_param(char* param_name, Token param_val){
    // create new frame
    fprintf(stdout, "CREATEFRAME\n");
    fprintf(stdout, "DEFVAR TF@%s\n", param_name);
    fprintf(stdout, "MOVE TF@%s ", param_name);
    if ( param_val.type == VAR_ID ){
        fprintf(stdout, "LF@%s\n", param_val.value.string.string);
    }
    else if ( param_val.type == INTEGER){
        fprintf(stdout, "int@%d\n", param_val.value.integer_value);
    }
    else if ( param_val.type == FLOAT ){
        fprintf(stdout, "float@%a\n", param_val.value.decimal_value);
    }
    else if ( param_val.type == STRING ){
        fprintf(stdout, "string@%s\n", param_val.value.string.string);
    }
    else{
        fprintf(stderr, "\n!\nGEN FUNC CALL - WRONG TYPE\n!\n");
        return;
    }
}

// generates call of user function
void gen_function_call(char* func_name){
    fprintf(stdout, "CALL $%s\n", func_name);
}

// generates exit instruction
void gen_exit(){
    fprintf(stdout, "POPS GF@VARstring\n");
    fprintf(stdout, "EXIT GF@VARstring\n");
}
