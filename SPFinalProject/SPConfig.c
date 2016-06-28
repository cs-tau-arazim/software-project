#include "SPConfig.h"

#define LINE_LENGTH 1024

/*
 * SPConfig.c
 *
 *  Created on: Jun 27, 2016
 *      Author: Gal
 */

/**
 * Creates a new system configuration struct. The configuration struct
 * is initialized based on the configuration file given by 'filename'.
 *
 * @param filename - the name of the configuration file
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return NULL in case an error occurs. Otherwise, a pointer to a struct which
 * 		   contains all system configuration.
 *
 * The resulting value stored in msg is as follow:
 * - SP_CONFIG_INVALID_ARGUMENT - if filename == NULL
 * - SP_CONFIG_CANNOT_OPEN_FILE - if the configuration file given by filename cannot be open
 * - SP_CONFIG_ALLOC_FAIL - if an allocation failure occurred
 * - SP_CONFIG_INVALID_INTEGER - if a line in the config file contains invalid integer
 * - SP_CONFIG_INVALID_STRING - if a line in the config file contains invalid string
 * - SP_CONFIG_MISSING_DIR - if spImagesDirectory is missing
 * - SP_CONFIG_MISSING_PREFIX - if spImagesPrefix is missing
 * - SP_CONFIG_MISSING_SUFFIX - if spImagesSuffix is missing
 * - SP_CONFIG_MISSING_NUM_IMAGES - if spNumOfImages is missing
 * - SP_CONFIG_SUCCESS - in case of success
 *
 *
 */
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
	// define variables
	FILE * configFilePtr;
	char * lineStr;
	int read;
	assert(msg != NULL); // assertion

	configFilePtr = fopen(filename, "r");
	if (configFilePtr == NULL) { // edge case
		(*msg) = SP_CONFIG_CANNOT_OPEN_FILE;
		return NULL;
	}

	/*while ((read = getline(&lineStr, &len, fp)) != -1) {
		printf("Retrieved line of length %zu :\n", read);
		printf("%s", line);
	}

	fclose(fp);
	if (lineStr)
		free(lineStr);*/

	fgets(lineStr, LINE_LENGTH, configFilePtr);
	//fscanf(configFilePtr,//somthing )
	// now we need to read from the file
	fclose(configFilePtr);
}
