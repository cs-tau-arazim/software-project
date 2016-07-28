/*
 * KDTreeNode.c
 *
 *  Created on: Jun 29, 2016
 *      Author: tomsegal
 */

#include "KDTreeNode.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#define ALLOC_ERROR_MSG "Allocation error"

struct kd_tree_node_t {
	int dim;
	double val;
	KDTreeNode left;
	KDTreeNode right;
	SPPoint data;
};

/*
 * Private method for kdTreeInit, recursively called
 */
KDTreeNode constructTree(KDArray mat, int size, int dim, int splitMethod,
		int lastLevelDim);

/**
 * Given an array of points the function creates a kd-tree containing the points.
 *
 * @param arr - the array of SPPoints
 * @param size - the size of arr
 * @param dim - the dimension of the points in arr
 * @param splitMethod - the method to construct
 * the tree: 0 - RANDOM, 1 - MAX_SPREAD, 2 - INCREMENTAL
 *
 * @assert dim = point->dim for each point in arr;
 *
 * @return
 * NULL if arr == NULL or size < 1 or dim <1 or splitMethod
 * isn't between 0 and 2.
 * the new kd-tree otherwise
 */
KDTreeNode kdTreeInit(SPPoint* arr, int size, int dim, int splitMethod) {
	KDTreeNode kdt;

	if (arr == NULL || size < 1 || dim < 1 || splitMethod < 0
			|| splitMethod > 2)
		return NULL ;

	KDArray mat;
	mat = kdArrayInit(arr, size, dim);

	kdt = constructTree(mat, size, dim, splitMethod, dim);
	kdArrayDestroy(mat);
	return kdt;

}

/**
 * A recursive function to construct the kd-tree
 */
KDTreeNode constructTree(KDArray mat, int size, int dim, int splitMethod,
		int lastLevelDim) {
	KDTreeNode newNode;
	KDArray left, right;
	SPPoint* p;
	int splitDim = -1;
	double medianVal;

	assert(size > 0);
	p = kdArrayGetPoints(mat);

	newNode = (KDTreeNode) malloc(sizeof(*newNode));
	if (newNode == NULL )
	{
		spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
		return NULL ;
	}

	if (size == 1) { // creates a new leaf which represents a point
		newNode->dim = -1;
		newNode->val = -1;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->data = spPointCopy(p[0]);
		return newNode;
	}

	if (splitMethod == 0) { // RANDOM
		srand(time(NULL ));
		splitDim = rand() % dim;
	} else if (splitMethod == 1) { // MAX SPREAD
		double maxSpread;
		int i;

		maxSpread = 0;
		for (i = 0; i < dim; i++) {
			int minIndex, maxIndex;
			double spread;

			minIndex = kdArrayGet(mat, i, 0);
			maxIndex = kdArrayGet(mat, i, size - 1);

			spread = spPointGetAxisCoor(p[maxIndex], i)
					- spPointGetAxisCoor(p[minIndex], i);
			if (spread >= maxSpread) {
				maxSpread = spread;
				splitDim = i;
			}
		}
	} else if (splitMethod == 2) { // INCREMENTAL
		splitDim = (lastLevelDim + 1) % dim;
	}

	left = kdArrayInitEmpty();
	if (left == NULL )
		return NULL ;
	right = kdArrayInitEmpty();
	if (right == NULL )
	{
		kdArrayDestroy(left);
		return NULL ;
	}

	// compute the median to split according to it
	medianVal = spPointGetAxisCoor(p[kdArrayGet(mat, splitDim, (size - 1) / 2)],
			splitDim);

	kdArraySplit(mat, splitDim, left, right);

	newNode->dim = splitDim;
	newNode->val = medianVal;

	// recursively create the left sub-tree
	newNode->left = constructTree(left, kdArrayGetSize(left), dim, splitMethod,
			splitDim);

	// recursively create the right sub-tree
	newNode->right = constructTree(right, kdArrayGetSize(right), dim,
			splitMethod, splitDim);

	newNode->data = NULL; // not a leaf

	// free data not needed anymore
	kdArrayDestroy(left);
	kdArrayDestroy(right);

	return newNode;
}


/**
 * Frees all memory resources associate with kdTreeNode.
 * If kdTreeNode == NULL nothig is done.
 */
void kdTreeNodeDestroy(KDTreeNode kdTreeNode) {
	if (kdTreeNode == NULL )
		return;
	if (kdTreeNode->data != NULL )
		spPointDestroy(kdTreeNode->data);
	kdTreeNodeDestroy(kdTreeNode->left);
	kdTreeNodeDestroy(kdTreeNode->right);
	free(kdTreeNode);
	return;
}


/**
 * Given a kd-tree and a point p, the function stores the nearest neighbors of p to bpq
 *
 * @param curr - the kd-tree containing the points
 * @param bpq - the bounded priority queue to store the nearest neighbors in
 * @param p - the point to find the nearest neighbors to
 *
 * does nothing if curr == NULL or bpq == NULL or p == NULL
 */
void nearestNeighbors(KDTreeNode curr, SPBPQueue bpq, SPPoint p) {
	SPListElement node;
	SPPoint q;
	bool isLeft;
	double coorDis;
	if (curr == NULL || bpq == NULL || p == NULL )
		return;

	q = curr->data;

	/* Add the current point to the BPQ. Note that this is a no-op if the
	 * point is not as good as the points we've seen so far.*/
	if (curr->dim == -1) {
		int index;
		double dis;

		index = spPointGetIndex(q);
		dis = spPointL2SquaredDistance(p, curr->data);
		node = spListElementCreate(index, dis);
		spBPQueueEnqueue(bpq, node);
		spListElementDestroy(node);
		return;
	}

	/* Recursively search the half of the tree that contains the test point. */
	if (spPointGetAxisCoor(p, curr->dim) <= curr->val) {
		nearestNeighbors(curr->left, bpq, p);
		isLeft = true;
	} else {
		nearestNeighbors(curr->right, bpq, p);
		isLeft = false;
	}

	/* If the candidate hypersphere crosses this splitting plane, look on the
	 * other side of the plane by examining the other subtree*/
	coorDis = abs(spPointGetAxisCoor(p, curr->dim) - curr->val);
	if (!spBPQueueIsFull(bpq) || coorDis*coorDis < spBPQueueMaxValue(bpq)) {
		if (isLeft)
			nearestNeighbors(curr->right, bpq, p);
		else
			nearestNeighbors(curr->left, bpq, p);
	}

}

/**
 * prints the tree for debuging
 */
void printTree(KDTreeNode kdTree) {
	int i;
	if (kdTree == NULL ) {
		return;
	}
	printf(" (");
	printTree(kdTree->left);
	if (kdTree->data == NULL )
		printf("dim: %d, val: %f", kdTree->dim, kdTree->val);
	else {
		printf("index: %d,point: ", spPointGetIndex(kdTree->data));
		for (i = 0; i < spPointGetDimension(kdTree->data); i++) {

			printf("%lf,", spPointGetAxisCoor(kdTree->data, i));
		}
	}
	printTree(kdTree->right);
	printf(" )");
}

