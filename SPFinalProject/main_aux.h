/*
 * main_aux.h
 *
 *  Created on: Jun 29, 2016
 *      Author: galwiernik
 */


#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_


#define LINE_LENGTH 1024

#include "KDTreeNode.h"
#include "SPConfig.h"

/*
 * Helper function-
 * receives argc and argv directly from main, and sets configPath as appropriate path,
 * based on command line arguments. returns 0 on success, 1 on fail (invalid command line).
 */
int getConfigPath(int argc, char** argv, char * configPath);


/**
 * The function receives an imageFeaturePath path and relevant parameters,
 * and extracts the info relevant to the specific image into the file.
 * returns 0 on success and 1 on failure.
 */
int writeFeaturesToFile(SPPoint ** featureArr, char * imageFeaturePath, int i, int numOfFeatures, int PCADim);


/**
 * The function receives an imageFeaturePath path and relevant parameters,
 * and extracts the info relevant FROM the specific file INTO the featureArr.
 *  * returns 0 on success and 1 on failure.
 */
int readFeaturesFromFile(SPPoint ** featureArr, char * imageFeaturePath, int i, int * numOfFeatures, int PCADim, int * featureArrSize, double * tempDoubleArr);


//TODO doc
int* bestImages(int numOfBestImages, int spKNN, KDTreeNode root, SPPoint* features, int numOfFeatures, int numOfImages);

void arrayTest();

void treeTest ();

void free2dPoints (SPPoint ** points, int size, int * rowsSizes);

void free1dPoints (SPPoint * points, int size);

#endif /* MAIN_AUX_H_ */
