#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstdio>
#include "sp_image_proc_util.h"
#include "main_aux.h"

#define LINE 1024
#define PRINT_LENGTH 5


#define STRING_INPUT_ERROR "String Input error"
#define ALLOCATION_FAILTURE "An error occurred - allocation failure\n"
#define ERROR_EXITING "An error occurred - now exiting"
#define ENTER_IMAGES_DIRECTORY_PATH "Enter images directory path:\n"
#define ENTER_IMGAES_PREFIX "Enter images prefix:\n"
#define ENTER_NUMBER_OF_IMAGES "Enter number of images:\n"
#define INVALID_NUMBER_OF_IMAGES "An error occurred - invalid number of images\n"
#define ENTER_IMGAES_SUFFIX "Enter images suffix:\n"
#define ENTER_NUMBER_OF_BINS "Enter number of bins:\n"
#define INVALID_NUMBER_OF_BINS "An error occurred - invalid number of bins\n"
#define ENTER_NUMBER_OF_FEATURES "Enter number of features:\n"
#define INVALID_NUMBER_OF_FEATURES "An error occurred - invalid number of features\n"
#define ENTER_QUERY_OR_TERMINATE "Enter a query image or # to terminate:\n"
#define EXITING "Exiting...\n"
#define NEAREST_USING_GLOBAL "Nearest images using global descriptors:\n"
#define NEAREST_USING_LOCAL "Nearest images using local descriptors:\n"


// Useful macros
#define fgets_fixed(str) { \
	fgets (str, sizeof(str), stdin);\
	if (str == NULL) {\
		printf(STRING_INPUT_ERROR);\
		exit(10);}\
	strtok(str, "\n");}

#define check_malloc(pointer) { \
	if (pointer == NULL) { \
		printf(ALLOCATION_FAILTURE);\
		exit(1);}}

#define check_if_null(pointer) { \
	if (pointer == NULL) { \
		printf(ERROR_EXITING);\
		exit(1);}}


