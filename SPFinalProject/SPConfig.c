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


struct sp_config_t{
	char* spImagesDirectory;
	char* spImagesPrefix;
	char* spImagesSuffix;
	int spNumOfImages;
	int spPCADimension; // default value = 20
	char* spPCAFilename; // default value = pca.yml
	int spNumOfFeatures; // default value = 100
	bool spExtractionMode; // default value = true
	int spNumOfSimilarImages; // default value = 1
	enum  {RANDOM, MAX_SPREAD, INCREMENTAL } spKDTreeSplitMethod; // default value = MAX_SPREAD
	int spKNN; // default value = 1
	bool spMinimalGUI; // default value = false
	int spLoggerLevel; // default value = 3
	char* spLoggerFilename; // default value = stdout
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
	int cmpRes;
	bool flag = false;
	FILE * configFilePtr;
	char * bufferVar = NULL;
	char * bufferParam = NULL;
	char bufferLine[LINE_LENGTH];
	const char * const varArray[] = {"spImagesDirectory", "spImagesPrefix",
			"spImagesSuffix", "spNumOfImages", "spPCADimension", "spPCAFilename",
			"spNumOfFeatures", "spExtractionMode", "spNumOfSimilarImages",
			"spKDTreeSplitMethod", "spKNN", "spMinimalGUI", "spLoggerLevel",
			"spLoggerFilename"
	};
	const int varArraySize = 14;
	assert(msg != NULL); // assertion

	if (filename == NULL) {
		(*msg) = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}

	configFilePtr = fopen(filename, "r");
	if (configFilePtr == NULL) { // edge case
		(*msg) = SP_CONFIG_CANNOT_OPEN_FILE;
		return NULL;
	}

	SPConfig config = (SPConfig) malloc(sizeof(*config));
	if (config == NULL) { // Allocation failure
		(*msg) = SP_CONFIG_ALLOC_FAIL;
		return NULL;
	}

	// Iterate the file's lines
	while (fgets(bufferLine, LINE_LENGTH, configFilePtr) != NULL) {
		res = checkValid(bufferLine, bufferVar, bufferParam);
		if (res == 2) {
			(*msg) = SP_CONFIG_INVALID_STRING;
			return NULL;
		}
		// if (res == 1) , ignore comment and move on
		if (res == 0) {

			// compare with all variables
			for(i = 0; i < varArraySize; i++) {
				cmpRes = strcmp(bufferVar, varArray[i]);
				// find match
				if (cmpRes == 0) {
					flag = true;
					if (i == 0)
						setSpImagesDirectory(config, bufferParam);
					if (i == 1)
						setSpImagesPrefix(config, bufferParam);
					if (i == 0)
						setSpImagesDirectory(config, bufferParam); // TODO change all from here
					if (i == 0)
						setSpImagesDirectory(config, bufferParam);
					if (i == 0)
						setSpImagesDirectory(config, bufferParam);
					if (i == 0)
						setSpImagesDirectory(config, bufferParam);
					if (i == 0)
						setSpImagesDirectory(config, bufferParam);
					if (i == 0)
						setSpImagesDirectory(config, bufferParam);
					if (i == 0)
						setSpImagesDirectory(config, bufferParam);
					if (i == 0)
						setSpImagesDirectory(config, bufferParam);
					if (i == 0)
						setSpImagesDirectory(config, bufferParam);

				}
			}
		}
	}

	fclose(configFilePtr);
	return NULL;
}

/*
 * checks bufferLine is in correct format.
 * If comment or empty- returns 1, leaves var and param untouched.
 * If invalid- returns 2, leaves var and param untouched.
 * If bufferLine is NULL, also returns 2.
 * If valid- returns 0, puts in var the variable and puts the parameter in param.
 */
int checkValid(char * bufferLine, char * var, char * param) {
	char * varIn;
	char * paramIn;
	char * endCheck;
	const char split[3] = " =\t";

	if (bufferLine == NULL)
		return 2;
	// Get strings
	varIn = strtok(bufferLine, split);
	paramIn = (char*) strtok(NULL, split);
	endCheck = (char*) strtok(NULL, split);

	// Check if empty
	if (varIn == NULL)
		return 1;

	// Check if comment
	if (varIn[0] == '#')
		return 1;

	// If there are more strings, should error
	if (endCheck != NULL)
		return 2;

	// If not enough strings, should error
	if (paramIn == NULL)
		return 2;

	// Else, set pointers var and param and then return success
	*var = *varIn;
	*param = *paramIn;
	return 0;
}

int setSpImagesDirectory(SPConfig config, char * bufferParam) {
	   strcpy(config->spImagesDirectory, bufferParam);
	   return 0;
}

int setSpImagesPrefix(SPConfig config, char * bufferParam) {
	   strcpy(config->spImagesPrefix, bufferParam);
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
 */
bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg != NULL);
	if (config == NULL)
	{
		*msg =  SP_CONFIG_INVALID_ARGUMENT;
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
 */
bool spConfigMinialGui(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg != NULL);
	if (config == NULL)
	{
		*msg =  SP_CONFIG_INVALID_ARGUMENT;
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
int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg != NULL);
	if (config == NULL)
	{
		*msg =  SP_CONFIG_INVALID_ARGUMENT;
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
int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg != NULL);
	if (config == NULL)
	{
		*msg =  SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfFeatures;
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
int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg != NULL);
	if (config == NULL)
	{
		*msg =  SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spPCADimension;
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
		int index)
{

	if (config == NULL || imagePath == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	if (index >= config->spNumOfImages)
		return SP_CONFIG_INDEX_OUT_OF_RANGE;

	char iStr[LINE_LENGTH];
	memset(iStr, 0, sizeof iStr);

	sprintf(iStr, "%d", index);
	// Create directory string as: dir+prefix+i+suffix
	strcpy(imagePath,config->spImagesDirectory);
	strcat(imagePath, config->spImagesPrefix);
	strcat(imagePath, iStr);
	strcat(imagePath, config->spImagesSuffix);

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
 * @param index - the index of the image.
 * @return
 *  - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 *  - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config)
{
	if (config == NULL || pcaPath == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;

	// Create directory string as: spImagesDirectory+spPcaFilename
	strcpy(pcaPath,config->spImagesDirectory);

	strcat(pcaPath, config->spPCAFilename);

	return SP_CONFIG_SUCCESS;
}

/**
 * Frees all memory resources associate with config.
 * If config == NULL nothig is done.
 */
void spConfigDestroy(SPConfig config)
{
	if (config == NULL)
		return;
	//TODO need to free anything else?
	free(config);
}


