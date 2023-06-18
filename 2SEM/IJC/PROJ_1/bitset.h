/**
 * @file bitset.h
 * @author Lukasz Pycz,xpyczl00 ,VUT FIT
 * @date 24/3/2021
 * @brief IJC-DU1, příklad a)
 * @details Header file with macro definitions and inline functions for bitset array.
 *          Compiled: gcc 9.2
 */

#include <limits.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "error.h"

typedef unsigned long bitset_index_t;
//Type of bit array index 
typedef bitset_index_t* bitset_t;
//Type of bit array


#define Byte_Size (bitset_index_t) (sizeof(bitset_index_t)
//size of unsigned long in bytes
#define Bit_Size (bitset_index_t) (sizeof(bitset_index_t)*CHAR_BIT) 
//size of unsigned long in bits


#define bitset_create(array,size_in_bits)\
    bitset_index_t array[size_in_bits/Bit_Size+((size_in_bits%Bit_Size)?2:1)] = {size_in_bits,0};\
    static_assert(size_in_bits>0 && size_in_bits>=Bit_Size,"size of array must be greater than 0 and cant be less than size of unsigned long ")          //zda je velkost vacsi ako 0 a zaroven vacsi ako velkost prvku pola
/**
 * @param array bitset name
 * @param size_in_bits bit size of array
 * @brief inicialization of dynamic bitset array
 */
   


#define bitset_alloc(array,size_in_bits)\
    assert(size_in_bits>0 );\
    assert(size_in_bits>=Bit_Size );\
    bitset_t array = (bitset_t)calloc(size_in_bits/Bit_Size+\
    ((size_in_bits%Bit_Size)?2:1),sizeof(unsigned long));\
    if(array==NULL){\
        error_exit("bitset_alloc: Error in allocation \n"); \
    };\
    array[0] = size_in_bits;
/**
 * @param array bitset name
 * @param size_in_bits bit size of array
 * @brief memory allocation of dynamic bit array
 */



    
    
#ifndef USE_INLINE
//INLINE FUNCTIONS

#define bitset_free(array)\
    free(array)

/**
 * @param array bit array name
 * @brief freeing memory of dynamic bit array
 */

#define bitset_size(array)\
    (array[0])
/**
 * @param array bit array name
 * @brief return size of array from index 0
*/


#define bitset_setbit(array, index, value)\
    if(value != 0) ((array[(index / Bit_Size)+1]) |= (1UL << (index%Bit_Size)));\
    else (array[(index / Bit_Size)+1]) &= ~(1UL << (index%Bit_Size));   
/**
 * @param array bit array name
 * @param index wanted position of array
 * @param value value of bit to set od index
 * @brief sets a value on index of bit array
*/

#define bitset_getbit(array, index) \
    ((array[(index / Bit_Size)+1] >> (index%Bit_Size)) & 1UL)
/**
 * @param array bit array name
 * @param index wanted position of array
 * @param value value of bit returned from the array with index
 * @brief returns bit from written index
*/

#else

//MACRO FUNTIONS

inline void bitset_free(bitset_t array){
    free(array);
}
inline unsigned long bitset_size(bitset_t array){
    return array[0];
}
 

inline void bitset_setbit(bitset_t array, bitset_index_t index, unsigned value)
{
    if(value != 0)
        (array[(index / Bit_Size)+1]) = (1UL << (index% Bit_Size)) | (array[(index / Bit_Size)+1]);
    else  
        (array[(index / Bit_Size)+1]) = ~(1UL << (index%Bit_Size)) & (array[(index / Bit_Size)+1]);
}

inline unsigned bitset_getbit(bitset_t array, bitset_index_t index)
{
    return ((array[(index / Bit_Size)+1] >> (index%Bit_Size)) & 1UL);
}
#endif