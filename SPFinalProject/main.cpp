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


	SP_CONFIG_MSG * configMsg;
	SPPoint * featureArr;
	sp::ImageProc *imgProc;
	int i, numOfImages;
	char imagePath[LINE_LENGTH];
	int numOfFeatures;
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

	// Check extraction mode
	if (spConfigIsExtractionMode(config, configMsg) == true) {
		printf("time to extract! :)");
		imgProc = new sp::ImageProc(config);

		for (i = 0; i < numOfImages; i++) {
			spConfigGetImagePath(imagePath, config, i);
			featureArr = imgProc->getImageFeatures(imagePath, i,
					&numOfFeatures);
		}

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

		features = imgProc->getImageFeatures(query, numOfImages, &numOfFeatures);

		closestImages = bestImages(numOfBestImages, spKNN, kdTree, features, numOfFeatures, numOfImages);

		minimalGui = spConfigMinialGui(config, configMsg);

		if(!minimalGui)
		{
			int i;
			printf(BEST_CANDIDATES, query);
			for (i = 0 ; i < numOfBestImages ; i++)
			{
				char imagePath[LINE_LENGTH];

				spConfigGetImagePath(imagePath, config, closestImages[i]); // TODO check if fails
				printf("%s\n" , imagePath);
			}
		}
		else
		{
			for (i = 0 ; i < numOfBestImages ; i++)
			{
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
