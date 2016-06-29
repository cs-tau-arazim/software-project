#include<stdio.h>
#include<cstdlib>

#include "SPImageProc.h"

extern "C"{
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

	numOfImages =  spConfigGetNumOfImages(config, configMsg);
	PCADim = spConfigGetPCADim(config, configMsg);

	// Check extraction mode
	if (spConfigIsExtractionMode(config, configMsg) == true) {
		printf("time to extract! :)");
		imgProc = new sp::ImageProc(config);

		// For all images:
		for(i = 0; i < numOfImages; i++) {

			// Create image path
			spConfigGetImagePath(imagePath, config, i);
			// Get image features
			featureImageArr = imgProc->getImageFeatures(imagePath, i, &numOfFeatures);

			// Increase array size
			featureArrSize += numOfFeatures;

			// add more memory
			featureArr = (SPPoint*)realloc(featureArr, featureArrSize * sizeof(SPPoint));
			// copy points
			for (j = 0; j < numOfFeatures; j++) {
				featureArr[(featureArrSize - numOfFeatures -1) + i] = featureImageArr[i];
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
		KDTree =  kdTreeInit(featureArr, featureArrSize, PCADim, spConfigGetSplitMethod(config, configMsg));
	}

	// Non-Extraction Mode- here we need to create the KD-Tree from the feature files.
	else {
		printf("YAY");
	}
	return 0;
}
