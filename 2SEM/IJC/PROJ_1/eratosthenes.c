/**
 * @file erathostenes.c
 * @author Lukasz Pycz,xpyczl00 ,VUT FIT
 * @date 24/3/2021
 * @brief IJC-DU1, příklad a)
 * @details Implementation details for function Eratosthenes sieve.
 *          Compiled: gcc 9.2
 */
#include "eratosthenes.h"
#include <math.h>
#include <stdio.h>

void eratos(bitset_t array){

    bitset_setbit(array,0,1); 
    bitset_setbit(array,1,1); 

    unsigned long array_size = bitset_size(array);
    unsigned long root = sqrt(array_size);

    for(bitset_index_t i = 2; i < root; i++){
        if(bitset_getbit(array,i) == 0){
            for(bitset_index_t n = i*i; n <array_size; n+=i){
                if(array_size==n)
                    printf("Memory leak\n");
                bitset_setbit(array,n,1);
            }
        }    
    }

}