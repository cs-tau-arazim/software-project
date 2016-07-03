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



struct kd_tree_node_t{
	int dim;
	double val;
	KDTreeNode left;
	KDTreeNode right;
	SPPoint data;
};

KDTreeNode constructTree(KDArray mat, int size,int dim, int splitMethod, int lastLevelDim);

/*
 * splitMethod:
 * 0 - RANDOM
 * 1 - MAX_SPREAD
 * 2 - INCREMENTAL
 */
KDTreeNode kdTreeInit (SPPoint* arr, int size, int dim, int splitMethod )
{
	if (arr == NULL || size < 1 || dim < 1 || splitMethod < 0 || splitMethod > 2)
		return NULL;
	KDArray mat;
	mat = kdArrayInit(arr, size, dim);
	printf("%d, %s\n",__LINE__, __func__); //TODO remove

	return constructTree(mat, size, dim, splitMethod, dim);

}

KDTreeNode constructTree(KDArray mat, int size, int dim, int splitMethod, int lastLevelDim)
{
	
	//printf("%d, %s, %d\n",__LINE__, __func__, size); //TODO remove

	KDTreeNode newNode;
	KDArray left, right;
	SPPoint* p;
	int splitDim = -1;
	double medianVal;

	assert(size > 0);
	//printf("%d, %s, %d\n",__LINE__, __func__, splitDim); //TODO remove
	p = kdArrayGetPoints(mat);

	newNode = (KDTreeNode)malloc(sizeof(*newNode));
	if (newNode == NULL)
		return NULL;

	if (size == 1)
	{
		newNode->dim = -1;
		newNode->val = -1;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->data = spPointCopy(p[0]);
		kdArrayDestroy(mat);
		return newNode;
	}

	if (splitMethod == 0)
	{
		srand(time(NULL));
		splitDim = rand()%dim;
	}
	else if (splitMethod == 1)
	{
		double maxSpread;
		int i;

		maxSpread = 0;
		for (i = 0 ; i < dim ; i++)
		{
			int minIndex, maxIndex;
			double spread;

			minIndex = kdArrayGet(mat, i, 0);
			maxIndex = kdArrayGet(mat, i, size-1);
			//printf("%d, %s, %d\n",__LINE__, __func__, minIndex); //TODO remove
			//printf("%d, %s, %d\n",__LINE__, __func__, maxIndex); //TODO remove

			spread = spPointGetAxisCoor(p[maxIndex],i) - spPointGetAxisCoor(p[minIndex],i);
			if (spread >= maxSpread)
			{
				maxSpread = spread;
				splitDim = i;
			}
		}
	}
	else if(splitMethod == 2)
	{
		splitDim = (lastLevelDim+1)%dim;
	}

	printf("%d, %s\n",__LINE__, __func__); //TODO remove

	left = kdArrayInitEmpty();
	if(left == NULL)
		return NULL;
	right = kdArrayInitEmpty();
	if(right == NULL)
		return NULL;

	printf("%d, %s, %d\n",__LINE__, __func__, splitDim); //TODO remove

	medianVal = spPointGetAxisCoor(p[kdArrayGet(mat, splitDim, (size-1)/2)], splitDim);
	printf("%d, %s\n",__LINE__, __func__); //TODO remove

	kdArraySplit (mat, splitDim, left, right);
	printf("%d, %s\n",__LINE__, __func__); //TODO remove


	newNode->dim = splitDim;
	newNode->val = medianVal;

	printf("%d, %s left\n",__LINE__, __func__); //TODO remove
	newNode->left = constructTree(left, kdArrayGetSize(left), dim, splitMethod, splitDim);

	printf("%d, %s right\n",__LINE__, __func__); //TODO remove
	newNode->right = constructTree(right, kdArrayGetSize(right), dim, splitMethod, splitDim);

	printf("%d, %s\n",__LINE__, __func__); //TODO remove

	newNode->data = NULL;

	printf("%d, %s\n",__LINE__, __func__); //TODO remove

	printf("%d, %s, %d\n",__LINE__, __func__, size); //TODO remove

	return newNode;
}

void kdTreeNodeDestroy (KDTreeNode kdTreeNode)
{
	if (kdTreeNode == NULL)
		return;
	kdTreeNodeDestroy (kdTreeNode->left);
	kdTreeNodeDestroy (kdTreeNode->right);
	if (kdTreeNode->data != NULL)
		free(kdTreeNode->data);
	free (kdTreeNode);
		return;
}

void nearestNeighbors (KDTreeNode curr, SPBPQueue bpq, SPPoint p)
{
	SPListElement node;
	SPPoint q;
	bool isLeft;
	double coorDis;
	if (curr == NULL || bpq == NULL || p == NULL)
		return;

	q = curr->data;

	/* Add the current point to the BPQ. Note that this is a no-op if the
	 * point is not as good as the points we've seen so far.*/
	if(curr->dim == -1)
	{
		int index;
		double dis;

		index = spPointGetIndex(q);
		dis = spPointL2SquaredDistance(p, curr->data);
		node = spListElementCreate(index, dis);
		spBPQueueEnqueue(bpq, node);
		return;
	}

	/* Recursively search the half of the tree that contains the test point. */
	if(spPointGetAxisCoor(p, curr->dim) <= curr->val)
	{
		nearestNeighbors(curr->left, bpq, p);
		isLeft = true;
	}
	else
	{
		nearestNeighbors(curr->right, bpq, p);
		isLeft = false;
	}

	/* If the candidate hypersphere crosses this splitting plane, look on the
	* other side of the plane by examining the other subtree*/
	coorDis = abs(spPointGetAxisCoor(p, curr->dim) - curr->val);
	if (!spBPQueueIsFull(bpq) || coorDis < spBPQueueMaxValue(bpq))
	{
		if (isLeft)
			nearestNeighbors(curr->right, bpq, p);
		else
			nearestNeighbors(curr->left, bpq, p);
	}

}

void printTree (KDTreeNode kdTree)
{
	int i;
	if (kdTree == NULL)
	{
		return;
	}
	printf(" (");
	printTree(kdTree->left);
	if (kdTree->data == NULL)
		printf("dim: %d, val: %f", kdTree->dim, kdTree-> val);
	else
	{
		printf("point: ");
		for (i = 0 ; i < spPointGetDimension(kdTree->data) ; i++)
		{

			printf("%lf,", spPointGetAxisCoor(kdTree->data, i));
		}
	}
	printTree(kdTree->right);
	printf(" )");
}

