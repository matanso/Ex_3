#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>//Mat
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>
#include <vector>

extern "C" {
#include "SPPoint.h"
#include "SPBPriorityQueue.h"
}

#include "sp_image_proc_util.h"



using namespace std;


#define MAX_BUFFER_SIZE 1024

#define DECISION_MSG "Enter a query image or # to terminate:\n"
#define EXIT_MSG "Exiting...\n"


int main() {
	char dirPath[MAX_BUFFER_SIZE], imgPrefix[MAX_BUFFER_SIZE], imgSuffix[MAX_BUFFER_SIZE];
	int numOfImages, nBins, nFeaturesToExtract;
	char* stringParametars = (char*) malloc(3 * sizeof(char));
	int* numericParametars = (int*) malloc(3 * sizeof(int));

	// Input
	if(!mainAuxGetParameters(stringParametars, numericParametars)){
		return 0;
	}
	dirPath = stringParametars[0];
	imgPrefix = stringParametars[1];
	imgSuffix = stringParametars[2];
	numOfImages = numericParametars[0];
	nBins = numericParametars[1];
	nFeaturesToExtract = numericParametars[2];

	SPPoint*** imagesHist = (SPPoint***) malloc(numOfImages * sizeof(SPPoint**))
	SPPoint*** imagesSift = (SPPoint***) malloc(numOfImages * sizeof(SPPoint**))
	int *nFeatures = (int*) malloc(numOfImages * sizeof(int));
	// For each image in {dir_path} calculate histogram and SIFT
	for (int i = 0; i < numOfImages; i++){
		const char* path = mainAuxBuildPath(dir_path, prefix, i, suffix);
		imagesHist[i] = spGetRGBHist(path, i, nBins);
		imagesSift[i] = spGetSiftDescriptors(path, i, nFeaturesToExtract, nFeatures + i);
	}


	const char decisonStr[MAX_BUFFER_SIZE];
	printf(DECISION_MSG);
	scanf("%s",decisonStr)

	if(strcmp(decisonStr, '#')){
		printf(EXIT_MSG);
		return 0;
	}

	int *queryNumberOfFeature;
	SPPoint *queryHist = spGetRGBHist(decisonStr, -1, nBins);
	SPPoint *queryFeature = spGetSiftDescriptors(decisonStr, -1, nFeaturesToExtract, numOfQueryFeatures);

	// mainAuxGetGlobalDescriptor();

    return 0;
}