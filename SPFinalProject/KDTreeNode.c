/*
 * KDTreeNode.c
 *
 *  Created on: Jun 29, 2016
 *      Author: tomsegal
 */

#include "KDTreeNode.h"
#include "KDArray.h"
#include "SPPoint.h"
#include <stdlib.h>
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
	return constructTree(mat, size, dim, splitMethod, dim);

}

KDTreeNode constructTree(KDArray mat, int size, int dim, int splitMethod, int lastLevelDim)
{
	KDTreeNode newNode;
	KDArray left, right;
	SPPoint* p;
	int splitDim;
	double medianVal;

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
		newNode->data = p[0];
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

			spread = spPointGetAxisCoor(p[maxIndex],i) - spPointGetAxisCoor(p[minIndex],i);
			if (spread > maxSpread)
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

	left = NULL;
	right = NULL;
	medianVal = spPointGetAxisCoor(p[kdArrayGet(mat, splitDim, (size+1)/2)], splitDim);
	kdArraySplit (mat, splitDim, left, right);

	newNode->dim = splitDim;
	newNode->val = medianVal;
	newNode->left = constructTree(left, kdArrayGetSize(left), dim, splitMethod, splitDim);
	newNode->right = constructTree(right, kdArrayGetSize(right), dim, splitMethod, splitDim);;
	newNode->data = NULL;

	return newNode;
}

