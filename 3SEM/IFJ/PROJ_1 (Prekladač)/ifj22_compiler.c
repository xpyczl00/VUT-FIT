/**
 * Project: Compiler for IFJ22
 *
 * @brief Main file
 *
 * @author Matej Koreň <xkoren10
 * @file ifj22_compiler.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "parser.h"

/**
 * @brief Error handling.
 *
 * @return EXIT_SUCCESS (0), if main program was successful, else appropriate error message.
 */
int print_error(int error_code)
{

	switch (error_code)
	{

	case ERROR_OK:
		return error_code;
	case ERROR_LEXICAL_ANALISYS:
		fprintf(stderr, "Chyba pri lexikalnej analyze\n");
		return error_code;
	case ERROR_SYNTAX_ANALYSIS:
		fprintf(stderr, "Chyba pri syntaktickej analyze\n");
		return error_code;
	case ERROR_SEMANTIC_FUNCTION:
		fprintf(stderr, "Chyba pri semantickej analyze\n");
		return error_code;
	case ERROR_SEMANTIC_PARAMETERS:
		fprintf(stderr, "Chyba pri semantickej analyze\n");
		return error_code;
	case ERROR_SEMANTIC_UNDEF_VARIABLE:
		fprintf(stderr, "Chyba pri semantickej analyze\n");
		return error_code;
	case ERROR_SEMANTIC_EXPRESSION:
		fprintf(stderr, "Chyba pri semantickej analyze\n");
		return error_code;
	case ERROR_SEMANTIC_TYPE_COMP:
		fprintf(stderr, "Chyba pri semantickej analyze\n");
		return error_code;
	case ERROR_SEMANTIC_OTHER:
		fprintf(stderr, "Chyba pri semantickej analyze\n");
		return error_code;
	case ERROR_INTERN:
		fprintf(stderr, "Interna chyba prekladaca\n");
		return error_code;
	case ERROR_IDK:
		fprintf(stderr, "Neocakavana chyba\n");
		return error_code;
	default:
		fprintf(stderr, "Neocakavana chyba\n");
		return error_code;
	}
	return ERROR_OK;
}

/**
 * @brief Main function.
 *
 * @return Error handlig function.
 */
int main()
{
	FILE *source_file;

	if (isatty(STDIN_FILENO)) // Empty stdin
	{
		fprintf(stderr, "Missing input file\n");
		return 1;
	}
	source_file = stdin;

	set_source(source_file);

	int output = 0;
	output = Program();

	return print_error(output);
}