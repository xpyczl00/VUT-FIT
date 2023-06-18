/**
 * @file ppm.h
 * @author Lukasz Pycz,xpyczl00 ,VUT FIT
 * @date 24/3/2021
 * @brief IJC-DU1, příklad a)
 * @details Header file for ppm.c with declaration definition of 
 *          struct and functions for reading and freeing allocated memory.
 *          Compiled: gcc 9.2
 */
struct ppm {
        unsigned xsize;
        unsigned ysize;
        char data[];    // RGB bajty, celkem 3*xsize*ysize
    };

struct ppm * ppm_read(const char * filename);
void ppm_free(struct ppm *p);