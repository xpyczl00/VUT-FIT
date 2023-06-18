CFLAGS=-std=gnu99 -Wall -Wextra
CC=gcc
FILES=symtable.c Tests/test.c Tests/test_util.c

.PHONY: clean 

make: ifj22_compiler.c dyn_string.c scanner.c parser.c symtable.c  expressions.c code_gen.c stack.c stack_table.c
	$(CC) $(CFLAGS) ifj22_compiler.c dyn_string.c scanner.c parser.c  symtable.c expressions.c code_gen.c stack.c stack_table.c -o ifj22_compiler

#---------------------------------------------------------------------------------------
test_scan: Tests/scanner_tests.c dyn_string.c scanner.c
	$(CC) $(CFLAGS) Tests/scanner_tests.c dyn_string.c scanner.c -o scanner_tests
	./scanner_tests
	rm -f scanner_tests
#---------------------------------------------------------------------------------------

#---------------------------------------------------------------------------------------
test_parse: Tests/parser_tests.c dyn_string.c scanner.c expressions.c stack.c symtable.c stack_table.c parser.c
	$(CC) $(CFLAGS) Tests/parser_tests.c dyn_string.c scanner.c expressions.c stack.c symtable.c stack_table.c parser.c code_gen.c -o parser_tests
	./parser_tests
	rm -f parser_tests
#---------------------------------------------------------------------------------------

#---------------------------------------------------------------------------------------
test_symtable: symtable.c Tests/symtable_tests.c 
	$(CC) $(CFLAGS) -Wno-missing-field-initializers symtable.c Tests/symtable_tests.c  -o symtable_test
	@./symtable_test > current-test.out
	@echo "-----------------------------------------------\n------------- Symtable tests ------------"
	@echo "\nTest output differences:"
	@diff -su Tests/ht.out current-test.out
	@rm current-test.out
	@rm symtable_test
#---------------------------------------------------------------------------------------

#---------------------------------------------------------------------------------------
test_expressions: Tests/expressions_tests.c dyn_string.c scanner.c expressions.c stack_table.c code_gen.c stack.c symtable.c
	$(CC) $(CFLAGS) Tests/expressions_tests.c dyn_string.c scanner.c expressions.c stack.c stack_table.c code_gen.c symtable.c -o expressions_test
	./expressions_test
	rm -f expressions_test
#---------------------------------------------------------------------------------------
	
#---------------------------------------------------------------------------------------
test_all: test_scan test_parse  test_expressions
#---------------------------------------------------------------------------------------

clean:
	rm -f *.o 

.PHONY: clean 