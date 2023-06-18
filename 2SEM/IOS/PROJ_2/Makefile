# Makefile
# Riešenie IOS-DU2, 2.5.2022
# Autor: Lukasz Pycz, xpyczl00


	
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic

all: proj2

proj2.o: proj2.c
	gcc $(CFLAGS) -c proj2.c -o proj2.o

proj2: proj2.o
	gcc $(CFLAGS) proj2.o -o proj2 -lpthread

pack:
	zip proj2.zip proj2.c Makefile

clean:
	rm -f proj2
	rm -f *.o
	rm -f proj2.zip