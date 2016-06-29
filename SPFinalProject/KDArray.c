#include "KDArray.h"
#include "SPPoint.h"
#include <stdlib.h>
#include <assert.h>

/** Type for defining the kdArray **/
struct kd_array_t{
	SPPoint* points;
	int* data;
	int size;
	int dim;
};

int get(KDArray kdArr, int i, int j);
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
	newKDArray->data = (int*)malloc(size*dim*sizeof(int));
	if (newKDArray->data == NULL)
		return NULL;
	newKDArray->points = (SPPoint*)malloc(size*sizeof(SPPoint));
	if (newKDArray->points == NULL)
		return NULL;
	for (i = 0 ; i < size ; i++)
	{
		newKDArray->points[i] = arr[i];
	}

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
			set(newKDArray, i, j, (int)ithCoor[j][1]);
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
 * @assert kdLeft == NULL && kdRight == NULL
 * does nothing if kdArr is NULL or coor < 0 or allocation failed
 */
void kdArraysplit (KDArray kdArr, int coor, KDArray kdLeft, KDArray kdRight) //TODO need to free mem when allocation fails?
{
	int i, j, k, size, dim, sizeL, sizeR; 
	int* x, mapL, mapR;
	SPPoint*  p, pL, pR;

	assert(kdLeft == NULL && kdRight == NULL);
	if (kdArr == NULL ||  || coor < 0)
		return;
	size = kdArr->size; 
	dim = kdArr->dim; 
	sizeL = (size+1)/2;
	sizeR = size/2; 
	p = kdArr->points;
	
	x = (int*)malloc(size*sizeof(int));
	if (x == NULL)
		return;

	// x[i] = 1 iff the ith point is in the right sub tree
	for (i = sizeL; i < size ; i++)
	{
		x[get(kdArr, coor, i)] = 1;
	}

	pL = (int*)malloc(sizeL*sizeof(int));
	if (pL == NULL)
		return;
	pR = (int*)malloc(sizeR*sizeof(int));
	if (pR == NULL)
		return;
	mapL = (int*)malloc(size*sizeof(int));
	if (mapL == NULL)
		return;
	mapR = (int*)malloc(size*sizeof(int));
	if (mapR == NULL)
		return;

	// comute the mapping from point index in kdArr to point index in kdLeft
	j = 0;
	for (i = 0 ; i < sizeL ; i++)
	{
		while (x[j] == 1)
		{
			mapL[j] = -1;
			j++;
		}
		pL[i] = p[j];
		mapL[j] = i;
		j++;
	}

	// comute the mapping from point index in kdArr to point index in kdRight
	j = 0;
	for (i = 0 ; i < sizeR ; i++)
	{
		while (x[j] == 0)
		{
			mapR[j] = -1;
			j++;
		}
		pR[i] = p[j];
		mapR[j] = i;
		j++;
	}
	free(x);

	kdLeft = (KDArray) malloc (sizeof (*kdLeft));
	if (kdLeft == NULL)
		return NULL;

	kdRight = (KDArray) malloc (sizeof (*kdRight));
	if (kdRight == NULL)
		return NULL;

	kdLeft->dim = dim;
	kdRight->dim = dim;

	kdLeft->size = sizeL;
	kdRight->size = sizeR;

	kdLeft->points = pL;
	kdRight->points = pR;

	kdLeft->data = (int*)malloc(sizeL*dim*sizeof(int));
	kdRight->data = (int*)malloc(sizeR*dim*sizeof(int));

	// create kdLeft using mapL
	for (k = 0; k < dim ; ++k)
	{
		j = 0;
		for (i = 0 ; i < sizeL ; i++)
		{
			int curr = get(kdArr, k, j);
			while (mapL(curr) == -1)
			{	
				j++;
				curr = get(kdArr, k, j);
			}
			set(kdLeft, k, i, mapL(curr));
		}
	}
	free(mapL);

	// create kdRight using mapR
	for (k = 0; k <  dim ; ++k)
	{
		j = 0;
		for (i = 0 ; i < sizeR ; i++)
		{
			int curr = get(kdArr, k, j);
			while (mapR(curr) == -1)
			{	
				j++;
				curr = get(kdArr, k, j);
			}
			set(kdRight, k, i, mapR(curr));
		}
	}
	free(mapR);

	kdArraydestroy(kdArr);
}

void kdArraydestroy (KDArray kdArr)
{
	free(kdArr->data);
	free(kdArr->points);
	free(kdArr);
}

int get(KDArray kdArr, int i, int j)
{
	return kdArr->data[i*kdArr->size + j];
}

void set(KDArray kdArr, int i, int j, int val)
{
	kdArr->data[i*kdArr->size + j] = val;
}

int cmpCoor (const void * point1, const void * point2)
{
	double *p1 = (double *)point1;
	double *p2 = (double *)point2;
	return (int)(p1[0] - p2[0]); // TODO check if inverse
}


