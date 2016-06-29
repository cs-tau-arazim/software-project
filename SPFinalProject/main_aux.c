/*
 * main_aux.c
 *
 *  Created on: Jun 29, 2016
 *      Author: galwiernik
 */

#include "main_aux.h"

/**
 * Helper function to print the appropriate message
 * for each error received by spConfigCreate.
 */
void printErrorType(SP_CONFIG_MSG * configMsg) {
	if ((*configMsg) == SP_CONFIG_INVALID_ARGUMENT)
		printf("Message: No configuration file");
	if ((*configMsg) == SP_CONFIG_CANNOT_OPEN_FILE)
		printf("Message: Invalid configuration file");
	if ((*configMsg) == SP_CONFIG_ALLOC_FAIL)
		printf("Message: Memory allocation failure occurred");

	if ((*configMsg) == SP_CONFIG_INVALID_INTEGER) {
		printf("Line: ??????"); // LINE CHECK MEANING
		printf("Message: Invalid value - constraint not met");
	}

	if ((*configMsg) == SP_CONFIG_INVALID_INTEGER
			|| (*configMsg) == SP_CONFIG_INVALID_STRING) {
		printf("Line: ??????"); // LINE CHECK MEANING
		printf("Message: Invalid value - constraint not met");
	}
	if ((*configMsg) == SP_CONFIG_MISSING_DIR) {
		printf("Line: ??????"); // LINE CHECK MEANING
		printf("Message: Parameter spImagesDirectory is not set");
	}
	if ((*configMsg) == SP_CONFIG_MISSING_PREFIX) {
		printf("Line: ??????"); // LINE CHECK MEANING
		printf("Message: Parameter spImagesPrefix is not set");
	}
	if ((*configMsg) == SP_CONFIG_MISSING_SUFFIX) {
		printf("Line: ??????"); // LINE CHECK MEANING
		printf("Message: Parameter spImagesSuffix is not set");
	}
	if ((*configMsg) == SP_CONFIG_MISSING_NUM_IMAGES) {
		printf("Line: ??????"); // LINE CHECK MEANING
		printf("Message: Parameter spNumOfImages is not set");
	}

}
