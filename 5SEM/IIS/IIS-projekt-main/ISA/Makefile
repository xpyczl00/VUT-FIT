# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=gnu99 -Wall -Wextra 

# Source files
SRC = dns.c   # Your resolver source file

# Object files (auto-generated from source files)
OBJ = $(SRC:.c=.o)

# Executable names
TARGET = dns

# Default target
all: $(TARGET)

# Compile the object files for resolver and receiver
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link the resolver executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)


test: all
	@echo -e "\033[0;32mTEST 1\033[0m"
	./dns -r -s kazi.fit.vutbr.cz www.fit.vut.cz
	@echo
	@echo -e "\033[0;32mTEST 2\033[0m"
	./dns -r -s kazi.fit.vutbr.cz www.ietf.org
	@echo
	@echo -e "\033[0;32mTEST 3\033[0m"
	./dns -s kazi.fit.vutbr.cz www.fit.vut.cz
	@echo
	@echo -e "\033[0;32mTEST 4\033[0m"
	./dns -s kazi.fit.vutbr.cz www.ietf.org
	@echo
	@echo -e "\033[0;32mTEST 5\033[0m"
	./dns -r -s 8.8.8.8 www.seznam.cz
	@echo
	@echo -e "\033[0;32mTEST 6\033[0m"
	./dns -r -6 -s 8.8.8.8 www.seznam.cz
	@echo
	@echo -e "\033[0;32mTEST 7\033[0m"
	./dns -r -x -s kazi.fit.vutbr.cz 8.8.8.8
	@echo
	@echo -e "\033[0;32mTEST 8\033[0m"
	./dns -r -x -s kazi.fit.vutbr.cz 2001:4860:4860::8888
	@echo
	@echo -e "\033[0;32mTEST 9\033[0m"
	./dns -x -s kazi.fit.vutbr.cz 2001:4860:4860::8888
	@echo
	@echo -e "\033[0;32mTEST 10\033[0m"
	./dns -x -s kazi.fit.vutbr.cz 8.8.4.4
	@echo


# Clean up
clean:
	rm -f $(OBJ) $(TARGET) $(OBJ_RECEIVER) $(TARGET_RECEIVER)
