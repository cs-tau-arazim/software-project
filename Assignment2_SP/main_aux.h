#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

void free_data(int ***data, size_t xlen, size_t ylen);

void free_data(double ***data, size_t xlen, size_t ylen);

int compareHits (const void * a, const void * b);

int cmpfunc (const void * a, const void * b);

int cmpTupleDI (const void * tupA, const void * tupB);

struct TupleDI{
   double a;
   int b;
};

#endif
