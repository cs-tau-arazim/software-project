#include "SPConfig.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define LINE_LENGTH 1024

#define MSG_DIR_NOT_SET "Message: Parameter spImagesDirectory is not set\n"
#define MSG_PREFIX_NOT_SET "Message: Parameter spImagesPrefix is not set\n"
#define MSG_SUFFIX_NOT_SET "Message: Parameter spImagesSuffix is not set\n"
#define MSG_NUM_NOT_SET "Message: Parameter spNumOfImages is not set\n"
#define MSG_INVALID_LINE "Message: Invalid configuration Line\n"
#define MSG_INVALID_VALUE "Message: Invalid value - constraint not met\n"

#define INVALID_FILENAME "Error: Invalid filename\n"
#define FILE_OPEN_FAIL "The configuration file %s couldn't be opened\n"
#define DEFAULT_FILE_OPEN_FAIL "The default configuration file spcbir.config couldn't be opened\n"
#define ALLOCATION_ERROR "Allocation Error - failed to allocate memory for configuration file\n"

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
	int lineNum = 1;
	FILE * configFilePtr;
	char bufferLine[LINE_LENGTH];

	// Array for which variables were set
	bool setArray[] = { false, false, false, false };

	// assertion
	assert(msg != NULL);

	// Edge cases
	if (filename == NULL ) {
		(*msg) = SP_CONFIG_INVALID_ARGUMENT;
		printf(INVALID_FILENAME);
		return NULL ;
	}

	configFilePtr = fopen(filename, "r");
	if (configFilePtr == NULL ) {
		(*msg) = SP_CONFIG_CANNOT_OPEN_FILE;
		if (strcmp(filename, "spcbir.config") == 0)
			printf(DEFAULT_FILE_OPEN_FAIL);
		else
			printf(FILE_OPEN_FAIL, filename);
		return NULL ;
	}

	// Allocate Config
	SPConfig config = (SPConfig) malloc(sizeof(*config));
	if (config == NULL ) { // Allocation failure
		(*msg) = SP_CONFIG_ALLOC_FAIL;
		printf(ALLOCATION_ERROR);
		fclose(configFilePtr);
		return NULL ;
	}

	// Set all default values at the beginning
	setDefaultValues(config);

	// Iterate the file's lines
	while (fgets(bufferLine, LINE_LENGTH, configFilePtr) != NULL ) {
		//strtok(bufferLine, "\n"); todo check
		// Get the relevant info
		res = extractInfoFromLine(config, msg, bufferLine, lineNum, filename,
				setArray);
		// if error occured

		if (res == 1) {
			free(config);
			fclose(configFilePtr);
			return NULL ;
		}
		lineNum++;
	}

	// Now we just need to check if any which values were not set yet
	if (setArray[0] == false) {
		(*msg) = SP_CONFIG_MISSING_DIR;
		free(config);
		fclose(configFilePtr);
		printConfigError(filename, lineNum - 1, MSG_DIR_NOT_SET);
		return NULL ;
	}
	if (setArray[1] == false) {
		(*msg) = SP_CONFIG_MISSING_PREFIX;
		free(config);
		fclose(configFilePtr);
		printConfigError(filename, lineNum - 1, MSG_PREFIX_NOT_SET);
		return NULL ;
	}
	if (setArray[2] == false) {
		(*msg) = SP_CONFIG_MISSING_SUFFIX;
		free(config);
		fclose(configFilePtr);
		printConfigError(filename, lineNum - 1, MSG_SUFFIX_NOT_SET);
		return NULL ;
	}
	if (setArray[3] == false) {
		(*msg) = SP_CONFIG_MISSING_NUM_IMAGES;
		free(config);
		fclose(configFilePtr);
		printConfigError(filename, lineNum - 1, MSG_NUM_NOT_SET);
		return NULL ;
	}

	// Success!
	fclose(configFilePtr);
	return config;
}

/**
 * Helper function to print the appropriate message
 * for each error received by spConfigCreate.
 */
