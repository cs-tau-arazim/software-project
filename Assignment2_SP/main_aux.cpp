#include <stdlib.h>// Mat
#include "main_aux.h"
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <string.h>


int cmpTupleDI (const void * tupA, const void * tupB)
{

  TupleDI *iTupA = (TupleDI *)tupA;
  TupleDI *iTupB = (TupleDI *)tupB;

  if (( iTupB->a - iTupA->a ) < 0) {
		  return 1;
  }
  else if (( iTupB->a - iTupA->a ) == 0) {
	  return iTupA->b - iTupB->b;
  }
  else {
	  return -1;
  }
}


int inverseCmpTupleDI (const void * tupA, const void * tupB)
{

  TupleDI *iTupA = (TupleDI *)tupA;
  TupleDI *iTupB = (TupleDI *)tupB;

  if (( iTupB->a - iTupA->a ) < 0) {
		  return -1;
  }
  else if (( iTupB->a - iTupA->a ) == 0) {
	  return iTupA->b - iTupB->b;
  }
  else {
	  return 1;
  }
}

/*
 * void free_2d_int(int **data, size_t xlen):
 *
 * receives pointer to a 2d array of integers and the rows size,
 * and frees the array.
 */
void free_2d_int(int **data, size_t xlen)
{
	if (data != NULL) {
		for (size_t i = 0; i < xlen; ++i)
		{
			int* currentIntPtr = data[i];
			free(currentIntPtr);
		}
		free(data);
	}
}


/*
 * void free_2d_double(double **data, size_t xlen):
 *
 * receives pointer to a 2d array of doubles and the rows size,
 * and frees the array.
 */
void free_2d_double(double **data, size_t xlen)
{
	if (data != NULL) {
		for (size_t i = 0; i < xlen; ++i)
		{
			double* currentIntPtr = data[i];
			free(currentIntPtr);
		}
		free(data);
	}
}



/*
 * void free_3d_int(int **data, size_t xlen, size_t ylen):
 *
 * receives pointer to a 3d array of integers and the rows size, columns size,
 * and frees the array.
 */
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


/*
 * void free_3d_double(double **data, size_t xlen, size_t ylen):
 *
 * receives pointer to a 3d array of doubles and the rows size, columns size,
 * and frees the array.
 */
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

/*
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
}*/

