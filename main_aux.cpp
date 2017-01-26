#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>//Mat
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>
#include <vector>
#include "sp_image_proc_util.h"
#include "main_aux.h"

#define MAX_BUFFER_SIZE 1024

#define ERR_MSG "An error occurred - "
#define IMG_NUM_ERR "invalid number of images\n"
#define BIN_NUM_ERR "invalid number of bins\n"
#define FEAT_NUM_ERR "invalid number of features\n"

int readNumberForMessage(char * msg){
	int number;
	printf(msg);
	scanf("%d",number);
	return number;

}
char* readStringForMessage(char* msg){
	char dir_path[MAX_BUFFER_SIZE];
	printf(msg);
	scanf("%s",dir_path);
	return dir_path;
}


const char* mainAuxBuildPath(char* dir, char* prefix, int i, char* suffix){
	char str[MAX_BUFFER_SIZE];
	sprintf(str, "%s%s%d%s", dir, prefix, i, suffix);
	return str;
}

bool mainAuxGetParameters(char** stringParametars,int* numericParametars){
	char dirPath[MAX_BUFFER_SIZE], imgPrefix[MAX_BUFFER_SIZE], imgSuffix[MAX_BUFFER_SIZE];
	int numOfImages, nBins, nFeaturesToExtract;


	// Input
	// Images path
	stringParametars[0] = readStringForMessage("Enter images directory path:\n");

	// Images prefix
	stringParametars[1] = readStringForMessage("Enter images prefix:\n");

	// Number of images
	numericParametars[0] = readNumberForMessage("Enter number of images:\n");
	if(numericParametars[0] < 1){ 
		printf("%s%s",ERR_MSG,IMG_NUM_ERR);
		return false;
	}

	// Images suffix
	stringParametars[2] = readStringForMessage("Enter images suffix:\n");

	// Number of Bins
	numericParametars[1] = readNumberForMessage("Enter number of bins:\n");
	if(numericParametars[1] < 1 || numericParametars[1] > 255){ 
		printf("%s%s",ERR_MSG,BIN_NUM_ERR);
		return false;
	}

	// Number of features to extract
	numericParametars[2] = readNumberForMessage("Enter number of features:\n");
	if(numericParametars[2] < 1){
		printf("%s%s",ERR_MSG,FEAT_NUM_ERR);
		return false;
	}
	return true;
}

void mainAuxGetGlobalDescriptor(SPPoint*** imagesHist, SPPoint* queryHist, int k){

}