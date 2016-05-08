#include <stdlib.h>// Mat
#include "main_aux.h"
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <string.h>

/*
 *  Compare two TupleDI structs by double value a.
* Tie breaker - index integer value b.
*/
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

/*
 *  Compare two TupleDI structs by double value a, in reverse order.
* Tie breaker - index integer value b.
*/
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



