#include <opencv2/core.hpp>// Mat
#include <opencv2/highgui.hpp>  //imshow
#include <opencv2/imgcodecs.hpp>//imread
#include <cstdio>
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
	Mat hist;

	/// The output type of the matrices is CV_32F (float)
	calcHist(&src, nImages, 0, Mat(), hist, 3, &nBins, &histRange);

	int **array = malloc(nrows * sizeof *array + (nrows * (ncolumns * sizeof **array));
	for (int i=0; i < )
	return array;
}

int main(){	Mat src;

/// Load image
src = imread("baboon.png", CV_LOAD_IMAGE_COLOR);

if (src.empty()) {
	return -1;
}

/// Separate the image in 3 places ( B, G and R )
std::vector<Mat> bgr_planes;
split(src, bgr_planes);

/// Establish the number of bins
int nBins = 256;

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
	return 0;
}



