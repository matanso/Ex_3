#include <opencv2/core.hpp> //Mat
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp> //SiftDescriptorExtractor
#include <vector>

extern "C" {
#include "SPBPriorityQueue.h"
#include "SPPoint.h"
}

#include "main_aux.h"
#include "sp_image_proc_util.h"

#define MAX_BUFFER_SIZE 1024

#define ERR_MSG "An error occurred - "
#define IMG_NUM_ERR "invalid number of images\n"
#define BIN_NUM_ERR "invalid number of bins\n"
#define FEAT_NUM_ERR "invalid number of features\n"
#define GLOBAL_DESC "Nearest images using global descriptors:\n"

int readNumberUsingMessage(const char *msg) {
  int number;
  printf("%s", msg);
  scanf("%d", &number);
  return number;
}

void readStringUsingMessage(const char *msg, char *result) {
  printf("%s", msg);
  scanf("%s", result);
}

void mainAuxBuildPath(char *dir, char *prefix, int i, char *suffix,
                      char *path) {
  sprintf(path, "%s%s%d%s", dir, prefix, i, suffix);
}

bool mainAuxGetParameters(char *dirPath, char *imgPrefix, char *imgSuffix,
                          int numOfImages, int nBins, int nFeaturesToExtract) {
  // Input
  // Directory path to images
  readStringUsingMessage("Enter images directory path:\n", dirPath);

  // Images prefix
  readStringUsingMessage("Enter images prefix:\n", imgPrefix);

  // Number of images
  numOfImages = readNumberUsingMessage("Enter number of images:\n");
  if (numOfImages < 1) {
    printf("%s%s", ERR_MSG, IMG_NUM_ERR);
    return false;
  }

  // Images suffix
  readStringUsingMessage("Enter images suffix:\n", imgSuffix);

  // Number of Bins
  nBins = readNumberUsingMessage("Enter number of bins:\n");
  if (nBins < 1 || nBins > 255) {
    printf("%s%s", ERR_MSG, BIN_NUM_ERR);
    return false;
  }

  // Number of features to extract
  nFeaturesToExtract = readNumberUsingMessage("Enter number of features:\n");
  if (nFeaturesToExtract < 1) {
    printf("%s%s", ERR_MSG, FEAT_NUM_ERR);
    return false;
  }
  return true;
}

void mainAuxPrintGlobalDescriptor(SPPoint ***imagesHist, SPPoint **queryHist,
                                  int numberOfImages, int k) {
  // Create a min-priority queue of size kClosest
  SPBPQueue *KClosestImages;
  if ((KClosestImages = spBPQueueCreate(k)) == NULL) {
    return;
  }
  for (int i = 0; i < numberOfImages; i++) {
    double dist = spRGBHistL2Distance(imagesHist[i], queryHist);
    if (spBPQueueEnqueue(KClosestImages, i, dist) ==
        SP_BPQUEUE_INVALID_ARGUMENT) {
      // TODO KClosestImages is NULL argument
    }
  }
  printf(GLOBAL_DESC);
  mainAuxPrintQueueIndex(KClosestImages);
}

void mainAuxPrintLocalDescriptor(SPPoint ***imagesSift, SPPoint **queryFeature,
                                 int *numOfQueryFeatures, int numberOfImages,
                                 int *nFeaturesPerImage, int k) {
  int *indexes = (int *)malloc(k * sizeof(int));
  for (int j = 0; j < *numOfQueryFeatures; j++) {
    indexes = spBestSIFTL2SquaredDistance(k, queryFeature[j], imagesSift,
                                          numberOfImages, nFeaturesPerImage);
    for (int i = 0; i < k; i++) {
      printf("%d", indexes[i]);
      if (i != k) {
        printf(", ");
      } else {
        break;
      }
    }
  }
  free(indexes);
  printf("\n");
}

void mainAuxPrintQueueIndex(SPBPQueue *queue) {
  if (queue == NULL)
    return;
  BPQueueElement element;
  bool isEmpty = spBPQueueIsEmpty(queue);
  while (!isEmpty) {
    assert(spBPQueuePeek(queue, &element) == SP_BPQUEUE_SUCCESS);
    printf("%d", element.index);
    assert(spBPQueueDequeue(queue) == SP_BPQUEUE_SUCCESS);
    isEmpty = spBPQueueIsEmpty(queue);
    if (!isEmpty)
      printf(", ");
  }
  printf("\n");
}

bool stringCompare(char *str1, char *str2) {
  if (str1 == NULL || str2 == NULL)
    return false;
  if (sizeof(str1) != sizeof(str2))
    return false;
  for (int i = 0; i < sizeof(a) / sizeof(char); i++)
    if (str1[i] != str2[i])
      return false;
  return true;
}
