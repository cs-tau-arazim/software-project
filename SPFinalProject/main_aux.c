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
		for (j = 0 ; j < spKNN ; i++)
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

	return bestImages;

}

int cmpCounts (const void * point1, const void * point2)
{
	int *p1 = (int *)point1;
	int *p2 = (int *)point2;
	return (p1[0] - p2[0]); // TODO check if inverse
}

