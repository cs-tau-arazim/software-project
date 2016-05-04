/*
int main()
{
	printf("Enter images directory path:\n");
	char* dir = 0;
	scanf("%s", &dir);

	printf("Enter images prefix:\n");
	char* prefix = 0;
	scanf("%s", &prefix);

	printf("Enter number of images:\n");
	int n = 0;
	scanf("%d", &n);
	if (n < 1)
	{
		printf("An error occurred - invalid number of images\n");
	}

	printf("Enter images suffix:\n");
	char* suffix = 0;
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

	for (int i = 0 ; i < n ; i++)
	{
		//char* currentDir = dir+prefix+i+suffix; // TODO
		// try to use atoi(number) to convert string to int -gal
		// TODO save rgb hist and features for each image
	}
}

*/

#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>//Mat
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>
#include <vector>

int main() {
	//Loading img - NOTE: Gray scale mode!
	cv::Mat src;
	src = cv::imread("baboon.png", CV_LOAD_IMAGE_GRAYSCALE);
	if (src.empty()) {
		return -1;
	}

	//Total number of features we will try to extract
	int maxNumberOfFeatures = 512;

	//Key points will be stored in kp1;
	std::vector<cv::KeyPoint> kp1;
	//Feature values will be stored in ds1;
	cv::Mat ds1;
	//Creating  a Sift Descriptor extractor
	cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor> detect =
			cv::xfeatures2d::SIFT::create(maxNumberOfFeatures);
	//Extracting features
	//The features will be stored in ds1
	//The output type of ds1 is CV_32F (float)
	detect->detect(src, kp1, cv::Mat());
	detect->compute(src, kp1, ds1);

	//This is not relevant for the purpose of assignment 2
	//The following lines draw the extracted key points
	//to the image and show the result in a window
	cv::drawKeypoints(src,kp1,src,0);
	cv::imshow("Baboon - SIFT",src);
	cv::waitKey(0);
	return 0;
}

