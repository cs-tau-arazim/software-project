#include "KDArray.h"
#include "SPPoint.h"
#include <stdlib.h>
#include <assert.h>

/** Type for defining the kdArray **/
struct kd_array_t{
	double* data;
	int size;
	int dim;
};

void set(KDArray kdArr, int i, int j, double val);
int cmpCoor (const void * point1, const void * point2);



/**
 * Initializes the kd-array with the data given by arr
 *
 * @assert dim = point->dim for each point in arr;
 * @return
 * NULL if memory allocation failed or arr == NULL or size < 1 or dim < 1
 * the new KDArray otherwise
 */
KDArray kdArrayinit (SPPoint* arr, int size, int dim)
{
	KDArray newKDArray;
	int i, j;
	if (arr == NULL || size < 1 || dim < 1)
		return NULL;

	newKDArray = (KDArray) malloc (sizeof (*newKDArray));
	if (newKDArray == NULL)
		return NULL;

	newKDArray->dim = dim;
	newKDArray->size = size;

	for (i = 0 ; i < dim ; i++)
	{
		double** ithCoor = (double**) malloc (size*sizeof (double*));
		if (ithCoor == NULL)
			return NULL;

		for (j = 0 ; j < size ; j ++)
		{
			ithCoor[j] = (double*) malloc (2*sizeof (double));
			if (ithCoor == NULL)
				return NULL;

			ithCoor[j][0] = spPointGetAxisCoor(arr[j], i);
			ithCoor[j][1] = (double)j;

		}
		qsort(ithCoor, size, sizeof(ithCoor[0]),cmpCoor );
		for (j = 0 ; j < size ; j ++)
		{
			set(newKDArray, i, j, ithCoor[j][1]);
		}
		for (j = 0 ; j < size ; j ++)
		{
			free(ithCoor[j]);
		}
		free(ithCoor);
	}

	return newKDArray;
}

/**
 * Stores two KDArrays in (kdLeft, kdRight) such that the first
 * half points with respect to the coordinate coor are in kdLeft,
 * and the rest of the points are in kdRight.
 *
 * does nothing if any of the params is NULL or coor < 1
 */
void kdArraysplit (KDArray kdArr, int coor, KDArray kdLeft, KDArray kdRight)
{

}

double get(KDArray kdArr, int i, int j)
{
	return kdArr->data[i*kdArr->size + j];
}

void set(KDArray kdArr, int i, int j, double val)
{
	kdArr->data[i*kdArr->size + j] = val;
}

int cmpCoor (const void * point1, const void * point2)
{
	double *p1 = (double *)point1;
	double *p2 = (double *)point2;
	return (int)(p1[0] - p2[0]); // TODO check if inverse
}


