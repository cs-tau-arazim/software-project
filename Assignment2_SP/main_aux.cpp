#include <stdlib.h>// Mat
#include "main_aux.h"
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <string.h>



/*void fgets_fixed(char* str) {
	fgets (str, sizeof(str), stdin);
	if (str == NULL) {
		exit(10);
	}
	strtok(str, "\n");

}*/





int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

int cmpTupleDI (const void * tupA, const void * tupB)
{

  TupleDI *iTupA = (TupleDI *)tupA;
  TupleDI *iTupB = (TupleDI *)tupB;

  return ( iTupB->a - iTupA->a );
}


void free_3d_int(int ***data, size_t xlen, size_t ylen)
{
    size_t i, j;

    for (i=0; i < xlen; ++i) {
        if (data[i] != NULL) {
            for (j=0; j < ylen; ++j)
                free(data[i][j]);
            free(data[i]);
        }
    }
    free(data);
}

void free_3d_double(double ***data, size_t xlen, size_t ylen)
{
    size_t i, j;

    for (i=0; i < xlen; ++i) {
        if (data[i] != NULL) {
            for (j=0; j < ylen; ++j)
                free(data[i][j]);
            free(data[i]);
        }
    }
    free(data);
}



int compareHits (const void * a, const void * b)
{
	const int *a_ = *(const int **)a;
	const int *b_ = *(const int **)b;
	if (a_[1] - b_[1] == 0)
		return ( a_[0] - b_[0] );
	return ( b_[1] - a_[1] );
}


int ***alloc_3d_int(size_t xlen, size_t ylen, size_t zlen)
{
    int ***p;
    size_t i, j;

    if ((p = (int***)malloc(xlen * sizeof *p)) == NULL) {
        perror("malloc 1");
        return NULL;
    }

    for (i=0; i < xlen; ++i)
        p[i] = NULL;

    for (i=0; i < xlen; ++i)
        if ((p[i] = (int**)malloc(ylen * sizeof *p[i])) == NULL) {
            perror("malloc 2");
            free_3d_int(p, xlen, ylen);
            return NULL;
        }

    for (i=0; i < xlen; ++i)
        for (j=0; j < ylen; ++j)
            p[i][j] = NULL;

    for (i=0; i < xlen; ++i)
        for (j=0; j < ylen; ++j)
            if ((p[i][j] =(int*) malloc(zlen * sizeof *p[i][j])) == NULL) {
                perror("malloc 3");
                free_3d_int(p, xlen, ylen);
                return NULL;
            }

    return p;
}

double ***alloc_3d_double(size_t xlen, size_t ylen, size_t zlen)
{
    double ***p;
    size_t i, j;

    if ((p = (double***)malloc(xlen * sizeof *p)) == NULL) {
        perror("malloc 1");
        return NULL;
    }

    for (i=0; i < xlen; ++i)
        p[i] = NULL;

    for (i=0; i < xlen; ++i)
        if ((p[i] = (double**)malloc(ylen * sizeof *p[i])) == NULL) {
            perror("malloc 2");
            free_3d_double(p, xlen, ylen);
            return NULL;
        }

    for (i=0; i < xlen; ++i)
        for (j=0; j < ylen; ++j)
            p[i][j] = NULL;

    for (i=0; i < xlen; ++i)
        for (j=0; j < ylen; ++j)
            if ((p[i][j] =(double*) malloc(zlen * sizeof *p[i][j])) == NULL) {
                perror("malloc 3");
                free_3d_double(p, xlen, ylen);
                return NULL;
            }

    return p;
}

