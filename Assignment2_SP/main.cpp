#include <opencv2/core.hpp>// Mat
#include <opencv2/highgui.hpp>  //imshow
#include <opencv2/imgcodecs.hpp>//imread
#include <cstdio>
#include <opencv2/imgproc.hpp>//calcHist
#include <opencv2/core.hpp>//Mat
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>//Mat
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>
#include <vector>
#include "sp_image_proc_util.h"

int main()
{
	printf("Enter images directory path:\n");
	char* dir;
	scanf("%s", &dir);

	printf("Enter images prefix:\n");
	char* prefix;
	scanf("%s", &prefix);

	printf("Enter number of images:\n");
	int n = 0;
	scanf("%d", &n);
	if (n < 1)
	{
		printf("An error occurred - invalid number of images\n");
	}

	printf("Enter images suffix:\n");
	char* suffix;
	scanf("%s", &suffix);

	printf("Enter number of bins:\n");
	int nBins = 0;
	scanf("%d", &nBins);
	if (n < 1)
	{
		printf("An error occurred - invalid number of bins\n");
	}

	printf("Enter number of features:\n");
	int maxNFeatures = 0;
	scanf("%d", &maxNFeatures);
	if (n < 1)
	{
		printf("An error occurred - invalid number of features\n");
	}



	//7
	int *nFeaturesPerImage;
	nFeaturesPerImage = (int*)malloc(n*sizeof(int));
	int*** rgb;
	rgb = (int***)malloc(n*sizeof(int));
	double*** sift;
	sift = (double***)malloc(n*sizeof(int));


	for (int i = 0 ; i < n ; i++)
	{
		char* iStr;
		sprintf(iStr, "%d", i);
		char* currentDir = dir; //+prefix+i+suffix
		strcat(currentDir, prefix);
		strcat(currentDir, iStr);
		strcat(currentDir, suffix);
		rgb[i] = spGetRGBHist(currentDir, nBins);
		sift[i] = spGetSiftDescriptors(currentDir,  maxNFeatures, &(nFeaturesPerImage[i]));
	}

	//8
	printf("Enter a query image or # to terminate:\n");
	char* query;
	scanf("%s", &query);

	//9
	if(strcmp("#",query) == 0)
	{
		printf("Exiting...\n");
		free_data(rgb,n,3);
		free_data(sift,n, maxNFeatures);
	}

	//10
	int *nFeaturesQuery;
	int** queryRGB = spGetRGBHist(query, nBins);
	double** querySift = spGetSiftDescriptors(query,  maxNFeatures, nFeaturesQuery);

	// TODO global search

	//Search using Local Features:
	//int**
	for (int i = 0 ; i < *nFeaturesQuery ; i++)
	{
		//spBestSIFTL2SquaredDistance(int bestNFeatures, double* featureA,
			//	double*** databaseFeatures, int numberOfImages,
				//int* nFeaturesPerImage);
	}

	int ** featuresCompare;
	featuresCompare = (int **)malloc(n* sizeof(*featuresCompare));
	for (int i = 0; i < n; i++) {
		featuresCompare[i]  = (int*)malloc(5 * sizeof(*(featuresCompare[i])));
	}

	for (int i = 0 ; i < *nFeaturesQuery ; i++)
	{
		featuresCompare[i] = spBestSIFTL2SquaredDistance(5, querySift[i], sift, n, nFeaturesPerImage);
	}

	int** hitsPerImage;
	hitsPerImage = (int **)malloc(n* sizeof(*hitsPerImage));
	for (int i = 0; i < n; i++) {
		hitsPerImage[i]  = (int*)malloc(2 * sizeof(*(hitsPerImage[i])));
	}

	for (int i=0; i<n; i++)
	{
		hitsPerImage[i][0] = i;
		hitsPerImage[i][1] = 0;
	}

	for (int i = 0 ; i < *nFeaturesQuery ; i++)
	{
		for (int j = 0 ; j < 5 ; j++)
		{
			int imageIndex = featuresCompare[i][j];
			hitsPerImage[imageIndex][1] += 1;
		}
	}

	qsort(hitsPerImage, n, 2, compareHits); // TODO check order

	printf("Nearest images using local descriptors:\n");

	for (int i=0; i<5; i++)
	{
		printf("j%d," , hitsPerImage[i][0]);
		//printf("res = %d\n" , funPow(x,n,d));
	}
	printf("/n");


	// free mem
	for (int i = 0; i < n; i++)
	{
	    int* currentIntPtr = hitsPerImage[i];
	    free(currentIntPtr);
	}
	for (int i = 0; i < n; i++)
	{
		int* currentIntPtr = featuresCompare[i];
		free(currentIntPtr);
	}
	for (int i = 0; i < n; i++)
	{
		int* currentIntPtr = queryRGB[i];
		free(currentIntPtr);
	}
	for (int i = 0; i < n; i++)
	{
		double* currentIntPtr = querySift[i];
		free(currentIntPtr);
	}
	free(nFeaturesQuery);
	// end of free mem

}






