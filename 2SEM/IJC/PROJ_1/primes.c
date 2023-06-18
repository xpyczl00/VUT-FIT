/**
 * @file primes.c
 * @author Lukasz Pycz,xpyczl00 ,VUT FIT
 * @date 24/3/2021
 * @brief IJC-DU1, příklad a)
 * @details Getting last 10 prime numbers of by using erathostenes sieve function
 *          Compiled: gcc 9.2
 */
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "eratosthenes.h"


#define Number 200000000

int main(){
    
clock_t start;
start = clock();

unsigned long size=Number-1,index=0;
unsigned long prime_array[10]={0};
bitset_create(array,Number);
eratos(array);

while(index!=10){
    if(bitset_getbit(array,size)==0){
        prime_array[index]=size;
        index++;
    }
    size--;
}  
for(int i = 9; i >= 0; i--)
        printf("%ld\n", prime_array[i]);

    printf("Time=%.3g\n",(double)(clock()-start)/CLOCKS_PER_SEC);

    return 0;

}

