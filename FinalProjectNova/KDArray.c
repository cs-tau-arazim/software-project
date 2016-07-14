#include "KDArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/** Type for defining the kdArray **/
struct kd_array_t {
	SPPoint* points;
	int* data;
	int size;
	int dim;
};

void set(KDArray kdArr, int i, int j, int val);
int cmpCoor(const void * point1, const void * point2);

KDArray kdArrayInitEmpty() {
	KDArray newKDArray;
	newKDArray = (KDArray) malloc(sizeof(*newKDArray));
	if (newKDArray == NULL )
		return NULL ;
	return newKDArray;
}

/**
 * Initializes the kd-array with the data given by arr
 *
 * @assert dim = point->dim for each point in arr;
 * @return
 * NULL if memory allocation failed or arr == NULL or size < 1 or dim < 1
 * the new KDArray otherwise
 */
KDArray kdArrayInit(SPPoint* arr, int size, int dim) {
	KDArray newKDArray;
	int i, j;
	if (arr == NULL || size < 1 || dim < 1)
		return NULL ;

	newKDArray = (KDArray) malloc(sizeof(*newKDArray));
	if (newKDArray == NULL )
		return NULL ;

	newKDArray->dim = dim;
	newKDArray->size = size;
	newKDArray->data = (int*) malloc(size * dim * sizeof(int));
	if (newKDArray->data == NULL )
		return NULL ;
	newKDArray->points = (SPPoint*) malloc(size * sizeof(SPPoint));
	if (newKDArray->points == NULL )
		return NULL ;
	for (i = 0; i < size; i++) {
		newKDArray->points[i] = spPointCopy(arr[i]);
		if (newKDArray->points[i] == NULL )
			return NULL ;
	}

	for (i = 0; i < dim; i++) {
		double** ithCoor = (double**) malloc(size * sizeof(double*));
		if (ithCoor == NULL )
			return NULL ;

		for (j = 0; j < size; j++) {
			ithCoor[j] = (double*) malloc(2 * sizeof(double));
			if (ithCoor == NULL )
				return NULL ;

			ithCoor[j][0] = spPointGetAxisCoor(arr[j], i);
			ithCoor[j][1] = (double) j;

		}
		qsort(ithCoor, size, sizeof(ithCoor[0]), cmpCoor);
		for (j = 0; j < size; j++) {
			set(newKDArray, i, j, (int) ithCoor[j][1]);
		}
		for (j = 0; j < size; j++) {
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
void kdArraySplit(KDArray kdArr, int coor, KDArray kdLeft, KDArray kdRight) //TODO need to free mem when allocation fails?
{
	int i, j, k, size, dim, sizeL, sizeR;
	int* x;
	int* mapL;
	int* mapR;
	SPPoint* p;
	SPPoint* pL;
	SPPoint* pR;

	assert(kdArr->size > 1);

	if (kdArr == NULL || kdLeft == NULL || kdRight == NULL || coor < 0)
		return;

	size = kdArr->size;
	dim = kdArr->dim;
	sizeL = (size + 1) / 2;
	sizeR = size / 2;
	p = kdArr->points;

	x = (int*) malloc(size * sizeof(int));
	if (x == NULL )
		return;

	// x[i] = 1 iff the ith point is in the right sub tree
	for (i = sizeL; i < size; i++) {
		x[kdArrayGet(kdArr, coor, i)] = 1;
	}
	for (i = 0; i < sizeL; i++) {
		x[kdArrayGet(kdArr, coor, i)] = 0;
	}

	pL = (SPPoint*) malloc(sizeL * sizeof(SPPoint));
	if (pL == NULL )
		return;
	pR = (SPPoint*) malloc(sizeR * sizeof(SPPoint));
	if (pR == NULL )
		return;
	mapL = (int*) malloc(size * sizeof(int));
	if (mapL == NULL )
		return;
	mapR = (int*) malloc(size * sizeof(int));
	if (mapR == NULL )
		return;

	// compute the mapping from point index in kdArr to point index in kdLeft
	j = 0;
	for (i = 0; i < sizeL; i++) {
		while (x[j] == 1 && j < size) {
			mapL[j] = -1;
			j++;
		}
		pL[i] = spPointCopy(p[j]);
		mapL[j] = i;

		j++;
		while (x[j] == 1 && j < size) {
			mapL[j] = -1;
			j++;
		}
	}

	// compute the mapping from point index in kdArr to point index in kdRight
	j = 0;
	for (i = 0; i < sizeR; i++) {
		while (x[j] == 0 && j < size) {
			mapR[j] = -1;
			j++;
		}
		pR[i] = spPointCopy(p[j]);
		mapR[j] = i;
		j++;
		while (x[j] == 0 && j < size) {
			mapR[j] = -1;
			j++;
		}
	}

	kdLeft->dim = dim;
	kdRight->dim = dim;

	kdLeft->size = sizeL;
	kdRight->size = sizeR;

	kdLeft->points = pL;
	kdRight->points = pR;


	kdLeft->data = (int*) malloc(sizeL * dim * sizeof(int));
	// TODO check if NULL

	kdRight->data = (int*) malloc(sizeR * dim * sizeof(int));
	// TODO check if NULL

	// create kdLeft using mapL
	for (k = 0; k < dim; ++k) {
		j = 0;
		for (i = 0; i < sizeL; i++) {
			int curr = kdArrayGet(kdArr, k, j);
			while (mapL[curr] == -1) {
				j++;
				curr = kdArrayGet(kdArr, k, j);
			}
			set(kdLeft, k, i, mapL[curr]);
			j++;
		}
	}

	// create kdRight using mapR
	for (k = 0; k < dim; ++k) {
		j = 0;
		for (i = 0; i < sizeR; i++) {
			int curr = kdArrayGet(kdArr, k, j);
			while (mapR[curr] == -1) {
				j++;
				curr = kdArrayGet(kdArr, k, j);
			}
			set(kdRight, k, i, mapR[curr]);
			j++;
		}
	}

	free(x);
	free(mapL);

	free(mapR);

	kdArrayDestroy(kdArr);
}

void kdArrayDestroy(KDArray kdArr) {
	int i;
	free(kdArr->data);
	for (i = 0 ; i < kdArr->size ; i++)
		spPointDestroy(kdArr->points[i]);
	free(kdArr->points);
	free(kdArr);
}

SPPoint* kdArrayGetPoints(KDArray kdArr) {
	return kdArr->points;
}

int kdArrayGet(KDArray kdArr, int i, int j) {
	if (kdArr == NULL || i < 0 || j < 0 || i >= kdArr->dim
			|| j >= kdArr->size) {

		return -1;
	}
	return kdArr->data[i * (kdArr->size) + j];
}

int kdArrayGetSize(KDArray kdArr) {
	if (kdArr == NULL )
		return 0;
	return kdArr->size;
}

void set(KDArray kdArr, int i, int j, int val) {
	if (kdArr == NULL || i < 0 || j < 0 || i >= kdArr->dim || j >= kdArr->size)
		return;
	kdArr->data[i * kdArr->size + j] = val;
}

int cmpCoor(const void * point1, const void * point2) {
	double *p1 = *(double **) point1;
	double *p2 = *(double **) point2;
	return (int) (p1[0] - p2[0]);
}

void printArray(KDArray kdArr) {
	int i, j;
	for (i = 0; i < kdArr->dim; i++) {
		for (j = 0; j < kdArr->size; j++) {
			printf("%d ", kdArrayGet(kdArr, i, j));
		}
		printf("\n");
	}
	printf("\n");
}
