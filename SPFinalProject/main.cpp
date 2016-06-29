#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "SPImageProc.h"

#define ENTER_QUERY "Please enter an image path:\n"
#define EXITING "Exiting..."
#define BEST_CANDIDATES "Best candidates for - %s - are:\n"
extern "C" {
#include "main_aux.h"
}

#define LINE_LENGTH 1024

int main(int argc, char **argv) {

	// Declare all variables

	SP_CONFIG_MSG * configMsg;
	SPPoint * featureArr;
	SPPoint * featureImageArr;
	sp::ImageProc *imgProc;
	KDTreeNode KDTree;

	int i, j, k, numOfImages;
	int PCADim;
	char imagePath[LINE_LENGTH];
	char imageFeaturePath[LINE_LENGTH];
	FILE * imageFeatureFile;

	int numOfFeatures;
	int featureArrSize = 0;

	printf("Hello World\n");

	// TODO check if config file received through cmd
	if (argc != 2) {
		printf("ERROR");
		exit(1);
	}

	// create SPConfig element
	configMsg = (SP_CONFIG_MSG *) malloc(sizeof(SP_CONFIG_MSG));
	SPConfig config = spConfigCreate(argv[1], configMsg);


	// Check for errors
	if (config == NULL) {
		printf("File: %s\n", argv[1]);
		printErrorType(configMsg);
	}

	numOfImages = spConfigGetNumOfImages(config, configMsg);
	PCADim = spConfigGetPCADim(config, configMsg);

	// Check extraction mode
	if (spConfigIsExtractionMode(config, configMsg) == true) {
		printf("time to extract! :)");
		imgProc = new sp::ImageProc(config);

		// For all images:
		for (i = 0; i < numOfImages; i++) {

			// Create image path
			spConfigGetImagePath(imagePath, config, i);
			// Get image features
			featureImageArr = imgProc->getImageFeatures(imagePath, i,
					&numOfFeatures);

			// Increase array size
			featureArrSize += numOfFeatures;

			// add more memory
			featureArr = (SPPoint*) realloc(featureArr,
					featureArrSize * sizeof(SPPoint));
			// copy points
			for (j = 0; j < numOfFeatures; j++) {
				featureArr[(featureArrSize - numOfFeatures - 1) + i] =
						featureImageArr[i];
			}

			// Create feature file with data
			spConfigGetImageFeatPath(imageFeaturePath, config, i);
			imageFeatureFile = fopen(imageFeaturePath, "w");
			fprintf(imageFeatureFile, "%d\n", i);
			fprintf(imageFeatureFile, "%d\n", numOfFeatures);

			// Print all features to file
			for (j = 0; j < numOfFeatures; j++) {
				for (k = 0; k < PCADim; k++) {
					fprintf(imageFeatureFile, "%d,", numOfFeatures);
				}
				fprintf(imageFeatureFile, "\n,");
			}
			fclose(imageFeatureFile);
		}

		// Now we just need to create the KDTree.
		KDTree = kdTreeInit(featureArr, featureArrSize, PCADim,
				spConfigGetSplitMethod(config, configMsg));
	}

	// Non-Extraction Mode- here we need to create the KD-Tree from the feature files.

	// Non-Extraction Mode- here we need to create the KD-Tree from the feature files.
	else {
		printf("YAY");

	}


	// Enter the main loop
	while (true) {
		char query[LINE_LENGTH];
		SPPoint* features;
		int numOfBestImages, spKNN, numOfFeatures, numOfImages;
		int* closestImages;
		bool minimalGui;

		memset(query, 0, sizeof(query));
		printf(ENTER_QUERY);
		fgets(query, LINE_LENGTH, stdout);
		strtok(query, "\n");

		if (strcmp("#", query) == 0) {
			printf(EXITING);
			// TODO free all mem
			return 0;
		}
		// TODO get image features

		numOfImages = spConfigGetNumOfImages(config, configMsg);
		spKNN = spConfigGetSPKNN(config, configMsg);
		numOfBestImages = spConfigGetNumOfSimilarImages(config, configMsg);

		features = imgProc->getImageFeatures(query, numOfImages,
				&numOfFeatures);

		closestImages = bestImages(numOfBestImages, spKNN, KDTree, features,
				numOfFeatures, numOfImages);

		minimalGui = spConfigMinialGui(config, configMsg);

		if (!minimalGui) {
			int i;
			printf(BEST_CANDIDATES, query);
			for (i = 0; i < numOfBestImages; i++) {
				char imagePath[LINE_LENGTH];

				spConfigGetImagePath(imagePath, config, closestImages[i]); // TODO check if fails
				printf("%s\n", imagePath);
			}
		} else {
			for (i = 0; i < numOfBestImages; i++) {
				char imagePath[LINE_LENGTH];

				spConfigGetImagePath(imagePath, config, closestImages[i]); // TODO check if fails
				imgProc->showImage(imagePath);
				system("pause"); // wait until a key is pressed
			}
		}

		free(features);
		free(closestImages);
	}
	return 0;
}
