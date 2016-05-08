#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstdio>
#include "sp_image_proc_util.h"
#include "main_aux.h"

#define LINE 1024
#define PRINT_LENGTH 5

// Useful macros
#define fgets_fixed(str) { \
	fgets (str, sizeof(str), stdin);\
	if (str == NULL) {\
		printf("String Input error");\
		exit(10);}\
	strtok(str, "\n");}

#define check_malloc(pointer) { \
	if (pointer == NULL) { \
		printf("An error occurred - allocation failure\n");\
		exit(1);}}

#define check_if_null(pointer) { \
	if (pointer == NULL) { \
		printf("An error occurred - now exiting");\
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
	printf("Enter images directory path:\n");
	fgets_fixed(dir);

	printf("Enter images prefix:\n");
	fgets_fixed(prefix);

	printf("Enter number of images:\n");
	fgets_fixed (strToInt);
	nImages = atoi(strToInt);
	if (nImages < 1)
	{
		printf("An error occurred - invalid number of images\n");
		exit(1);
	}

	printf("Enter images suffix:\n");
	fgets_fixed(suffix);

	printf("Enter number of bins:\n");
	fgets_fixed(strToInt);
	nBins = atoi(strToInt);
	if (nImages < 1)
	{
		printf("An error occurred - invalid number of bins\n");
		exit(1);
	}

	printf("Enter number of features:\n");
	fgets_fixed(strToInt);
	maxNFeatures = atoi(strToInt);

	if (nImages < 1)
	{
		printf("An error occurred - invalid number of features\n");
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
		printf("Enter a query image or # to terminate:\n");
		fgets_fixed(query);

		// Check if user requested to terminate the program
		if(strcmp("#",query) == 0)
		{
			printf("Exiting...\n");
			free_3d_int(RGBDatabase,nImages,3);
			free_3d_double(SIFTDatabase,nImages, maxNFeatures);
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
				printf("An error occured. Program will now exit");
				exit(1);
			}
		}

		// Sort using TupleDI
		qsort(RGBDistList, nImages, sizeof(TupleDI), cmpTupleDI);

		// Print results
		printf("Nearest images using global descriptors:\n");
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

		free_2d_double(querySift, nImages); // free array

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
		printf("Nearest images using local descriptors:\n");
		for (int i=0; i<4; i++)
		{
			printf("%d, " , SIFTDistList[i].b);
		}
		printf("%d\n", SIFTDistList[4].b);

		free(SIFTDistList); // free array
	}
}






