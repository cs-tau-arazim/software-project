/*
 * KDArray.h
 *
 *  Created on: Jun 28, 2016
 *      Author: tomsegal
 */


#ifndef KDARRAY_H_
#define KDARRAY_H_

#include "SPPoint.h"

/** Type for defining the KDArray **/
typedef struct kd_array_t* KDArray;

// TODO doc
KDArray kdArrayInitEmpty ();

/**
 * Initializes the kd-array with the data given by arr
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
 * does nothing if any of the params is NULL or coor < 1
 */
void kdArraySplit (KDArray kdArr, int coor, KDArray kdLeft, KDArray kdRight);

/**
 *  Free all memory allocation associated with kdArr,
 * 	if kdArr is NULL nothing happens.
 */
void kdArrayDestroy (KDArray kdArr);

// TODO doc
SPPoint* kdArrayGetPoints (KDArray kdArr);

// TODO doc
int kdArrayGet(KDArray kdArr, int i, int j);

// TODO doc
int kdArrayGetSize(KDArray kdArr);

void printArray (KDArray kdArr);



#endif /* KDARRAY_H_ */
