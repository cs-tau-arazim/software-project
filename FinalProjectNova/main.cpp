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

// For Debbuging:
// 	printf("%d, %s\n",__LINE__, __func__); //TODO remove

int main(int argc, char **argv) {

	// Declare all variables

	SP_CONFIG_MSG * configMsg;
	SPPoint ** featureArr;
	SPPoint * feature1DimArr;
	sp::ImageProc *imgProc;
	KDTreeNode KDTree;

	int i, j, k, numOfImages;
	int PCADim;
	double * tempDoubleArr;
	char imagePath[LINE_LENGTH];
	char imageFeaturePath[LINE_LENGTH];
	FILE * imageFeatureFile;

	int* numOfFeatures;
	int featureArrSize = 0;

	if (argc != 2) {
		printf("ERROR\n");
		return 0;
	}

	// create SPConfig element
	configMsg = (SP_CONFIG_MSG *) malloc(sizeof(SP_CONFIG_MSG));

	SPConfig config = spConfigCreate(argv[1], configMsg);

	// Check for errors
	if (config == NULL) {
		printf("File: %s\n", argv[1]);
		printErrorType(configMsg);
		free(configMsg);
		return 0;
	}

	numOfImages = spConfigGetNumOfImages(config, configMsg);
	PCADim = spConfigGetPCADim(config, configMsg);
	imgProc = new sp::ImageProc(config);


	featureArr = (SPPoint**) malloc(sizeof(SPPoint*) * numOfImages);
	numOfFeatures = (int*) malloc(sizeof(int) *numOfImages);


	// Check extraction mode
	if (spConfigIsExtractionMode(config, configMsg) == true) {

		// start
		printf("time to extract! :)\n"); // TODO REMOVE
		//printConfig(config); // TODO REMOVE

		// For all images:
		for (i = 0; i < numOfImages; i++) {

			// Create image path
			spConfigGetImagePath(imagePath, config, i);
			// Get image features
			featureArr[i] = imgProc->getImageFeatures(imagePath, i, &(numOfFeatures[i]));

			k = spPointGetAxisCoor(featureArr[i][0],0);

			// Increase array size
			featureArrSize += numOfFeatures[i];

			// Create feature file with data
			spConfigGetImageFeatPath(imageFeaturePath, config, i);
			imageFeatureFile = fopen(imageFeaturePath, "w");
			fprintf(imageFeatureFile, "%d\n", i);
			fprintf(imageFeatureFile, "%d\n", numOfFeatures[i]);


			// Print all features to file
			for (j = 0; j < numOfFeatures[i]; j++) {
				for (k = 0; k < PCADim; k++) {
					fprintf(imageFeatureFile, "%lf,",
							spPointGetAxisCoor(featureArr[i][j], k));
				}
				fprintf(imageFeatureFile, "\n");
			}
			fclose(imageFeatureFile);
		}

		feature1DimArr = (SPPoint*) malloc(featureArrSize * sizeof(SPPoint));
		k = 0;
		for (i = 0; i < numOfImages; i++) {
			for (j = 0; j < numOfFeatures[i]; j++) {
				feature1DimArr[k] = spPointCopy(featureArr[i][j]);
				k++;
			}
		}

		// Now we just need to create the KDTree.
		KDTree = kdTreeInit(feature1DimArr, featureArrSize, PCADim,
				spConfigGetSplitMethod(config, configMsg));

	}

	// Non-Extraction Mode- here we need to create the KD-Tree from the feature files.
	else {
		// start
		printf("time to NOT extract! :)\n"); // TODO REMOVE
		//printConfig(config); //TODO REMOVE

		// For all images:
		tempDoubleArr = (double*) malloc(sizeof(double) * PCADim);
		for (i = 0; i < numOfImages; i++) {
			int imIndex;
			// Find feature file with data
			spConfigGetImageFeatPath(imageFeaturePath, config, i);
			imageFeatureFile = fopen(imageFeaturePath, "r");

			fscanf(imageFeatureFile, "%d\n", &imIndex);
			assert(i == imIndex);

			fscanf(imageFeatureFile, "%d\n", &numOfFeatures[i]);

			// Increase array size
			featureArrSize += numOfFeatures[i];

			// create array of points
			featureArr[i] = (SPPoint*) malloc(sizeof(SPPoint) * numOfFeatures[i]);


			// Get all features from file
			for (j = 0; j < numOfFeatures[i]; j++) {
				for (k = 0; k < PCADim; k++) {
					fscanf(imageFeatureFile, "%lf,", &(tempDoubleArr[k]));
				}
				fscanf(imageFeatureFile, "\n");
				featureArr[i][j] = spPointCreate(tempDoubleArr, PCADim, i);
			}
			fclose(imageFeatureFile);
		}
		free(tempDoubleArr);

		feature1DimArr = (SPPoint*) malloc(featureArrSize * sizeof(SPPoint));
		k = 0;
		for (i = 0; i < numOfImages; i++) {
			for (j = 0; j < numOfFeatures[i]; j++) {
				feature1DimArr[k] = spPointCopy(featureArr[i][j]);
				k++;
			}
		}

		// Now we just need to create the KDTree.
		KDTree = kdTreeInit(feature1DimArr, featureArrSize, PCADim,
				spConfigGetSplitMethod(config, configMsg));
	}


	for (i = 0; i < numOfImages; i++) {
		for (j = 0; j < numOfFeatures[i]; j++) {
			spPointDestroy(featureArr[i][j]);
		}
	}
	free(numOfFeatures);

	for (i = 0; i < numOfImages; i++) {
		free(featureArr[i]);
	}
	free(featureArr);

	for (i = 0; i < featureArrSize; i++) {
		spPointDestroy(feature1DimArr[i]);
	}
	free(feature1DimArr);


	// Enter the main loop
	while (true) {
		char query[LINE_LENGTH];
		SPPoint* features;
		int numOfBestImages, spKNN, numOfQueryFeatures, numOfImages;
		int* closestImages;
		bool minimalGui;

		memset(query, 0, sizeof(query));
		printf(ENTER_QUERY);
		fgets(query, LINE_LENGTH, stdin);

		if (strcmp("\n", query) == 0) {
			break;
		}

		strtok(query, "\n");


		numOfImages = spConfigGetNumOfImages(config, configMsg);
		spKNN = spConfigGetSPKNN(config, configMsg);
		numOfBestImages = spConfigGetNumOfSimilarImages(config, configMsg);

		features = imgProc->getImageFeatures(query, numOfImages,
				&numOfQueryFeatures);
		//printf("%d", spPointGetDimension(features[3]));

		closestImages = bestImages(numOfBestImages, spKNN, KDTree, features,
				numOfQueryFeatures, numOfImages);


		minimalGui = spConfigMinimalGui(config, configMsg);

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


		for (i = 0 ; i< numOfQueryFeatures ; i++)
		{
			spPointDestroy(features[i]);
		}
		free(features);
		free(closestImages);
	}


	printf(EXITING);
	kdTreeNodeDestroy(KDTree);
	free(configMsg);
	free(config);
	delete imgProc;
	return 0;

}
