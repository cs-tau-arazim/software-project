/*
 * KDArray.h
 *
 *  Created on: Jun 28, 2016
 *      Author: tomsegal
 */
#include "SPPoint.h"

#ifndef KDARRAY_H_
#define KDARRAY_H_

/** Type for defining the KDArray **/
typedef struct kd_array_t* KDArray;

/**
 * Initializes the kd-array with the data given by arr
 *
 * @return
 * NULL if memory allocation failed or arr == NULL or size < 1 or dim < 1
 * the new KDArray otherwise
 */
KDArray kdArrayinit (SPPoint* arr, int size, int dim);

/**
 * Stores two KDArrays in (kdLeft, kdRight) such that the first
 * half points with respect to the coordinate coor are in kdLeft,
 * and the rest of the points are in kdRight.
 *
 * does nothing if any of the params is NULL or coor < 1
 */
void kdArraysplit (KDArray kdArr, int coor, KDArray kdLeft, KDArray kdRight);

/**
 *  Free all memory allocation associated with kdArr,
 * 	if kdArr is NULL nothing happens.
 */
void kdArraydestroy (KDArray kdArr);

#endif /* KDARRAY_H_ */
