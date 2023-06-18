/**
 * @file steg-decode.c
 * @author Lukasz Pycz,xpyczl00 ,VUT FIT
 * @date 24/3/2021
 * @brief IJC-DU1, příklad a)
 * @details Decoding stored picture data and printing the decoded string output message
 *          Compiled: gcc 9.2
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "ppm.h"
#include "error.h"
#include "eratosthenes.h"
#include "bitset.h"
#define MAX 3*8000*8000
#define BIT 8
 
int main(int argc,char *argv[]){
    if(argc < 2 || argc > 2)
    {   warning_msg("Invalid file format \n");
        return 0;
    }
    struct ppm *pic = ppm_read(argv[1]);
    if (pic == NULL) {
        error_exit("File cannot be opened\n");
    }

    bitset_alloc(prime_array,MAX);
    eratos(prime_array);

    bitset_index_t prime_number=23;
    bitset_index_t array_size=bitset_size(prime_array);
    char message[2]={'\0'};
    unsigned bit_pos=0;

    while(prime_number < array_size){
        if(bitset_getbit(prime_array,prime_number)==0){
            
            bitset_setbit(message,bit_pos,(pic->data[prime_number]&1));
            bit_pos++;
        
            if(message[1]=='\0'&&!(bit_pos%BIT)){
                printf("\n");
                break;
            }
            else if(!(bit_pos%BIT)){ 
                printf("%c",message[1]);
                bit_pos=0;
            }
        }
        prime_number++;  
    }
    bitset_free(prime_array);
    ppm_free(pic);
   
    return 0;
}
