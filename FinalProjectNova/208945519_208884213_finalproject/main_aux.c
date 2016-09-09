/*
 * main_aux.c
 *
 *  Created on: Jun 29, 2016
 *      Author: galwiernik
 */

#include "main_aux.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

/**
 * The function receives an imageFeaturePath path and relevant parameters,
 * and extracts the info relevant to the specific image into the file.
 *  * returns 0 on success and 1 on failure.
 */
int writeFeaturesToFile(SPPoint ** featureArr, char * imageFeaturePath, int i, int numOfFeatures, int PCADim) {
	FILE * imageFeatureFile;
	int j, k, failCheck;
	imageFeatureFile = fopen(imageFeaturePath, "w");
	if (imageFeatureFile == NULL)
		return 1;
	fprintf(imageFeatureFile, "%d\n", i);
	fprintf(imageFeatureFile, "%d\n", numOfFeatures);

	// Print all features to file
	for (j = 0; j < numOfFeatures; j++) {
		for (k = 0; k < PCADim; k++) {
			fprintf(imageFeatureFile, "%lf,",
					spPointGetAxisCoor(featureArr[i][j], k));
		}
		fprintf(imageFeatureFile, "\n");
	}
	failCheck = fclose(imageFeatureFile);
	if (failCheck != 0)
		return 1;
	return 0;
}

/**
 * The function receives an imageFeaturePath path and relevant parameters,
 * and extracts the info relevant FROM the specific file INTO the featureArr.
 * returns 0 on success and 1 on failure.
 */
int readFeaturesFromFile(SPPoint ** featureArr, char * imageFeaturePath, int i, int * numOfFeatures, int PCADim, int * featureArrSize, double * tempDoubleArr) {
	FILE * imageFeatureFile;
	int imIndex, j, k, failCheck;
	imageFeatureFile = fopen(imageFeaturePath, "r");
	if (imageFeatureFile == NULL)
		return 1;
	fscanf(imageFeatureFile, "%d\n", &imIndex);
	assert(i == imIndex);

	fscanf(imageFeatureFile, "%d\n", &numOfFeatures[i]);

	// Increase array size
	*featureArrSize += numOfFeatures[i];

	// create array of points
	featureArr[i] = (SPPoint*) malloc(
			sizeof(SPPoint) * numOfFeatures[i]);

	// Get all features from file
	for (j = 0; j < numOfFeatures[i]; j++) {
		for (k = 0; k < PCADim; k++) {
			fscanf(imageFeatureFile, "%lf,", &(tempDoubleArr[k]));
		}
		fscanf(imageFeatureFile, "\n");
		featureArr[i][j] = spPointCreate(tempDoubleArr, PCADim, i);
	}
	failCheck = fclose(imageFeatureFile);
	if (failCheck != 0)
		return 1;
	return 0;
}


/**
 * Returns the closest images to the query image.
 * 
 * Input:
 * numOfBestImages - the number of images to return
 * spKNN - the number of features to consider close
 * root - the root of the database kdtree
 * features - the features of the query image
 * numOfFeatures - the size of features
 * numOfImages - the total number of images
 * 
 * Returns NULL in case of allocation failure.
 */
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

/**
 * Comparator function used for comparing integers with indexes.
 * Compares integers according to their value (index 0) in reverse order.
 * used for calling the qsort function for sorting lists of points.
 */
int cmpCounts (const void * point1, const void * point2)
{
	int *p1 = *(int **)point1;
	int *p2 = *(int **)point2;
	return -(p1[0] - p2[0]);
}

/**
 * Tester function for the KDArray module.
 * Checks for baisc functionality of the module, and was used for debugging.
 */
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

/**
 * Tester function for the KDTreeNode module.
 * Checks for baisc functionality of the module, and was used for debugging.
 */
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

/**
 * The function receives a two-dimentional array of points, its size, and a list of its subarray's sizes.
 * It frees the array, its subarrays and destorys all the points inside it.
 */
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

/**
 * The function receives an array of points and its size.
 * It frees the array and destorys all the points inside it.
 */
void free1dPoints (SPPoint * points, int size)
{
	int i;
	for (i = 0 ; i < size ; i ++)
	{
		spPointDestroy(points[i]);
	}
	free(points);
}
