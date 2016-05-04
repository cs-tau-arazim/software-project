#include <opencv2/core.hpp>// Mat


int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}


int cmpTupleDI (const void * tupA, const void * tupB)
{
   return ( *(int*)tupA.a - *(int*)tupB.a );
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

struct TupleDI{
   double a;
   int b;
};



