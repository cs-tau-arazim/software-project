#include "SPConfig.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define LINE_LENGTH 1024

/*
 * SPConfig.c
 *
 *  Created on: Jun 27, 2016
 *      Author: Gal
 */

struct sp_config_t {
	char spImagesDirectory[LINE_LENGTH];
	char spImagesPrefix[LINE_LENGTH];
	char spImagesSuffix[LINE_LENGTH];
	int spNumOfImages;
	int spPCADimension; // default value = 20
	char spPCAFilename[LINE_LENGTH]; // default value = pca.yml
	int spNumOfFeatures; // default value = 100
	bool spExtractionMode; // default value = true
	int spNumOfSimilarImages; // default value = 1
	enum {
		RANDOM, MAX_SPREAD, INCREMENTAL
	} spKDTreeSplitMethod; // default value = MAX_SPREAD
	int spKNN; // default value = 1
	bool spMinimalGUI; // default value = false
	int spLoggerLevel; // default value = 3
	char spLoggerFilename[LINE_LENGTH]; // default value = stdout
};

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

	// Define variables
	int res = 0;
	int i;
	int lineNum = 0;
	int cmpRes;
	int assignRes = 0;
	FILE * configFilePtr;
	char bufferVar[LINE_LENGTH];
	char bufferParam[LINE_LENGTH];
	char bufferLine[LINE_LENGTH];

	// Array for scanning the input and comparing with possible values
	const char * const varArray[] = { "spImagesDirectory", "spImagesPrefix",
			"spImagesSuffix", "spNumOfImages", "spPCADimension",
			"spPCAFilename", "spNumOfFeatures", "spExtractionMode",
			"spNumOfSimilarImages", "spKDTreeSplitMethod", "spKNN",
			"spMinimalGUI", "spLoggerLevel", "spLoggerFilename" };

	// Array for which variables were set
	bool setArray[] = { false, false, false, false };

	const int varArraySize = 14;
	assert(msg != NULL);
	// assertion

	if (filename == NULL ) {
		(*msg) = SP_CONFIG_INVALID_ARGUMENT;
		return NULL ;
	}

	configFilePtr = fopen(filename, "r");
	if (configFilePtr == NULL ) { // edge case
		(*msg) = SP_CONFIG_CANNOT_OPEN_FILE;
		return NULL ;
	}

	SPConfig config = (SPConfig) malloc(sizeof(*config));
	if (config == NULL ) { // Allocation failure
		(*msg) = SP_CONFIG_ALLOC_FAIL;
		return NULL ;
	}

	// Set all default values at the beginning
	setDefaultValues(config);

	// Iterate the file's lines
	while (fgets(bufferLine, LINE_LENGTH, configFilePtr) != NULL ) {

		strtok(bufferLine, "\n");

		res = checkValid(bufferLine, bufferVar, bufferParam);

		if (res == 2) {
			(*msg) = SP_CONFIG_INVALID_STRING;
			free(config);
			return NULL ;
		}
		// if (res == 1) , ignore comment and move on
		if (res == 0) {
			// compare with all variables
			for (i = 0; i < varArraySize; i++) {
				cmpRes = strcmp(bufferVar, varArray[i]);
				// find match
				if (cmpRes == 0) {
					// update boolean flag
					if (i <= 3)
						setArray[i] = true;

					if (i == 0) {
						assignRes = setSpImagesDirectory(config, bufferParam);
					} else if (i == 1)
						assignRes = setSpImagesPrefix(config, bufferParam);
					else if (i == 2)
						assignRes = setSpImagesSuffix(config, bufferParam);
					else if (i == 3)
						assignRes = setSpNumOfImages(config, bufferParam);
					else if (i == 4)
						assignRes = setSpPCADimension(config, bufferParam);
					else if (i == 5)
						assignRes = setSpPCAFilename(config, bufferParam);
					else if (i == 6)
						assignRes = setSpNumOfFeatures(config, bufferParam);
					else if (i == 7)
						assignRes = setSpExtractionMode(config, bufferParam);
					else if (i == 8)
						assignRes = setSpNumOfSimilarImages(config,
								bufferParam);
					else if (i == 9)
						assignRes = setSpKDTreeSplitMethod(config, bufferParam);
					else if (i == 10)
						assignRes = setSpKNN(config, bufferParam);
					else if (i == 11)
						assignRes = setSpMinimalGUI(config, bufferParam);
					else if (i == 12)
						assignRes = setSpLoggerLevel(config, bufferParam);
					else if (i == 13)
						assignRes = setSpLoggerFilename(config, bufferParam);

					// Check if succeeded
					if (assignRes != 0) {
						// print line nubmer
						printf("Error in line #%d", lineNum);
						// Integer error
						if (i == 3 || i == 4 || i == 6 || i == 8 || i == 10
								|| i == 12) {
							(*msg) = SP_CONFIG_INVALID_INTEGER;
							free(config);
							return NULL ;
						} else {
							(*msg) = SP_CONFIG_INVALID_STRING;
							free(config);
							return NULL ;
						}
					}
					break;
				}
			}
		}
		lineNum++;
	}

	// Now we just need to check if any which values were not set yet
	if (setArray[0] == false) {
		(*msg) = SP_CONFIG_MISSING_DIR;
		free(config);
		return NULL ;
	}
	if (setArray[1] == false) {
		(*msg) = SP_CONFIG_MISSING_PREFIX;
		free(config);
		return NULL ;
	}
	if (setArray[2] == false) {
		(*msg) = SP_CONFIG_MISSING_DIR;
		free(config);
		return NULL ;
	}
	if (setArray[3] == false) {
		(*msg) = SP_CONFIG_MISSING_NUM_IMAGES;
		free(config);
		return NULL ;
	}

	// Success!
	fclose(configFilePtr);
	return config;
}

