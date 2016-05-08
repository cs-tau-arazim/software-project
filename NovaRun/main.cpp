#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstdio>
#include "sp_image_proc_util.h"
#include "main_aux.h"

#define LINE 1024
#define fgets_fixed(str) { \
	fgets (str, sizeof(str), stdin);\
	if (str == NULL) {\
		exit(10);}\
	strtok(str, "\n");}

int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);
	char strToInt[LINE];
	printf("Enter images directory path:\n");
	char dir[LINE];
	fgets_fixed(dir);

	printf("Enter images prefix:\n");
	char prefix[LINE];
	fgets_fixed(prefix);

	printf("Enter number of images:\n");
	int nImages;
	fgets_fixed (strToInt);
	nImages = atoi(strToInt);
	if (nImages < 1)
	{
		printf("An error occurred - invalid number of images\n");
	}

	printf("Enter images suffix:\n");
	char suffix[LINE];
	fgets_fixed(suffix);


	printf("Enter number of bins:\n");
	int nBins;
	fgets_fixed(strToInt);
	nBins = atoi(strToInt);
	if (nImages < 1)
	{
		printf("An error occurred - invalid number of bins\n");
	}


	printf("Enter number of features:\n");

	int maxNFeatures;
	fgets_fixed(strToInt);
	maxNFeatures = atoi(strToInt);
	if (nImages < 1)
	{
		printf("An error occurred - invalid number of features\n");
		return 0;
	}

	int *nFeaturesPerImage;
	nFeaturesPerImage = (int*)malloc(nImages*sizeof(int));

	int*** rgb;
	rgb = (int***)malloc(nImages*sizeof(int**));


	double*** sift;
	sift = (double***)malloc(nImages*sizeof(double**));


	char currentDir[LINE];

	for (int i = 0 ; i < nImages ; i++)
	{
		char iStr[LINE*4];
		sprintf(iStr, "%d", i);
		// Create directory string as: dir+prefix+i+suffix
		strcpy(currentDir,dir);
		strcat(currentDir, prefix);
		strcat(currentDir, iStr);
		strcat(currentDir, suffix);

		rgb[i] = spGetRGBHist(currentDir, nBins);

		sift[i] = spGetSiftDescriptors(currentDir,  maxNFeatures, &(nFeaturesPerImage[i]));
	}

	while(true)
	{
		printf("Enter a query image or # to terminate:\n");
		char query[LINE];
		fgets_fixed(query);

		// Check if user requested to terminate the program
		if(strcmp("#",query) == 0)
		{
			printf("Exiting...\n");
			free_3d_int(rgb,nImages,3);
			free_3d_double(sift,nImages, maxNFeatures);
			return 0;
		}


		int** queryRGB = spGetRGBHist(query, nBins);

		// Search using Global Features:

		TupleDI* RGBDistList = (TupleDI*)malloc(nImages * sizeof(TupleDI));
		for (int i = 0; i < nImages; i++) {
			RGBDistList[i].a = spRGBHistL2Distance(queryRGB, rgb[i], nBins);
			RGBDistList[i].b = i;
		}

		// Sort using TupleDI
		qsort(RGBDistList, nImages, sizeof(TupleDI), cmpTupleDI);

		printf("Nearest images using global descriptors:\n");

		for (int i=0; i < 4; i++)
		{
			printf("%d, " , (int)RGBDistList[i].b);
		}
		printf("%d\n", (int)RGBDistList[4].b);

		// free memory
		free_2d_int(queryRGB, 3);



		// Search using Local Features:

		int nFeaturesQuery;
		double** querySift = spGetSiftDescriptors(query,  maxNFeatures, &nFeaturesQuery);


		int** featuresCompare;
		featuresCompare = (int**)malloc(nFeaturesQuery* sizeof(int*));

		for (int i = 0 ; i < nFeaturesQuery ; i++)
		{
			featuresCompare[i] = spBestSIFTL2SquaredDistance(5, querySift[i], sift, nImages, nFeaturesPerImage);
		}
		free_2d_double(querySift, nImages);



		TupleDI* SIFTDistList = (TupleDI*)malloc(nImages * sizeof(TupleDI));
		for (int i=0; i<nImages; i++)
		{
			SIFTDistList[i].a = 0;
			SIFTDistList[i].b = i;
		}
		for (int i = 0; i < nFeaturesQuery; i++) {
			for (int j = 0 ; j < 5 ; j++)
			{
				int imageIndex = featuresCompare[i][j];
				SIFTDistList[imageIndex].a += 1;
			}
		}

		free_2d_int(featuresCompare, nFeaturesQuery);

		qsort(SIFTDistList, nImages, sizeof(TupleDI), inverseCmpTupleDI);

		printf("Nearest images using local descriptors:\n");

		for (int i=0; i<4; i++)
		{
			printf("%d, " , SIFTDistList[i].b);
		}
		printf("%d\n", SIFTDistList[4].b);


	}

}






