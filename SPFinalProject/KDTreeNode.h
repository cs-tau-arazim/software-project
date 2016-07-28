/*
 * KDTreeNode.h
 *
 *  Created on: Jun 29, 2016
 *      Author: tomsegal
 */
#include "SPBPriorityQueue.h"
#include "KDArray.h"


#ifndef KDTREENODE_H_
#define KDTREENODE_H_

/** Type for defining the KDTree **/
typedef struct kd_tree_node_t* KDTreeNode;

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
 * isn't between 0 and 2 or memory allocation failed
 * the new kd-tree otherwise
 */
KDTreeNode kdTreeInit (SPPoint* arr, int size, int dim, int splitMethod );


/**
 * Frees all memory resources associate with kdTreeNode.
 * If kdTreeNode == NULL nothig is done.
 */
void kdTreeNodeDestroy (KDTreeNode kdTreeNode);

/**
 * Given a kd-tree and a point p, the function stores the nearest neighbors of p to bpq
 *
 * @param curr - the kd-tree containing the points
 * @param bpq - the bounded priority queue to store the nearest neighbors in
 * @param p - the point to find the nearest neighbors to
 *
 * does nothing if curr == NULL or bpq == NULL or p == NULL
 */
void nearestNeighbors (KDTreeNode curr, SPBPQueue bpq, SPPoint p);

/**
 * prints the tree for debuging
 */
void printTree (KDTreeNode kdTree);


#endif /* KDTREENODE_H_ */