int main()
{
	// For Eclipse to behave better with I/O
	setvbuf(stdout, NULL, _IONBF, 0);

	// Initial input variables, all static
	char strToInt[LINE];
	char dir[LINE];
	char prefix[LINE];
	int nImages;
	char suffix[LINE];
	int nBins;
	int maxNFeatures;
	char currentDir[LINE];
	char iStr[LINE*4];

	// Pointers to all database arrays
	int *nFeaturesPerImage;
	int*** RGBDatabase;
	double*** SIFTDatabase;

	// Main loop variables
	char query[LINE];
	int** queryRGB; // Global Features
	TupleDI* RGBDistList;
	int nFeaturesQuery; // Local Features
	double** querySift;
	int** featuresCompare;
	TupleDI* SIFTDistList;


	// Asking the user for input
	printf(ENTER_IMAGES_DIRECTORY_PATH);
	fgets_fixed(dir);

	printf(ENTER_IMGAES_PREFIX);
	fgets_fixed(prefix);

	printf(ENTER_NUMBER_OF_IMAGES);
	fgets_fixed (strToInt);
	nImages = atoi(strToInt);
	if (nImages < 1)
	{
		printf(INVALID_NUMBER_OF_IMAGES);
		exit(1);
	}

	printf(ENTER_IMGAES_SUFFIX);
	fgets_fixed(suffix);

	printf(ENTER_NUMBER_OF_BINS);
	fgets_fixed(strToInt);
	nBins = atoi(strToInt);
	if (nBins < 1)
	{
		printf(INVALID_NUMBER_OF_BINS);
		exit(1);
	}

	printf(ENTER_NUMBER_OF_FEATURES);
	fgets_fixed(strToInt);
	maxNFeatures = atoi(strToInt);

	if (maxNFeatures < 1)
	{
		printf(INVALID_NUMBER_OF_FEATURES);
		exit(1);
	}

	// Allocating the arrays for the database
	nFeaturesPerImage = (int*)malloc(nImages*sizeof(int));
	check_malloc(nFeaturesPerImage);

	RGBDatabase = (int***)malloc(nImages*sizeof(int**));
	check_malloc(RGBDatabase);

	SIFTDatabase = (double***)malloc(nImages*sizeof(double**));
	check_malloc(SIFTDatabase);

	// Building the database
	for (int i = 0 ; i < nImages ; i++)
	{
		// Reset string
		memset(iStr, 0, sizeof iStr);

		sprintf(iStr, "%d", i);
		// Create directory string as: dir+prefix+i+suffix
		strcpy(currentDir,dir);
		strcat(currentDir, prefix);
		strcat(currentDir, iStr);
		strcat(currentDir, suffix);

		RGBDatabase[i] = spGetRGBHist(currentDir, nBins);
		check_if_null(RGBDatabase[i]);

		SIFTDatabase[i] = spGetSiftDescriptors(currentDir,  maxNFeatures, &(nFeaturesPerImage[i]));
		check_if_null(SIFTDatabase[i]);
	}

	// Enter the main loop
	while(true)
	{
		memset(query, 0, sizeof query);
		printf(ENTER_QUERY_OR_TERMINATE);
		fgets_fixed(query);

		// Check if user requested to terminate the program
		if(strcmp("#",query) == 0)
		{
			printf(EXITING);
			free_3d_int(RGBDatabase,nImages,3);
			free_3d_sift(SIFTDatabase,nImages, nFeaturesPerImage);
			free(nFeaturesPerImage);
			return 0;
		}

		//// Search using Global Features:

		// Load RGBHist for query
		queryRGB = spGetRGBHist(query, nBins);
		check_if_null(queryRGB);

		// Calculate distances
		RGBDistList = (TupleDI*)malloc(nImages * sizeof(TupleDI));
		check_malloc(RGBDistList);

		for (int i = 0; i < nImages; i++) {
			RGBDistList[i].a = spRGBHistL2Distance(queryRGB, RGBDatabase[i], nBins);
			RGBDistList[i].b = i;
			if (RGBDistList[i].a == -1) {
				printf(ERROR_EXITING);
				exit(1);
			}
		}

		// Sort using TupleDI
		qsort(RGBDistList, nImages, sizeof(TupleDI), cmpTupleDI);

		// Print results
		printf(NEAREST_USING_GLOBAL);
		for (int i=0; i < 4; i++)
		{
			printf("%d, " , (int)RGBDistList[i].b);
		}
		printf("%d\n", (int)RGBDistList[4].b);

		// Free query memory
		free_2d_int(queryRGB, 3);


		//// Search using Local Features:

		nFeaturesQuery = 0;
		// Loading query local features
		querySift = spGetSiftDescriptors(query,  maxNFeatures, &nFeaturesQuery);
		check_if_null(querySift);
		featuresCompare = (int**)malloc(nFeaturesQuery* sizeof(int*));
		check_malloc(featuresCompare);

		for (int i = 0 ; i < nFeaturesQuery ; i++) {
			featuresCompare[i] = spBestSIFTL2SquaredDistance(PRINT_LENGTH, querySift[i], SIFTDatabase, nImages, nFeaturesPerImage);
			check_if_null(featuresCompare[i]);
		}

		free_2d_double(querySift, nFeaturesQuery); // free array

		// Sorting using TupleDI
		SIFTDistList = (TupleDI*)malloc(nImages * sizeof(TupleDI));
		check_malloc(SIFTDistList);

		for (int i=0; i<nImages; i++)
		{
			SIFTDistList[i].a = 0;
			SIFTDistList[i].b = i;
		}

		// Calculating score for each photo
		for (int i = 0; i < nFeaturesQuery; i++) {
			for (int j = 0 ; j < PRINT_LENGTH ; j++)
			{
				int imageIndex = featuresCompare[i][j];
				SIFTDistList[imageIndex].a += 1;
			}
		}

		free_2d_int(featuresCompare, nFeaturesQuery); // free array

		// Sort through tuples
		qsort(SIFTDistList, nImages, sizeof(TupleDI), inverseCmpTupleDI);

		// Print results
		printf(NEAREST_USING_LOCAL);
		for (int i=0; i<4; i++)
		{
			printf("%d, " , SIFTDistList[i].b);
		}
		printf("%d\n", SIFTDistList[4].b);

		free(SIFTDistList); // free array
	}
}