void printConfigError(const char * filename, int lineNum, char * message) {
	printf("File: %s\n", filename);
	printf("Line: %d\n", lineNum);
	printf("%s", message);
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
 * The function receives the line we intend on scanning, and some othre relevant parameters.
 * it reads the line and checks whether it is a vaild configuration line.
 * If vaild - updates relevant field in config and returns 0.
 * If invalid - updates (msg) to be the correct output message, and returns -1.
 */
int extractInfoFromLine(SPConfig config, SP_CONFIG_MSG * msg, char * bufferLine,
		int lineNum, const char * filename, bool * setArray) {
	int i, assignRes;
	int varNameStart, varNameLen, varValueStart, varValueLen; // Parameter for tracking varName and varValue
	bool cmpRes;
	char bufferVar[LINE_LENGTH] = { 0 };
	char bufferParam[LINE_LENGTH] = { 0 };
	if (bufferLine[strlen(bufferLine) - 1] != '\n')
		strcat(bufferLine, "\n");
	// Array for scanning the input and comparing with possible values
	const char * const varArray[] = { "spImagesDirectory", "spImagesPrefix",
			"spImagesSuffix", "spNumOfImages", "spPCADimension",
			"spPCAFilename", "spNumOfFeatures", "spExtractionMode",
			"spNumOfSimilarImages", "spKDTreeSplitMethod", "spKNN",
			"spMinimalGUI", "spLoggerLevel", "spLoggerFilename" };

	const int varArraySize = 14;
	i = 0;
	while (bufferLine[i] == ' ' || bufferLine[i] == '\t' || bufferLine[i] == '\r') // Ignore whitespace before variable name
		i++;
	if (bufferLine[i] == '#' || bufferLine[i] == '\n') // Either comment or empty line
		return 0;
	varNameStart = i;
	while (bufferLine[i] != ' ' && bufferLine[i] != '\t' && bufferLine[i] != '=') // Read variable name
	{
		// If line ends amidst variable name
		if (bufferLine[i] == '\n' || bufferLine[i] == '\r') {
			printConfigError(filename, lineNum, MSG_INVALID_LINE);
			*msg = SP_CONFIG_INVALID_STRING;
			return 1;
		}
		i++;
	}

	varNameLen = i - varNameStart;
	while (bufferLine[i] == ' ' || bufferLine[i] == '\t') // Ignore whitespace after variable name
		i++;
	if (bufferLine[i] != '=') // Next non-whitespace character must be equals
			{
		printConfigError(filename, lineNum, MSG_INVALID_LINE);
		*msg = SP_CONFIG_INVALID_STRING;
		return 1;
	}
	i++;

	while (bufferLine[i] == ' ' || bufferLine[i] == '\t') // Ignore whitespace before variable value
		i++;
	varValueStart = i;

	while (bufferLine[i] != ' ' && bufferLine[i] != '\t'
			&& bufferLine[i] != '\n' && bufferLine[i] != '\r') // Read variable value
		i++;

	varValueLen = i - varValueStart;

	while (bufferLine[i] == ' ' || bufferLine[i] == '\t') // Ignore whitespace after variable value
		i++;

	if (bufferLine[i] != '\n' && bufferLine[i] != '\r') // Next non-whitespace character must be new line
			{
		printConfigError(filename, lineNum, MSG_INVALID_LINE);
		*msg = SP_CONFIG_INVALID_STRING;
		return 1;
	}

	strncpy(bufferVar, bufferLine + varNameStart, varNameLen); // Copy variable name
	strncpy(bufferParam, bufferLine + varValueStart, varValueLen); // Copy variable value

	cmpRes = false; // flag for whether we found a match
	// compare with all variables
	for (i = 0; i < varArraySize; i++) {
		// find match
		if (strcmp(bufferVar, varArray[i]) == 0) {

			// update boolean flags
			cmpRes = true;
			if (i <= 3)
				setArray[i] = true;

			// check which variable should be updated
			if (i == 0)
				assignRes = setSpImagesDirectory(config, bufferParam);
			else if (i == 1)
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
				assignRes = setSpNumOfSimilarImages(config, bufferParam);
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

			// Check if assignment failed
			if (assignRes != 0) {
				// print Error
				printConfigError(filename, lineNum, MSG_INVALID_VALUE);

				// Integer error
				if (i == 3 || i == 4 || i == 6 || i == 8 || i == 10
						|| i == 12) {
					(*msg) = SP_CONFIG_INVALID_INTEGER;
					return 1;
				}
				// String error
				else {
					(*msg) = SP_CONFIG_INVALID_STRING;
					return 1;
				}
			}
			break;
		}
	}
	// if no match was found
	if (cmpRes == false) {
		(*msg) = SP_CONFIG_INVALID_STRING;
		printConfigError(filename, lineNum, MSG_INVALID_LINE);
		return 1;
	}

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

SP_CONFIG_MSG spConfigGetLoggerFileName(char* name, const SPConfig config) {
	if (config == NULL || name == NULL ) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	sprintf(name, "%s", config->spLoggerFilename);
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetLoggerLevel(int* level, const SPConfig config) {
	if (config == NULL ) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	*level = config->spLoggerLevel;
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
