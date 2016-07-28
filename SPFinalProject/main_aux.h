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


/**
 * the function return the numOfBestImages closet images to the query image
 *
 * @param numOfBestImages - the number of images to return
 * @param spKNN - the number of features to consider close to each of the query features
 * @param root - the root of the kd-Tree with contains all the features of all the images in the database
 * @param features - features of the query image
 * @param numOfFeatures - the size of features
 * @param numOfImages - the number of image in the database
 *
 * @return NULL if memory allocation failed
 * an array of the closet images otherwise
 */
int* bestImages(int numOfBestImages, int spKNN, KDTreeNode root, SPPoint* features, int numOfFeatures, int numOfImages);

/**
 * A helper function, used in the debugging phase.
 *
 * Runs some basic functions using the KDArray module, in order to confirm that the module works properly.
 */
void arrayTest();

/**
 * A helper function, used in the debugging phase.
 *
 * Runs some basic functions using the KDTreeNode module, in order to confirm that the module works properly.
 */
void treeTest ();

/**
 * The function receives a two-dimensional array of points, the size of the array,
 * and an array containing the specific size of each sub-array.
 *
 * It destroys all the points contained in the arrays. It also frees all the inner arrays and the outer array.
 */
void free2dPoints (SPPoint ** points, int size, int * rowsSizes);

/**
 * The function receives a one-dimensional array of points, and the size of the array.
 *
 * It destroys all the points contained in the arrays, and frees the array.
 */
void free1dPoints (SPPoint * points, int size);

#endif /* MAIN_AUX_H_ */
