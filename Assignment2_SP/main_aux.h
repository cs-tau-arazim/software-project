#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

void free_2d_double(double **data, size_t xlen);

void free_2d_int(int **data, size_t xlen);

void free_3d_double(double ***data, size_t xlen, size_t ylen);

void free_3d_int(int ***data, size_t xlen, size_t ylen);

int compareHits (const void * a, const void * b);

int cmpfunc (const void * a, const void * b);

int cmpTupleDI (const void * tupA, const void * tupB);

struct TupleDI{
   double a;
   int b;
};

#endif