/**
 * receives an SPConfig type,
 * and sets all of its default values as stated in the project documentation.
 */
void setDefaultValues(SPConfig config) {
	config->spPCADimension = 20;
	strcpy(config->spPCAFilename, "pca.yml");
	config->spNumOfFeatures = 100;
	config->spExtractionMode = true;
	config->spMinimalGUI = false;
	config->spNumOfSimilarImages = 1;
	config->spKNN = 1;
	config->spPCADimension = 20;
	config->spKDTreeSplitMethod = MAX_SPREAD;
	config->spLoggerLevel = 3;
	strcpy(config->spLoggerFilename, "stdout");
}

/*
 * checks bufferLine is in correct format.
 * If comment or empty- returns 1, leaves var and param untouched.
 * If invalid- returns 2, leaves var and param untouched.
 * If bufferLine is NULL, also returns 2.
 * If valid- returns 0, puts in var the variable and puts the parameter in param.
 */
int checkValid(char * bufferLine, char * var, char * param) {
	char varIn[LINE_LENGTH];
	char paramIn[LINE_LENGTH];
	char * endCheck;
	const char split[3] = " =\t";

	if (bufferLine == NULL || strcmp(bufferLine, "\n") == 0)
		return 1;

	// Get strings
	strcpy(varIn, strtok(bufferLine, split));
	strcpy(paramIn, strtok(NULL, split));
	endCheck = (char*) strtok(NULL, split);

	if (varIn == NULL )
		return 1;

	// Check if comment
	if (varIn[0] == '#')
		return 1;

	// If there are more strings, should error
	if (endCheck != NULL )
		return 2;

	// If not enough strings, should error
	if (paramIn == NULL )
		return 2;

	// Else, set pointers var and param and then return success
	strcpy(var, varIn);
	strcpy(param, paramIn);

	return 0;
}

/**
 * Group of "set" functions for each field.
 * each has its own edge cases,
 * and returns 0 on success or 1 on fail.
 */
int setSpImagesDirectory(SPConfig config, char * bufferParam) {
	strcpy(config->spImagesDirectory, bufferParam);
	return 0;
}

int setSpImagesPrefix(SPConfig config, char * bufferParam) {
	strcpy(config->spImagesPrefix, bufferParam);
	return 0;
}

int setSpImagesSuffix(SPConfig config, char * bufferParam) {
	// Check if correct format
	if (!(strcmp(bufferParam, ".jpg") == 0 || strcmp(bufferParam, ".png") == 0
			|| strcmp(bufferParam, ".bmp") == 0
			|| strcmp(bufferParam, ".gif") == 0))
		return 1;

	strcpy(config->spImagesSuffix, bufferParam);
	return 0;
}

int setSpNumOfImages(SPConfig config, char * bufferParam) {
	int num = atoi(bufferParam);
	if (num <= 0)
		return 1;

	config->spNumOfImages = num;
	return 0;
}

int setSpPCADimension(SPConfig config, char * bufferParam) {
	int num = atoi(bufferParam);
	if (num < 10 || num > 28)
		return 1;

	config->spPCADimension = num;
	return 0;
}

