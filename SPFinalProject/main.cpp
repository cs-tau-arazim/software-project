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
	char imagePath[LINE_LENGTH];
	char imageFeaturePath[LINE_LENGTH];
	FILE * imageFeatureFile;

	int* numOfFeatures;
	int featureArrSize = 0;

	printf("Hello World\n");

	// TODO check if config file received through cmd
	if (argc != 2) {
		printf("ERROR");
		return 0;
	}

	// create SPConfig element
	configMsg = (SP_CONFIG_MSG *) malloc(sizeof(SP_CONFIG_MSG));
	SPConfig config = spConfigCreate(argv[1], configMsg);



	// Check for errors
	if (config == NULL) {
		printf("File: %s\n", argv[1]);
		printErrorType(configMsg);
		free (configMsg);
		return 0;
	}

	printf("%d, %s\n",__LINE__, __func__); //TODO remove
	numOfImages = spConfigGetNumOfImages(config, configMsg);
	PCADim = spConfigGetPCADim(config, configMsg);

	// Check extraction mode
	if (spConfigIsExtractionMode(config, configMsg) == true) {
		printf("time to extract! :)\n");
		printConfig(config);
		imgProc = new sp::ImageProc(config);
		printf("%d, %s\n",__LINE__, __func__); //TODO remove

		featureArr = (SPPoint**) malloc(sizeof(SPPoint*) * numOfImages);
		numOfFeatures = (int*) malloc(sizeof(int) * numOfImages);

		// For all images:
		for (i = 1; i < numOfImages; i++) { // TODO change to 0!!!!!!!!!

			// Create image path
			spConfigGetImagePath(imagePath, config, i);
			// Get image features
			featureArr[i] = imgProc->getImageFeatures(imagePath, i, &(numOfFeatures[i]));

			// Increase array size
			featureArrSize += numOfFeatures[i];

			// add more memory
			//featureArr = (SPPoint*) realloc(featureArr,featureArrSize * sizeof(SPPoint));
			// copy points
			//for (j = 0; j < numOfFeatures; j++) {
			//	featureArr[(featureArrSize - numOfFeatures - 1) + i] =
			//			featureImageArr[i];
			//}

			// Create feature file with data
			spConfigGetImageFeatPath(imageFeaturePath, config, i);
			imageFeatureFile = fopen(imageFeaturePath, "w");
			fprintf(imageFeatureFile, "%d\n", i);
			fprintf(imageFeatureFile, "%d\n", numOfFeatures[i]);

			printf("%d, %s\n",__LINE__, __func__); //TODO remove

			// Print all features to file
			for (j = 0; j < numOfFeatures[i]; j++) {
				for (k = 0; k < PCADim; k++) {
					fprintf(imageFeatureFile, "%lf,", spPointGetAxisCoor(featureArr[i][j],k));
				}
				fprintf(imageFeatureFile, "\n,");
			}
			fclose(imageFeatureFile);
		}

		feature1DimArr = (SPPoint*)malloc(featureArrSize*sizeof(SPPoint));
		k = 0;
		for (i = 1 ; i < numOfImages ; i++)
		{
			for (j = 0 ; j < numOfFeatures[i] ; j++)
			{
				feature1DimArr[k] = spPointCopy(featureArr[i][j]);
				k++;
			}
		}

		printf("%d, %s\n",__LINE__, __func__); //TODO remove


		// Now we just need to create the KDTree.
		KDTree = kdTreeInit(feature1DimArr, featureArrSize, PCADim,
				spConfigGetSplitMethod(config, configMsg));
		printf("%d, %s\n",__LINE__, __func__); //TODO remove

		for (i = 0 ; i < featureArrSize ; i++)
		{
			free(feature1DimArr[i]);
		}
		free(feature1DimArr);
		free(numOfFeatures);

		for (i = 0 ; i < numOfImages ; i++)
		{
			free(featureArr[i]);
		}
		free(featureArr);
		printf("%d, %s\n",__LINE__, __func__); //TODO remove

	}

	// Non-Extraction Mode- here we need to create the KD-Tree from the feature files.

	// Non-Extraction Mode- here we need to create the KD-Tree from the feature files.
	else {
		printf("YAY");

	}
	printf("%d, %s\n",__LINE__, __func__); //TODO remove
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
