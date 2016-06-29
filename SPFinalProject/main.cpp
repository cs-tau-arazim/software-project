#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "SPImageProc.h"

#define ENTER_QUERY "Please enter an image path:\n"
#define EXITING "Exiting..."
extern "C"{
#include "main_aux.h"
}

int main(int argc, char **argv) {

	char query[LINE_LENGTH];
	SP_CONFIG_MSG * configMsg;
	printf("Hello World");

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



	// Enter the main loop
	while(true)
	{
		SP_CONFIG_MSG* msg;
		SPPoint* features;
		int numOfBestImages, spKNN, numOfFeatures, numOfImages;
		int* closestImages;

		msg = (SP_CONFIG_MSG*)malloc(sizeof(SP_CONFIG_MSG));
		memset(query, 0, sizeof (query));
		printf(ENTER_QUERY);
		fgets(query,LINE_LENGTH, stdout);
		strtok(query, "\n");

		if(strcmp("#",query) == 0)
		{
			printf(EXITING);
			// TODO free all mem
			return 0;
		}
		// TODO get image features

		numOfImages = spConfigGetNumOfImages(config, msg); // TODO check if success
		numOfFeatures = spConfigGetNumOfFeatures(config, msg); // TODO check if success
		spKNN = spConfigGetSPKNN(config, msg); // TODO check if success
		numOfBestImages = spConfigGetNumOfSimilarImages(config, msg); // TODO check if success

		//closestImages = bestImages(numOfBestImages, spKNN, kdTree, features, numOfFeatures, numOfImages);



	}
	return 0;
}