int setSpPCAFilename(SPConfig config, char * bufferParam) {
	strcpy(config->spPCAFilename, bufferParam);
	return 0;
}

int setSpNumOfFeatures(SPConfig config, char * bufferParam) {
	int num = atoi(bufferParam);
	if (num <= 0)
		return 1;

	config->spNumOfFeatures = num;
	return 0;
}

int setSpExtractionMode(SPConfig config, char * bufferParam) {
	// Check if is indeed of boolean type
	if (strcmp(bufferParam, "true") == 0) {
		config->spExtractionMode = true;
		return 0;
	} else if (strcmp(bufferParam, "false") == 0) {
		config->spExtractionMode = false;
		return 0;
	} else {
		return 1;
	}
}

int setSpNumOfSimilarImages(SPConfig config, char * bufferParam) {
	int num = atoi(bufferParam);
	if (num <= 0)
		return 1;

	config->spNumOfSimilarImages = num;
	return 0;
}

int setSpKDTreeSplitMethod(SPConfig config, char * bufferParam) {
	// Check if is indeed of splitMethod enum type type
	if (strcmp(bufferParam, "RANDOM") == 0) {
		config->spKDTreeSplitMethod = RANDOM;
		return 0;
	} else if (strcmp(bufferParam, "MAX_SPREAD") == 0) {
		config->spKDTreeSplitMethod = MAX_SPREAD;
		return 0;
	} else if (strcmp(bufferParam, "INCREMENTAL") == 0) {
		config->spKDTreeSplitMethod = INCREMENTAL;
		return 0;
	} else {
		return 1;
	}
}

int setSpKNN(SPConfig config, char * bufferParam) {
	int num = atoi(bufferParam);
	if (num <= 0)
		return 1;

	config->spKNN = num;
	return 0;
}

int setSpMinimalGUI(SPConfig config, char * bufferParam) {
	// Check if is indeed of boolean type
	if (strcmp(bufferParam, "true") == 0) {
		config->spMinimalGUI = true;
		return 0;
	} else if (strcmp(bufferParam, "false") == 0) {
		config->spMinimalGUI = false;
		return 0;
	} else {
		return 1;
	}
}

int setSpLoggerLevel(SPConfig config, char * bufferParam) {
	int num = atoi(bufferParam);
	if (num <= 0 || num > 4)
		return 1;

	// Check if is a correct spLogger level
	else if (num == 1 || num == 2 || num == 3 || num == 4) {
		config->spLoggerLevel = num;
		return 0;
	} else {
		return 1;
	}
}

int setSpLoggerFilename(SPConfig config, char * bufferParam) {
	strcpy(config->spLoggerFilename, bufferParam);
	return 0;
}

