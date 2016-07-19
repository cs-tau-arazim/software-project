#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "SPImageProc.h"

#define ENTER_QUERY "Please enter an image path:\n"
#define EXITING "Exiting...\n"
#define BEST_CANDIDATES "Best candidates for - %s - are:\n"

#define GENERAL_ERROR_MSG "An error occurred"
#define PCA_DIM_ERROR_MSG "PCA dimension couldn't be resolved"
#define PCA_FILE_NOT_EXIST "PCA file doesn't exist"
#define PCA_FILE_NOT_RESOLVED "PCA filename couldn't be resolved"
#define NUM_OF_IMAGES_ERROR "Number of images couldn't be resolved"
#define NUM_OF_FEATS_ERROR "Number of features couldn't be resolved"
#define MINIMAL_GUI_ERROR "Minimal GUI mode couldn't be resolved"
#define IMAGE_PATH_ERROR "Image path couldn't be resolved"
#define IMAGE_NOT_EXIST_MSG ": Images doesn't exist"
#define MINIMAL_GUI_NOT_SET_WARNING "Cannot display images in non-Minimal-GUI mode"
#define FILE_WRITE_ERROR "Failed to write to feature file"
#define FILE_READ_ERROR "Failed to read from feature file"

#define ALLOC_ERROR_MSG "Allocation error"
#define INVALID_ARG_ERROR "Invalid arguments"

#define INVALID_CMD_LINE "Invalid command line : use -c <config_filename>\n"
#define LINE_LENGTH 1024

extern "C" {
#include "SPLogger.h"
#include "main_aux.h"
}

int main(int argc, char **argv) {

	// Declare all variables

	SP_CONFIG_MSG configMsg = SP_CONFIG_SUCCESS;

	SPPoint ** featureArr;
	SPPoint * feature1DimArr;
	sp::ImageProc *imgProc;
	KDTreeNode KDTree;

	int i, j, k, res, numOfImages;
	int PCADim;
	double * tempDoubleArr;
	char configPath[LINE_LENGTH];
	char imagePath[LINE_LENGTH];
	char imageFeaturePath[LINE_LENGTH];
	FILE * imageFeatureFile;

	int* numOfFeatures;
	int featureArrSize = 0;

	// ***********************
	// Part 1 - Configuration
	// ***********************

	// Get config path and check for failure
	if (getConfigPath(argc, argv, configPath) != 0) {
		printf(INVALID_CMD_LINE);
		return 0;
	}

	SPConfig config = spConfigCreate(argv[2], &configMsg);

	if (configMsg != SP_CONFIG_SUCCESS) {
		return 0;
	}

	// TODO Logger
	numOfImages = spConfigGetNumOfImages(config, &configMsg);
	if (configMsg != SP_CONFIG_SUCCESS) {
		spLoggerPrintError(NUM_OF_IMAGES_ERROR, __FILE__, __func__, __LINE__);
		free(config);
		return 0;
	}

	PCADim = spConfigGetPCADim(config, &configMsg);
	if (configMsg != SP_CONFIG_SUCCESS) {
		spLoggerPrintError(PCA_DIM_ERROR_MSG, __FILE__, __func__, __LINE__);
		free(config);
		return 0;
	}

	imgProc = new sp::ImageProc(config);

	featureArr = (SPPoint**) malloc(sizeof(SPPoint*) * numOfImages);
	if (featureArr == NULL) {
		spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
		free(config);
		delete imgProc;
		return 0;
	}

	numOfFeatures = (int*) malloc(sizeof(int) * numOfImages);
	if (numOfFeatures == NULL) {
		spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
		free(config);
		free(featureArr);
		delete imgProc;
		return 0;
	}

	// ***********************
	// Part 2 - Extraction
	// ***********************

	// Check extraction mode
	if (spConfigIsExtractionMode(config, &configMsg) == true) {

		// start
		printf("time to extract! :)\n"); // TODO REMOVE
		//printConfig(config); // TODO REMOVE

		// For all images:
		for (i = 0; i < numOfImages; i++) {

			// Create image path
			configMsg = spConfigGetImagePath(imagePath, config, i);
			if (configMsg != SP_CONFIG_SUCCESS) {
				spLoggerPrintError(IMAGE_PATH_ERROR, __FILE__, __func__,
						__LINE__);
				free(config);
				free(numOfFeatures);
				free2dPoints(featureArr, i, numOfFeatures);
				delete imgProc;
				return 0;
			}

			// Get image features
			featureArr[i] = imgProc->getImageFeatures(imagePath, i,
					&(numOfFeatures[i]));
			if (featureArr[i] == NULL) {
				spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__,
						__LINE__);
				free(config);
				delete imgProc;
				free2dPoints(featureArr, i, numOfFeatures);
				free(numOfFeatures);
				return 0;
			}

			k = spPointGetAxisCoor(featureArr[i][0], 0);

			// Increase array size
			featureArrSize += numOfFeatures[i];

			// Create feature file with data
			spConfigGetImageFeatPath(imageFeaturePath, config, i);
			if (configMsg != SP_CONFIG_SUCCESS) {
				spLoggerPrintError(IMAGE_PATH_ERROR, __FILE__, __func__,
						__LINE__);
				free(config);
				delete imgProc;
				free2dPoints(featureArr, i + 1, numOfFeatures);
				free(numOfFeatures);
				return 0;
			}

			// Write data to file
			res = writeFeaturesToFile(featureArr, imageFeaturePath, i,
					numOfFeatures[i], PCADim);
			// Check for errors
			if (res == 1) {
				spLoggerPrintError(FILE_WRITE_ERROR, __FILE__, __func__,
						__LINE__);
			}

		}

	}

	// Non-Extraction Mode- here we need to create the KD-Tree from the feature files.
	else {
		// start
		printf("time to NOT extract! :)\n"); // TODO REMOVE

		tempDoubleArr = (double*) malloc(sizeof(double) * PCADim);

		// For all images:
		for (i = 0; i < numOfImages; i++) {
			int imIndex;
			// Find feature file with data
			spConfigGetImageFeatPath(imageFeaturePath, config, i);
			if (configMsg != SP_CONFIG_SUCCESS) {
				spLoggerPrintError(IMAGE_PATH_ERROR, __FILE__, __func__,
						__LINE__);
				free(config);
				delete imgProc;
				free2dPoints(featureArr, i, numOfFeatures);
				free(numOfFeatures);
				return 0;
			}

			// Read data from file
			res = readFeaturesFromFile(featureArr, imageFeaturePath, i,
					numOfFeatures, PCADim, &featureArrSize, tempDoubleArr);

			// Check for errors
			if (res == 1) {
				spLoggerPrintError(FILE_WRITE_ERROR, __FILE__, __func__,
						__LINE__);
				free(config);
				delete imgProc;
				free2dPoints(featureArr, i, numOfFeatures);
				free(numOfFeatures);
				return 0;
			}
		}
	}
	free(tempDoubleArr);

	feature1DimArr = (SPPoint*) malloc(featureArrSize * sizeof(SPPoint));
	if (feature1DimArr == NULL) {
		spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
		free(config);
		delete imgProc;
		free2dPoints(featureArr, numOfImages, numOfFeatures);
		free(numOfFeatures);
		return 0;
	}

	k = 0;
	for (i = 0; i < numOfImages; i++) {
		for (j = 0; j < numOfFeatures[i]; j++) {
			feature1DimArr[k] = spPointCopy(featureArr[i][j]);
			k++;
		}
	}

