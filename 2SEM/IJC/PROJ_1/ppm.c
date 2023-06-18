/**
 * @file ppm.c
 * @author Lukasz Pycz,xpyczl00 ,VUT FIT
 * @date 24/3/2021
 * @brief IJC-DU1, příklad a)
 * @details Reading data from picture.ppm and storing into struct
 *          Compiled: gcc 9.2
 */
#include <stdio.h>
#include <stdlib.h>
#include "ppm.h"
#include "error.h"
#include "bitset.h"

#define MAX_SIZE 8000*8000*3

void ppm_free(struct ppm *p) {
    free(p);
}

struct ppm * ppm_read(const char * filename){
    FILE* f=fopen(filename,"r");
    if(f==NULL){
        warning_msg("The file is invalid \n");
        return NULL;
    }

    long unsigned X;
    long unsigned Y;
    int colour;

    size_t RGB=fscanf(f, "P6 %lu %lu %d%*c",&X,&Y,&colour);
    unsigned size= X*Y*3;
    
    if(RGB!=3|| colour!=255||size > MAX_SIZE){
        warning_msg("Invalid File Format or Size");
        fclose(f);
        return NULL;
    }
    
    struct ppm *pic = malloc(sizeof(struct pic*)+sizeof(char)*size);
    if(pic == NULL){
        warning_msg("Error while allocating memory");
        fclose(f);
        return NULL;
    }
    pic->xsize=X;
    pic->ysize=Y;
    
    if(fread(pic->data,sizeof(char),size,f)!=3*X*Y){
        warning_msg("Error with file");
        fclose(f);
        ppm_free(pic);
        return NULL;
    }

    fclose(f);
    return pic;
}

