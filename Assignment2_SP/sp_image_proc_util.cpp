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


using namespace cv;

/*
 * Calculates the RGB channels histogram. The histogram will be stored in a
 * two dimensional array of dimensions 3XnBins . The first row is the
 * red channel histogram, the second row is the green channel histogram
 * and the third row is the blue channel histogram.
 *
 * @param str - The path of the image for which the histogram will be calculated
 * @param nBins - The number of subdivision for the intensity histogram
 * @return NULL if str is NULL or nBins <= 0 or allocation error occurred,
 *  otherwise a two dimensional array representing the histogram.
 */

int** spGetRGBHist(char* str, int nBins)
{
	/// Load image
	Mat src = imread("baboon.png", CV_LOAD_IMAGE_COLOR);

	/// Separate the image in 3 places ( B, G and R )
	std::vector<Mat> bgr_planes;
	split(src, bgr_planes);

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	/// Set the other parameters:
	int nImages = 1;

	//Output
	Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	/// The results will be store in b_hist,g_hist,r_hist.
	/// The output type of the matrices is CV_32F (float)
	calcHist(&bgr_planes[0], nImages, 0, Mat(), b_hist, 1, &nBins, &histRange);
	calcHist(&bgr_planes[1], nImages, 0, Mat(), g_hist, 1, &nBins, &histRange);
	calcHist(&bgr_planes[2], nImages, 0, Mat(), r_hist, 1, &nBins, &histRange);

	int **histInt;
	histInt=(int **) malloc(b_hist.rows * sizeof(*histInt));
	for (int i = 0; i < 3; i++)
	    histInt[i] = (int *) malloc(b_hist.cols * sizeof(int));


	for (int i = 0; i <  b_hist.rows; i++) {
		histInt[i][0] = b_hist.at<int>(i,0);
	}
	for (int i = 0; i <  g_hist.rows; i++) {
		histInt[i][1] = g_hist.at<int>(i,0);
	}
	for (int i = 0; i <  r_hist.rows; i++) {
		histInt[i][2] = r_hist.at<int>(i,0);
	}
	return histInt;
}


double spRGBHistL2Distance(int** histA, int** histB, int nBins)
{
	double dis;
	for (int i = 0; i < 3 ; i++)
	{
		for (int j = 0 ; j < sizeof(histA[0]) ; j++)
		{
			dis += (histA[i][j] - histB[i][j])*(histA[i][j] - histB[i][j]);
		}
	}
	dis *= 0.33;
	return dis;

}


double** spGetSiftDescriptors(char* str, int maxNFeatures, int *nFeatures)
{

	//Loading img - NOTE: Gray scale mode!
	Mat src;
	src = imread("baboon.png", CV_LOAD_IMAGE_GRAYSCALE);

	//Key points will be stored in kp1;
	std::vector<KeyPoint> kp1;
	//Feature values will be stored in ds1;
	Mat ds1;
	//Creating  a Sift Descriptor extractor
	Ptr<xfeatures2d::SiftDescriptorExtractor> detect =
			xfeatures2d::SIFT::create(maxNFeatures);
	//Extracting features
	//The features will be stored in ds1
	//The output type of ds1 is CV_32F (float)
	detect->detect(src, kp1, cv::Mat());
	detect->compute(src, kp1, ds1);


	double ** descriptors;
	descriptors = (double **)malloc(128* sizeof(*descriptors));
	for (int i = 0; i < 128; i++) {
		descriptors[i]  = (double*)malloc(ds1.rows * sizeof(*(descriptors[i])));
	}

	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < ds1.rows; j++) {
			descriptors[i][j] = ds1.at<double>(i,0);
		}
	}

	// TODO confirm nFeatures pointer value
	*nFeatures = ds1.rows;

<<<<<<< HEAD
	//	descriptors[i]  = (double*)malloc(ds1.rows * sizeof(*(descriptors[i])));

=======
>>>>>>> 0ab83a1e2e42b78a669bf78e7dc0f55c90f20455
	return descriptors;
}


double spL2SquaredDistance(double* featureA, double* featureB)
{
	double dis;
	for (int j = 0 ; j < sizeof(featureA) ; j++)
		{
			dis += (featureA[j] - featureB[j])*(featureA[j] - featureB[j]);
		}
	return dis;
}


int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}


/**
 * Given sift descriptors of the images in the database (databaseFeatures), finds the
 * closest bestNFeatures to a given SIFT feature (featureA). The function returns the
 * INDEXES of the images to which the closest features belong, stored in ascending order
 * (Closest feature image index is first, second closest feature image index is second, etc...).
 * Assumptions:
 *   - Tie break - In case featureA has the same distance (L2Squared distance) from two features,
 *     then the feature that corresponds to the smallest image
 *     index in the database is closer.
 *
 *   - The returned result may contain duplicates in case two features belongs to the same image.
 *
 *   - databaseFeatures is an array of two dimensional arrays, the number of elements
 *     in databaseFeatures is numberOfImages.
 *
 *   - Each entry in databaseFeatures corresponds to the features of some image in the database.
 *     The ith entry corresponds to the features of image_i in the database, and it is a two dimensional
 *     array of dimension nFeaturesPerImage[i]X128.
 *
 *   - The number of descriptors for the ith image is nFeaturesPerImage[i]
 *
 * @param bestNFeatures     - The number of indexes to return.
 * @param featureA          - A sift descriptor which will be compared with the other descriptor
 * 							  (Assumption dimension(bestNFeatures) = 128)
 * @param databaseFeatures  - An array of two dimensional array, in which the descriptors of the images are stored.
 * 							  The ith entry of the array corresponds to the features of the ith image in the database
 * @param numberOfImages    - The number of images in the database. (Number of entries in databaseFeatures)
 * @param nFeaturesPerImage - The number of features for each image. (i.e databaseFeatures[i] is two dimensional
 * 							  array with the dimension nFeaturesPerImage[i]X128
 * @return - NULL if either the following:
 * 			 * featureA is NULL
 * 			 * databaseFeatures is NULL
 * 			 * numberOfImages <= 1
 * 			 * nFeaturesPerImage is NULL
 * 			 * allocation error occurred
 * 			 otherwise, an array of size bestNFeatures is returned such that:
 * 			 * Given that f1, f2, ... the closest features to featureA (i.e d(featureA,f1) <= d(featureA,f2) <= ...)
 * 			 * i1, i2, .... are the indexes of the images to which fi belongs (i.e f1 is a SIFT descriptor of image i1,
 * 			   f2 is a SIFT descriptor of image i2 etc..)
 * 			 Then the array returned is {i1,i2,...,i_bestNFeatures}
 */
int* spBestSIFTL2SquaredDistance(int bestNFeatures, double* featureA,
		double*** databaseFeatures, int numberOfImages,
		int* nFeaturesPerImage)
{

	// for each image
	for(int i = 0; i < numberOfImages; i++) {

		int nFeaturesImageI = nFeaturesPerImage[i];
		// array of distances for each feature
		double distList[nFeaturesPerImage[i]];

		// calculate all the distances of specific photo from featureA
		for (int j = 0; j < nFeaturesImageI; j++) {
			distList[j] = spL2SquaredDistance(featureA, databaseFeatures[i][j]);
		}

		qsort(distList, nFeaturesImageI, sizeof(double), cmpfunc);

		// calculate sum of shortest distances
		double distance = 0;

		// safety calculation of minimum
		int min = bestNFeatures ^ ((nFeaturesImageI ^ bestNFeatures) & -(nFeaturesImageI < bestNFeatures));

		// TODO
	}
}