// Now we just need to create the KDTree.
	KDTree = kdTreeInit(feature1DimArr, featureArrSize, PCADim,
			spConfigGetSplitMethod(config, &configMsg));
	if (KDTree == NULL) {
		// TODO print to logger in tree func
		free(config);
		delete imgProc;
		free2dPoints(featureArr, numOfImages, numOfFeatures);
		free1dPoints(feature1DimArr, featureArrSize);
		free(numOfFeatures);
		return 0;
	}

	free2dPoints(featureArr, numOfImages, numOfFeatures);
	free1dPoints(feature1DimArr, featureArrSize);
	free(numOfFeatures);

// ***********************
// Part 3 - Main Loop
// ***********************

	while (true) {
		// Define variables in beginning of scope
		char query[LINE_LENGTH];
		SPPoint* features;
		int numOfBestImages, spKNN, numOfQueryFeatures, numOfImages;
		int* closestImages;
		bool minimalGui;

		memset(query, 0, sizeof(query));
		printf(ENTER_QUERY);
		fgets(query, LINE_LENGTH, stdin);

		// Check if no query was given
		if (strcmp("\n", query) == 0) {
			break;
		}
		strtok(query, "\n");

		// Get variables from configuration file
		numOfImages = spConfigGetNumOfImages(config, &configMsg);
		spKNN = spConfigGetSPKNN(config, &configMsg);
		numOfBestImages = spConfigGetNumOfSimilarImages(config, &configMsg);
		minimalGui = spConfigMinimalGui(config, &configMsg);

		// Extract Features
		features = imgProc->getImageFeatures(query, numOfImages,
				&numOfQueryFeatures);

		// Get closest images
		closestImages = bestImages(numOfBestImages, spKNN, KDTree, features,
				numOfQueryFeatures, numOfImages);

		if (!minimalGui) {
			int i;
			printf(BEST_CANDIDATES, query);
			for (i = 0; i < numOfBestImages; i++) {
				char imagePath[LINE_LENGTH];

				spConfigGetImagePath(imagePath, config, closestImages[i]); // TODO check if fails
				printf("%s\n", imagePath);
			}
			printf("\n");
		} else {
			for (i = 0; i < numOfBestImages; i++) {
				char imagePath[LINE_LENGTH];

				spConfigGetImagePath(imagePath, config, closestImages[i]); // TODO check if fails
				imgProc->showImage(imagePath);
			}
		}

		// Free query features
		for (i = 0; i < numOfQueryFeatures; i++) {
			spPointDestroy(features[i]);
		}
		free(features);
		free(closestImages);
	}

	printf(EXITING);
	kdTreeNodeDestroy(KDTree);
	free(config);
	delete imgProc;
	return 0;

}