/*
 * Returns true if spExtractionMode = true, false otherwise.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spExtractionMode = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL ) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;

	return config->spExtractionMode;

}

/*
 * Returns true if spMinimalGUI = true, false otherwise.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spMinimalGUI = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL ) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;

	return config->spMinimalGUI;

}

/*
 * Returns the number of images set in the configuration file, i.e the value
 * of spNumOfImages.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL ) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfImages;
}

/*
 * Returns the number of features to be extracted. i.e the value
 * of spNumOfFeatures.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL ) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfFeatures;
}

/*
 * Returns the number of images to consider close. i.e the value
 * of spNumOfSimilarImages.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetNumOfSimilarImages(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL ) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfSimilarImages;
}

/*
 * Returns the number of features to consider close. i.e the value
 * of spKNN.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetSPKNN(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL ) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spKNN;
}

/**
 * Returns the dimension of the PCA. i.e the value of spPCADimension.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL ) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spPCADimension;
}

/**
 * Returns the split Method given in the SPConfig type.
 * returns an INT and not an enum-
 * this way, RANDOM = 0, MAX_SPREAD = 1 and INCREMENTAL = 2.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetSplitMethod(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL ) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spKDTreeSplitMethod;
}

/**
 * Given an index 'index' the function stores in imagePath the full path of the
 * ith image.
 *
 * For example:
 * Given that the value of:
 *  spImagesDirectory = "./images/"
 *  spImagesPrefix = "img"
 *  spImagesSuffix = ".png"
 *  spNumOfImages = 17
 *  index = 10
 *
 * The functions stores "./images/img10.png" to the address given by imagePath.
 * Thus the address given by imagePath must contain enough space to
 * store the resulting string.
 *
 * @param imagePath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @param index - the index of the image.
 *
 * @return
 * - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 * - SP_CONFIG_INDEX_OUT_OF_RANGE - if index >= spNumOfImages
 * - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,
		int index) {

	if (config == NULL || imagePath == NULL )
		return SP_CONFIG_INVALID_ARGUMENT;
	if (index >= config->spNumOfImages)
		return SP_CONFIG_INDEX_OUT_OF_RANGE;

	char iStr[LINE_LENGTH];
	memset(iStr, 0, sizeof iStr);

	sprintf(iStr, "%d", index);
	// Create directory string as: dir+prefix+i+suffix
	strcpy(imagePath, config->spImagesDirectory);
	strcat(imagePath, config->spImagesPrefix);
	strcat(imagePath, iStr);
	strcat(imagePath, config->spImagesSuffix);

	return SP_CONFIG_SUCCESS;
}

/**
 * Given an index 'index' the function stores in imagePath the full path of the
 * ith image's FEATURE.
 *
 * For example:
 * Given that the value of:
 *  spImagesDirectory = "./images/"
 *  spImagesPrefix = "img"
 *  spImagesSuffix = ".png"
 *  spNumOfImages = 17
 *  index = 10
 *
 * The functions stores "./images/img10.feats" to the address given by imagePath.
 * Thus the address given by imagePath must contain enough space to
 * store the resulting string.
 *
 * @param imagePath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @param index - the index of the image.
 *
 * @return
 * - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 * - SP_CONFIG_INDEX_OUT_OF_RANGE - if index >= spNumOfImages
 * - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetImageFeatPath(char* imagePath, const SPConfig config,
		int index) {

	if (config == NULL || imagePath == NULL )
		return SP_CONFIG_INVALID_ARGUMENT;
	if (index >= config->spNumOfImages)
		return SP_CONFIG_INDEX_OUT_OF_RANGE;

	char iStr[LINE_LENGTH];
	memset(iStr, 0, sizeof iStr);

	sprintf(iStr, "%d", index);
	// Create directory string as: dir+prefix+i+suffix
	strcpy(imagePath, config->spImagesDirectory);
	strcat(imagePath, config->spImagesPrefix);
	strcat(imagePath, iStr);
	strcat(imagePath, ".feats");

	return SP_CONFIG_SUCCESS;
}


/**
 * The function stores in pcaPath the full path of the pca file.
 * For example given the values of:
 *  spImagesDirectory = "./images/"
 *  spPcaFilename = "pca.yml"
 *
 * The functions stores "./images/pca.yml" to the address given by pcaPath.
 * Thus the address given by pcaPath must contain enough space to
 * store the resulting string.
 *
 * @param imagePath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @return
 *  - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 *  - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config) {
	if (config == NULL || pcaPath == NULL )
		return SP_CONFIG_INVALID_ARGUMENT;

	// Create directory string as: spImagesDirectory+spPcaFilename
	strcpy(pcaPath, config->spImagesDirectory);

	strcat(pcaPath, config->spPCAFilename);

	return SP_CONFIG_SUCCESS;
}

/**
 * Frees all memory resources associate with config.
 * If config == NULL nothig is done.
 */
void spConfigDestroy(SPConfig config) {
	if (config == NULL )
		return;
	free(config);
}


/**
 * SPCOnfig printer for debugging.
 */
void printConfig(SPConfig config) {
	printf("Config:\n");
	printf("spImagesDirectory: %s\n", config->spImagesDirectory);
	printf("spImagesPrefix: %s\n", config->spImagesPrefix);
	printf("spImagesSuffix: %s\n", config->spImagesSuffix);
	printf("spNumOfImages: %d\n", config->spNumOfImages);
	printf("spPCADimension: %d\n", config->spPCADimension);
	printf("spPCAFilename: %s\n", config->spPCAFilename);
	printf("spNumOfFeatures: %d\n", config->spNumOfFeatures);
	printf("spExtractionMode: %d\n", config->spExtractionMode);
	printf("spNumOfSimilarImages: %d\n", config->spNumOfSimilarImages);
	printf("spKDTreeSplitMethod: %d\n", config->spKDTreeSplitMethod);
	printf("spKNN: %d\n", config->spKNN);
	printf("spMinimalGUI: %d\n", config->spMinimalGUI);
	printf("spLoggerLevel: %d\n", config->spLoggerLevel);
	printf("spLoggerFilename: %s\n", config->spLoggerFilename);
}
