#include<stdio.h>
#include<cstdlib>

#include "SPImageProc.h"

extern "C"{
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

	numOfImages =  spConfigGetNumOfImages(config, configMsg);

	// Check extraction mode
	if (spConfigIsExtractionMode(config, configMsg) == true) {
		printf("time to extract! :)");
		imgProc = new sp::ImageProc(config);

		for(i = 0; i < numOfImages; i++) {
			spConfigGetImagePath(imagePath, config, i);
			featureArr = imgProc->getImageFeatures(imagePath, i, &numOfFeatures);
		}
	}
	return 0;
}
