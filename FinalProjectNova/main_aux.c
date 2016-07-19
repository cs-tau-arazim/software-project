/*
 * main_aux.c
 *
 *  Created on: Jun 29, 2016
 *      Author: galwiernik
 */

#include "main_aux.h"
#include <stdlib.h>
#include <string.h>

#define INVALID_CMD_LINE "Invalid command line : use -c <config_filename>\n"


int cmpCounts (const void * point1, const void * point2);


/*
 * Helper function-
 * receives argc and argv directly from main, and sets configPath as appropriate path,
 * based on command line arguments. returns 0 on success, 1 on fail (invalid command line).
 */
int getConfigPath(int argc, char** argv, char * configPath) {
	// check cases
	if (argc == 1) {
			strcpy(configPath, "spcbir.config");
			return 0;
	}
	else if (argc != 3) {
		return 1;
	}
	else {
		if (strcmp(argv[1],"-c") != 0) {
			return 1;
		}
		else {
			strcpy(configPath, argv[2]);
			return 0;
		}
	}
}

int* bestImages(int numOfBestImages, int spKNN, KDTreeNode root, SPPoint* features, int numOfFeatures, int numOfImages)
{
	SPBPQueue bpq;
	int** closeFeaturesCount;
	int* bestImages;
	int i,j;

	closeFeaturesCount = (int**) malloc (numOfImages*sizeof (int*));
	if (closeFeaturesCount == NULL)
		return NULL;

	for (j = 0 ; j < numOfImages ; j++)
	{
		closeFeaturesCount[j] = (int*) malloc (2*sizeof (int));
		if (closeFeaturesCount == NULL)
			return NULL;

		closeFeaturesCount[j][0] = 0;
		closeFeaturesCount[j][1] = j;
	}

	for (i = 0 ; i < numOfFeatures ; i++)
	{
		bpq = spBPQueueCreate(spKNN);
		if (bpq == NULL)
			return NULL;

		nearestNeighbors(root, bpq, features[i]);
		for (j = 0 ; j < spKNN ; j++)
		{
			int index;
			SPListElement curr = spBPQueuePeek(bpq);
			index = spListElementGetIndex(curr);
			closeFeaturesCount[index][0]++;
			spBPQueueDequeue(bpq);

			spListElementDestroy(curr);

		}

		spBPQueueDestroy(bpq);
	}

	qsort(closeFeaturesCount, numOfImages, sizeof(closeFeaturesCount[0]),cmpCounts );

	bestImages = (int*)malloc(numOfBestImages*sizeof(int));

	for(i = 0 ; i < numOfBestImages ; i++)
	{
		bestImages[i] = closeFeaturesCount[i][1];
	}

	for (j = 0 ; j < numOfImages ; j++)
	{
		free(closeFeaturesCount[j]);
	}
	free(closeFeaturesCount);

	return bestImages;

}

int cmpCounts (const void * point1, const void * point2)
{
	int *p1 = *(int **)point1;
	int *p2 = *(int **)point2;
	return -(p1[0] - p2[0]);
}

void arrayTest ()
{
	SPPoint p1,p2,p3,p4,p5;
	KDArray arr,left,right;
	double arr1[] = {1,2}, arr2[] = {123,70}, arr3[] = {2,7}, arr4[] = {9,11}, arr5[] = {3,4};
	p1 = spPointCreate(arr1, 2, 1);
	p2 = spPointCreate(arr2, 2, 1);
	p3 = spPointCreate(arr3, 2, 2);
	p4 = spPointCreate(arr4, 2, 2);
	p5 = spPointCreate(arr5, 2, 3);
	SPPoint pointArr[] ={p1,p2,p3,p4,p5};
	arr = kdArrayInit(pointArr, 5, 2);

	printf("original:\n");
	printArray(arr);
	left = kdArrayInitEmpty();
	right = kdArrayInitEmpty();
	kdArraySplit(arr, 0, left, right);
	printf("after split:\n");
	printf("left:\n");
	printArray(left);
	printf("right:\n");
	printArray(right);
}

void treeTest() {

	SPPoint p1,p2,p3,p4,p5;
	double arr1[] = {1,2}, arr2[] = {123,70}, arr3[] = {2,7}, arr4[] = {9,11}, arr5[] = {3,4};
	p1 = spPointCreate(arr1, 2, 1);
	p2 = spPointCreate(arr2, 2, 1);
	p3 = spPointCreate(arr3, 2, 2);
	p4 = spPointCreate(arr4, 2, 2);
	p5 = spPointCreate(arr5, 2, 3);
	SPPoint pointArr[] ={p1,p2,p3,p4,p5};


	KDTreeNode tree = kdTreeInit(pointArr, 5, 2,1);
	printTree(tree);

}

void free2dPoints (SPPoint ** points, int size, int * rowsSizes)
{
	int i,j;
	for (i = 0 ; i < size ; i ++)
	{
		for (j = 0 ; j < rowsSizes[i] ; j++)
		{
			spPointDestroy(points[i][j]);
		}
		free(points[i]);
	}
	free(points);
}

void free1dPoints (SPPoint * points, int size)
{
	int i;
	for (i = 0 ; i < size ; i ++)
	{
		spPointDestroy(points[i]);
	}
	free(points);
}
