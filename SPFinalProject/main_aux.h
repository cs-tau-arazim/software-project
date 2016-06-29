/*
 * main_aux.h
 *
 *  Created on: Jun 29, 2016
 *      Author: galwiernik
 */


#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

#define LINE_LENGTH 1024
#include "SPConfig.h"
#include "KDTreeNode.h"
/**
 * Helper function to print the appropriate message
 * for each error received by spConfigCreate.
 */
void printErrorType(SP_CONFIG_MSG * configMsg);

//TODO doc
int* bestImages(int numOfBestImages, int spKNN, KDTreeNode root, SPPoint* features, int numOfFeatures, int numOfImages);

#endif /* MAIN_AUX_H_ */
