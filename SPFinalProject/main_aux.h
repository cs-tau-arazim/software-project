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


//TODO doc
int* bestImages(int numOfBestImages, int spKNN, KDTreeNode root, SPPoint* features, int numOfFeatures, int numOfImages);

void arrayTest();

void treeTest ();

#endif /* MAIN_AUX_H_ */
