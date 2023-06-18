/**
 * @file bitset.c
 * @author Lukasz Pycz,xpyczl00 ,VUT FIT
 * @date 24/3/2021
 * @brief IJC-DU1, příklad a)
 * @details Inline function extern definitions
 *          Compiled: gcc 9.2
 */
#include "bitset.h"

#ifdef USE_INLINE

extern void bitset_free(bitset_t array);
extern unsigned long bitset_size(bitset_t array);
extern void bitset_setbit(bitset_t array,bitset_index_t index, unsigned vaule);
extern unsigned bitset_getbit(bitset_t array,bitset_index_t index);


#endif