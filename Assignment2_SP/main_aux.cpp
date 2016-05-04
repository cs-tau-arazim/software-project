#include <opencv2/core.hpp>// Mat
#include "main_aux.h"



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




void free_data(int ***data, size_t xlen, size_t ylen)
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

void free_data(double ***data, size_t xlen, size_t ylen)
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
	return ( a_[1] - b_[1] );
}




