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
	//fgets (dir, sizeof(dir), stdin);
	fgets_fixed(dir);

	printf("Enter images prefix:\n");
	char prefix[LINE];
	//fgets (prefix, sizeof(prefix), stdin);
	fgets_fixed(prefix);

	printf("Enter number of images:\n");
	int n; // TODO change!
	fgets_fixed (strToInt);
	n = atoi(strToInt);
	if (n < 1)
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
	if (n < 1)
	{
		printf("An error occurred - invalid number of bins\n");
	}


	printf("Enter number of features:\n");

	int maxNFeatures;
	fgets_fixed(strToInt);
	maxNFeatures = atoi(strToInt);
	if (n < 1)
	{
		printf("An error occurred - invalid number of features\n");
	}

	//TODO remove
	//printf("%s%s%d%s, %d, %d\n",dir,prefix,n,suffix,nBins,maxNFeatures);
	//printf("all's good so far\n");
	//7
	int *nFeaturesPerImage;
	nFeaturesPerImage = (int*)malloc(n*sizeof(int));

	int*** rgb;
	rgb = (int***)malloc(n*sizeof(int**));
			//(int***)alloc_3d_int(n, 3, nBins);


	double*** sift;
	sift = (double***)malloc(n*sizeof(double**));
			//(double***)alloc_3d_double(n, maxNFeatures, 128);

	//printf("malloc success 1\n");



	char currentDir[LINE];

	for (int i = 0 ; i < n ; i++)
	{
		//printf("%d\n", &(nFeaturesPerImage[i]));
	}
	for (int i = 0 ; i < n ; i++)
	{
		//printf("\n");
		char iStr[LINE*4];
		sprintf(iStr, "%d", i);

		strcpy(currentDir,dir);
		 //+prefix+i+suffix
		strcat(currentDir, prefix);
		strcat(currentDir, iStr);
		strcat(currentDir, suffix);
		//printf("%s", currentDir);
		//printf("\n");
		rgb[i] = spGetRGBHist(currentDir, nBins);
		//printf("success with RGBHIst\n");

		sift[i] = spGetSiftDescriptors(currentDir,  maxNFeatures, &(nFeaturesPerImage[i]));
		//printf("nFeatures: %d\n", nFeaturesPerImage[i]);
		//printf("success with SiftDescriptors\n");

	}

	while(true)
	{
		//8
		printf("Enter a query image or # to terminate:\n");
		char query[LINE];
		fgets_fixed(query);

		//9
		if(strcmp("#",query) == 0)
		{
			printf("Exiting...\n");
			free_3d_int(rgb,n,3);
			free_3d_double(sift,n, maxNFeatures);
			return 0;
		}

		//10

		int** queryRGB = spGetRGBHist(query, nBins);

		// Search using Global Features:



		// attempt with tuple
		TupleDI* RGBDistList = (TupleDI*)malloc(n * sizeof(TupleDI*));
		for (int i = 0; i < n; i++) {
			RGBDistList[i].a = spRGBHistL2Distance(queryRGB, rgb[i], nBins);
			RGBDistList[i].b = i;
			//printf("(%f, %d), ", RGBDistList[i].a, RGBDistList[i].b);
		}
		//printf("\n");

		qsort(RGBDistList, n, sizeof(TupleDI), cmpTupleDI);
		for (int i = 0; i < n; i++) {
			//printf("(%f, %d), ", RGBDistList[i].a, RGBDistList[i].b);
		}
		printf("TUPLE Nearest images using global descriptors:\n");

		for (int i=0; i < 5; i++)
		{

			printf("%d, " , (int)RGBDistList[i].b);
		}
		//printf("\n");

		//free mem
		free_2d_int(queryRGB, 3);
		//printf("freed rgb\n");

		int nFeaturesQuery;
		double** querySift = spGetSiftDescriptors(query,  maxNFeatures, &nFeaturesQuery);

		// Search using Local Features:
		int** featuresCompare;
		//printf("featuresCompare malloc begins\n");
		featuresCompare = (int**)malloc(nFeaturesQuery* sizeof(int*));

		printf("%d\n", nFeaturesQuery);

		for (int i = 0 ; i < nFeaturesQuery ; i++)
		{
			featuresCompare[i] = spBestSIFTL2SquaredDistance(5, querySift[i], sift, n, nFeaturesPerImage);
			printf("%d\n", i);
		}
		free_2d_double(querySift, n);
		printf("freed querySift\n");
		//printf("featuresCompare computed\n");

		TupleDI* SIFTDistList = (TupleDI*)malloc(n * sizeof(TupleDI*));
		for (int i=0; i<n; i++)
		{
			SIFTDistList[i].a = 0;
			SIFTDistList[i].b = i;
		}
		for (int i = 0; i < nFeaturesQuery; i++) {
			for (int j = 0 ; j < 5 ; j++)
			{
				int imageIndex = featuresCompare[i][j];
				SIFTDistList[imageIndex].a += 1;
				//printf("(%f, %d), ", RGBDistList[i].a, RGBDistList[i].b);
			}
		}


		free_2d_int(featuresCompare, nFeaturesQuery);
		//free(featuresCompare);
		printf("freed featuresCompare\n");

		//printf("\n");

		for (int i = 0; i < n; i++) {

			printf("%d, %f\n",i, SIFTDistList[i].a);
			//printf("(%f, %d), ", RGBDistList[i].a, RGBDistList[i].b);

		}

		qsort(SIFTDistList, n, sizeof(TupleDI), inverseCmpTupleDI);

		/*
		int** hitsPerImage;
		hitsPerImage = (int **)malloc(n* sizeof(*hitsPerImage));
		for (int i = 0; i < n; i++) {
			hitsPerImage[i]  = (int*)malloc(2 * sizeof(*(hitsPerImage[i])));
		}
		printf("hitsPerImage malloc success\n");

		for (int i=0; i<n; i++)
		{
			hitsPerImage[i][0] = i;
			hitsPerImage[i][1] = 0;
		}
		printf("hitsPerImage init\n");

		for (int i = 0 ; i < *nFeaturesQuery ; i++)
		{
			for (int j = 0 ; j < 5 ; j++)
			{
				//printf("%d\n" , featuresCompare[i][j]);
				int imageIndex = featuresCompare[i][j];
				hitsPerImage[imageIndex][1] += 1;
			}
		}
		printf("hitsPerImage computed\n");

		qsort(hitsPerImage, n, sizeof(int*), compareHits); // TODO check order
		printf("hitsPerImage sorted\n");
		*/
		printf("Nearest images using local descriptors:\n");

		for (int i=0; i<5; i++)
		{
			printf("%d, " , SIFTDistList[i].b);
		}
		printf("/n");
		//printf("%d\n", sizeof(double));

		// free mem







		// end of free mem



	}

}






