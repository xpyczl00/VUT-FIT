/**
* Project - Compiler for IFJ22
* 
* @brief Error definitions.
* 
* @author Matej Koreň <xkoren10
* @author Matej Hložek <xhloze02>
* @file error_codes.h
*
**/

#ifndef _ERROR_CODES_H
#define _ERROR_CODES_H

#define ERROR_OK                          0   // najs
#define ERROR_LEXICAL_ANALISYS            1   // chybná struktura aktuálního lexému
#define ERROR_SYNTAX_ANALYSIS             2   // chybná syntaxe programu, chybějící hlavička, atp.
#define ERROR_SEMANTIC_FUNCTION           3   // nedefinovaná funkce, pokus o redefinice funkce
#define ERROR_SEMANTIC_PARAMETERS         4   // špatný počet/typ parametrů u volání funkce či typ návratové hodnoty z funkce
#define ERROR_SEMANTIC_UNDEF_VARIABLE     5   // použití nedefinované proměnné
#define ERROR_SEMANTIC_EXPRESSION         6   // chybějící/přebývající výraz v příkazu návratu z funkce.
#define ERROR_SEMANTIC_TYPE_COMP          7   // chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech
#define ERROR_SEMANTIC_OTHER              8   // ostatní sémantické chyby
#define ERROR_INTERN                      99  // interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti atd.)

#define ERROR_IDK                         -1  //temp

#endif  /* ERROR_CODES_H*/