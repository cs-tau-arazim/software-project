/*
 * KDArray.h
 *
 *  Created on: Jun 28, 2016
 *      Author: tomsegal
 */


#ifndef KDARRAY_H_
#define KDARRAY_H_

#include "SPPoint.h"
#include "SPLogger.h"

/** Type for defining the KDArray **/
typedef struct kd_array_t* KDArray;

/**
 * Initializes an empty kd-array with
 *
 * @return
 * NULL if memory allocation failed
 * the new KDArray otherwise
 */
KDArray kdArrayInitEmpty ();

/**
 * Initializes the kd-array with the data given by arr
 *
 * @param arr - the array of SPPoints
 * @param size - the size of arr
 * @param dim - the dimension of the points in arr
 *
 * @assert dim = point->dim for each point in arr;
 *
 * @return
 * NULL if memory allocation failed or arr == NULL or size < 1 or dim < 1
 * the new KDArray otherwise
 */
KDArray kdArrayInit (SPPoint* arr, int size, int dim);

/**
 * Stores two KDArrays in (kdLeft, kdRight) such that the first
 * half points with respect to the coordinate coor are in kdLeft,
 * and the rest of the points are in kdRight.
 *
 * @assert(kdArr->size > 1);
 *
 * does nothing if one of the arrays is NULL or coor < 0 or allocation failed
 */
void kdArraySplit (KDArray kdArr, int coor, KDArray kdLeft, KDArray kdRight);

/**
 *  Free all memory allocation associated with kdArr,
 * 	if kdArr is NULL nothing happens.
 */
void kdArrayDestroy (KDArray kdArr);

/**
 * returns the points stored in kdArr
 */
SPPoint* kdArrayGetPoints (KDArray kdArr);

/**
 * returns -1 if kdArr == NULL || i < 0 || j < 0 || i >= kdArr->dim || j >= kdArr->size
 * otherwise - kdArr[i][j]
 */
int kdArrayGet(KDArray kdArr, int i, int j);

/**
 * returns 0 if kdArr == NULL
 * otherwise - the size of the kdArr
 */
int kdArrayGetSize(KDArray kdArr);

/**
 * prints the array for debuging
 */
void printArray (KDArray kdArr);



#endif /* KDARRAY_H_ */
