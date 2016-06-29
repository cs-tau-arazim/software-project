#include<stdio.h>
#include<cstdlib>
#include "SPImageProc.h"

extern "C" {
#include "main_aux.h"
#include "SPPoint.h"
#include "SPConfig.h"
#include "KDArray.h"
}

int main(int argc, char **argv) {
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

	return 0;
}
