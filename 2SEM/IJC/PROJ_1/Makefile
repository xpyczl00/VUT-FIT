
#Makefile
#Lukasz Pycz,xpyczl00 ,VUT FIT
# 24/3/2021
#IJC-DU1, příklad a)
# Makefile with run and clean commands
#Compiled: gcc 9.2


CFLAGS=-g -std=c11 -pedantic -Wall -Wextra -O2

all: primes primes-i steg-decode

.PHONY: all clean 

primes: primes.o  error.o eratosthenes.o
	gcc $(CFLAGS) primes.o  error.o eratosthenes.o -o primes -lm

primes.o: primes.c bitset.h
	gcc $(CFLAGS) -c primes.c -o primes.o -lm


primes-i: primes-i.o error.o eratosthenes-i.o 
	gcc $(CFLAGS) -DUSE_INLINE primes-i.o  error.o eratosthenes-i.o -o primes-i -lm

primes-i.o: primes.c bitset.h
	gcc $(CFLAGS) -c -DUSE_INLINE primes.c -o primes-i.o


steg-decode: steg-decode.o eratosthenes.o ppm.o error.O
	gcc $(CFLAGS) steg-decode.o eratosthenes.o ppm.o error.o -o steg-decode -lm

steg-decode.o: steg-decode.c eratosthenes.h ppm.h error.h bitset.h
	gcc $(CFLAGS) -c steg-decode.c -o steg-decode.o


bitset-i.o: bitset.h bitset.c
	gcc $(CFLAGS) -c -DUSE_INLINE bitset.c -o bitset-i.o

error.o: error.c error.h
	gcc $(CFLAGS) -c error.c -o error.o
 
eratosthenes.o: eratosthenes.c
	gcc $(CFLAGS) -c eratosthenes.c -o eratosthenes.o -lm

eratosthenes-i.o: eratosthenes.c
	gcc $(CFLAGS) -c -DUSE_INLINE eratosthenes.c -o eratosthenes-i.o

ppm.o: ppm.c error.h ppm.h
	gcc $(CFLAGS) -c ppm.c -o ppm.o

clean:
	rm -f  primes steg-decode primes-i *.o

run:
	./steg-decode du1-obrazek.ppm
	ulimit -s 65000 && time ./primes	
	ulimit -s 65000 && time ./primes-i	