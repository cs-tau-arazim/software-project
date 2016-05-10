#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_


/*
 * void free_2d_double(double **data, size_t xlen):
 *
 * receives pointer to a 2d array of doubles and the rows size,
 * and frees the array.
 */
void free_2d_double(double **data, size_t xlen);


/*
 * void free_2d_int(int **data, size_t xlen):
 *
 * receives pointer to a 2d array of integers and the rows size,
 * and frees the array.
 */
void free_2d_int(int **data, size_t xlen);


/*
 * void free_3d_sift(double **data, size_t xlen, size_t ylen):
 *
 * receives pointer to a 3d array of doubles and the rows size, array of columns sizes,
 * and frees the array.
 */
void free_3d_sift(double ***data, size_t xlen, int* ylens);


/*
 * void free_3d_int(int **data, size_t xlen, size_t ylen):
 *
 * receives pointer to a 3d array of integers and the rows size, columns size,
 * and frees the array.
 */
void free_3d_int(int ***data, size_t xlen, size_t ylen);

/*
 * Compare two TupleDI structs by double value a.
* Tie breaker - index integer value b.
*/
int cmpTupleDI (const void * tupA, const void * tupB);

/*
 *  Compare two TupleDI structs by double value a, in reverse order.
* Tie breaker - index integer value b.
*/
int inverseCmpTupleDI (const void * tupA, const void * tupB);


// TupleDI- Tuple containing Double and Integer
struct TupleDI{
   double a;
   int b;
};




#endif
