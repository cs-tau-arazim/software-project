/*
 * main_aux.c
 *
 *  Created on: Jun 29, 2016
 *      Author: galwiernik
 */

#include "main_aux.h"
#include <stdlib.h>


int cmpCounts (const void * point1, const void * point2);
/**
 * Helper function to print the appropriate message
 * for each error received by spConfigCreate.
 */
void printErrorType(SP_CONFIG_MSG * configMsg) {
	if ((*configMsg) == SP_CONFIG_INVALID_ARGUMENT)
		printf("Message: No configuration file");
	if ((*configMsg) == SP_CONFIG_CANNOT_OPEN_FILE)
		printf("Message: Invalid configuration file");
	if ((*configMsg) == SP_CONFIG_ALLOC_FAIL)
		printf("Message: Memory allocation failure occurred");

	if ((*configMsg) == SP_CONFIG_INVALID_INTEGER) {
		printf("Line: ??????"); // LINE CHECK MEANING
		printf("Message: Invalid value - constraint not met");
	}

	if ((*configMsg) == SP_CONFIG_INVALID_INTEGER
			|| (*configMsg) == SP_CONFIG_INVALID_STRING) {
		printf("Line: ??????"); // LINE CHECK MEANING
		printf("Message: Invalid value - constraint not met");
	}
	if ((*configMsg) == SP_CONFIG_MISSING_DIR) {
		printf("Line: ??????"); // LINE CHECK MEANING
		printf("Message: Parameter spImagesDirectory is not set");
	}
	if ((*configMsg) == SP_CONFIG_MISSING_PREFIX) {
		printf("Line: ??????"); // LINE CHECK MEANING
		printf("Message: Parameter spImagesPrefix is not set");
	}
	if ((*configMsg) == SP_CONFIG_MISSING_SUFFIX) {
		printf("Line: ??????"); // LINE CHECK MEANING
		printf("Message: Parameter spImagesSuffix is not set");
	}
	if ((*configMsg) == SP_CONFIG_MISSING_NUM_IMAGES) {
		printf("Line: ??????"); // LINE CHECK MEANING
		printf("Message: Parameter spNumOfImages is not set");
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

	printf("%d, %s\n", __LINE__, __func__); //TODO remove

	for (j = 0 ; j < numOfImages ; j++)
	{
		closeFeaturesCount[j] = (int*) malloc (2*sizeof (int));
		if (closeFeaturesCount == NULL)
			return NULL;

		closeFeaturesCount[j][0] = 0;
		closeFeaturesCount[j][1] = j;
	}


	printf("%d, %s\n", __LINE__, __func__); //TODO remove

	for (i = 0 ; i < numOfFeatures ; i++)
	{
		bpq = spBPQueueCreate(spKNN);
		if (bpq == NULL)
			return NULL;

		printf("%d, %s\n", __LINE__, __func__); //TODO remove
		nearestNeighbors(root, bpq, features[i]);
		printf("%d, %s, %d\n", __LINE__, __func__, spKNN); //TODO remove
		for (j = 0 ; j < spKNN ; j++)
		{
			printf("%d, %s\n", __LINE__, __func__); //TODO remove
			int index;
			SPListElement curr = spBPQueuePeek(bpq);
			printf("%d, %s\n", __LINE__, __func__); //TODO remove
			index = spListElementGetIndex(curr);
			printf("%d, %s, %d\n", __LINE__, __func__, index); //TODO remove
			closeFeaturesCount[index][0]++;
			printf("%d, %s\n", __LINE__, __func__); //TODO remove
			spBPQueueDequeue(bpq);
			printf("%d, %s\n", __LINE__, __func__); //TODO remove

			spListElementDestroy(curr);
			printf("%d, %s\n", __LINE__, __func__); //TODO remove

		}
		printf("%d, %s\n", __LINE__, __func__); //TODO remove

		spBPQueueDestroy(bpq);
		printf("%d, %s\n", __LINE__, __func__); //TODO remove

	}

	printf("%d, %s\n", __LINE__, __func__); //TODO remove


	qsort(closeFeaturesCount, numOfImages, sizeof(closeFeaturesCount[0]),cmpCounts );

	bestImages = (int*)malloc(numOfBestImages*sizeof(int));

	printf("%d, %s\n", __LINE__, __func__); //TODO remove

	for(i = 0 ; i < numOfBestImages ; i++)
	{
		bestImages[i] = closeFeaturesCount[i][1];
	}

	printf("%d, %s\n", __LINE__, __func__); //TODO remove

	return bestImages;

}

int cmpCounts (const void * point1, const void * point2)
{
	int *p1 = *(int **)point1;
	int *p2 = *(int **)point2;
	return -(p1[0] - p2[0]); // TODO check if inverse
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
