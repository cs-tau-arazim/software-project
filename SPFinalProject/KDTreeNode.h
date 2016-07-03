/*
 * KDTreeNode.h
 *
 *  Created on: Jun 29, 2016
 *      Author: tomsegal
 */
#include "SPPoint.h"
#include "SPBPriorityQueue.h"
#include "KDArray.h"
#include "SPPoint.h"


#ifndef KDTREENODE_H_
#define KDTREENODE_H_

/** Type for defining the KDTree **/
typedef struct kd_tree_node_t* KDTreeNode;

/** TODO doc
 * Initializes the kd-tree with the data given by arr
 *
 * @return
 * NULL if memory allocation failed or arr == NULL or size < 1 or dim < 1
 * the new KDArray otherwise
 */
KDTreeNode kdTreeInit (SPPoint* arr, int size, int dim, int splitMethod );


/**
 *  Free all memory allocation associated with kdTree,
 * 	if kdTree is NULL nothing happens.
 */
void kdTreeNodeDestroy (KDTreeNode kdTreeNode);

/**
 * TODO doc
 */
void nearestNeighbors (KDTreeNode curr, SPBPQueue bpq, SPPoint p);

void printTree (KDTreeNode kdTree);


#endif /* KDTREENODE_H_ */
