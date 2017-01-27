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
#define GLOBAL_DESC "Nearest images using global descriptors:\n"

// TODO check if we can use this to report errors
#define report_error(str)								\
 	do { 												\
 		printf("Error: %s, line %d\n",(str), __LINE__); \
 		return NULL;								 	\
 	} while(0)
#define report_error_exit(str) 							\
 	do { 												\
 		printf("Error: %s, line %d\n",(str), __LINE__); \
 		exit(-1); 										\
 	} while(0)
#define report_error_ret(str,ret)						\
 	do { 												\
 		printf("Error: %s, line %d\n",(str), __LINE__); \
 		return (ret); 									\
 	} while(0)

#define report_error_msg(msg)							\
 	do { 												\
 		printf("Image cannot be loaded - %s:\n",msg);   \
		exit(-1);										\
		}                                               \
 	} while(0)



int readNumberUsingMessage(char * msg){
	int number;
	printf("%s",msg);
	scanf("%d",&number);
	return number;

}
void readStringUsingMessage(char* msg, char *result){
	// char dir_path[MAX_BUFFER_SIZE];
	printf("%s", msg);
	scanf("%s", result);
	// return &dir_path;
}


const char* mainAuxBuildPath(char* dir, char* prefix, int i, char *suffix){
	char str[MAX_BUFFER_SIZE];
	sprintf(str, "%s%s%d%s", dir, prefix, i, suffix);
	return &str;
}

bool mainAuxGetParameters(char *dirPath, char *imgPrefix, char *imgSuffix
  ,int numOfImages,int nBins,int nFeaturesToExtract){
	// Input
	// Images path
	dirPath = readStringUsingMessage("Enter images directory path:\n");

	// Images prefix
	imgPrefix = readStringUsingMessage("Enter images prefix:\n");

	// Number of images
	numOfImages = readNumberUsingMessage("Enter number of images:\n");
	if(numOfImages < 1){
		printf("%s%s",ERR_MSG,IMG_NUM_ERR);
		return false;
	}

	// Images suffix
	imgSuffix = readStringUsingMessage("Enter images suffix:\n");

	// Number of Bins
	nBins = readNumberUsingMessage("Enter number of bins:\n");
	if(nBins < 1 || nBins > 255){
		printf("%s%s",ERR_MSG,BIN_NUM_ERR);
		return false;
	}

	// Number of features to extract
	nFeaturesToExtract = readNumberUsingMessage("Enter number of features:\n");
	if(nFeaturesToExtract < 1){
		printf("%s%s",ERR_MSG,FEAT_NUM_ERR);
		return false;
	}
	return true;
}

void mainAuxPrintGlobalDescriptor(SPPoint*** imagesHist, SPPoint** queryHist,
  int numberOfImages, int k){
	// Create a min-priority queue of size kClosest
	SPBPQueue *KClosestImages;
    if((KClosestImages = spBPQueueCreate(k)) == NULL){ return; }
	for(int i = 0; i < numberOfImages; i++){
		double dist = spRGBHistL2Distance(imagesHist[i], queryHist);
		if(spBPQueueEnqueue(KClosestImages, i, dist) == SP_BPQUEUE_INVALID_ARGUMENT){
            // TODO KClosestImages is NULL argument
		}
	}
	printf(GLOBAL_DESC);
	mainAuxPrintQueueIndex(KClosestImages);
}


void mainAuxPrintLocalDescriptor(SPPoint*** imagesSift, SPPoint** queryFeature,
  int numberOfImages, int *nFeaturesPerImage, int k){
    int* index = malloc(k * sizeof(int));
    index = spBestSIFTL2SquaredDistance(k, queryFeature, imagesSift,
      numberOfImages, nFeaturesPerImage);
    for (int i = 0; i < k; i++) {
      printf("%d",index[i]);
      if(i != k){ printf(", "); } else { break; }
    }
    printf("\n");
  }

void mainAuxPrintQueueIndex(SPBPQueue *queue) {
    if(queue == NULL) return;
    BPQueueElement element;
    bool isEmpty = spBPQueueIsEmpty(queue);
    while (!isEmpty) { // Printing k nearest points
        assert(spBPQueuePeek(queue, &element) == SP_BPQUEUE_SUCCESS);
        printf("%d", element.index);
        assert(spBPQueueDequeue(queue) == SP_BPQUEUE_SUCCESS);
        isEmpty = spBPQueueIsEmpty(queue);
        if (!isEmpty) printf(", ");
    }
    printf("\n");
}
//
// bool stringCompare(char* a, char* b){
//   if(sizeof(a) != sizeof(b)){
//     return false;
//   }
//   for (int i = 0; i < count; i++) {
//     /* code */
//   }
// }
