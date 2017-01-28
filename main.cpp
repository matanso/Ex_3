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

using namespace std;

#define MAX_BUFFER_SIZE 1024

#define DECISION_MSG "Enter a query image or # to terminate:\n"
#define EXIT_MSG "Exiting...\n"

int main() {
  char dirPath[MAX_BUFFER_SIZE], imgPrefix[MAX_BUFFER_SIZE],
      imgSuffix[MAX_BUFFER_SIZE];
  int numOfImages, nBins, nFeaturesToExtract;
  int *nFeaturesPerImage, *numOfQueryFeatures = 0;
  SPPoint **queryFeature, **queryHist;
  SPPoint ***imagesHist, ***imagesSift;
  // Input
  numOfImages = nBins = nFeaturesToExtract = 0;
  if (!mainAuxGetParameters(dirPath, imgPrefix, imgSuffix, numOfImages, nBins,
                            nFeaturesToExtract)) {
    return 0;
  }

  if ((imagesHist = (SPPoint ***)malloc(numOfImages * sizeof(SPPoint **))) ==
      NULL) {
    // TODO malloc error
  }
  if ((imagesSift = (SPPoint ***)malloc(numOfImages * sizeof(SPPoint **))) ==
      NULL) {
    // TODO malloc error
  }

  if ((nFeaturesPerImage = (int *)malloc(numOfImages * sizeof(int))) == NULL) {
    // TODO malloc error
  }

  char path[MAX_BUFFER_SIZE];
  // For each image in {dir_path} calculate histogram and SIFT
  for (int i = 0; i < numOfImages; i++) {
    mainAuxBuildPath(dirPath, imgPrefix, i, imgSuffix, path);
    imagesHist[i] = spGetRGBHist((const char *)path, i, nBins);
    imagesSift[i] = spGetSiftDescriptors(
        (const char *)path, i, nFeaturesToExtract, nFeaturesPerImage + i);
  }

  char decisonStr[MAX_BUFFER_SIZE];
  printf(DECISION_MSG);
  scanf("%s", decisonStr);
  if (stringCompare(decisonStr[0], '#')) {
    printf(EXIT_MSG);
    return 0;
  }

  do {

    queryHist = spGetRGBHist(decisonStr, -1, nBins);
    queryFeature = spGetSiftDescriptors(decisonStr, -1, nFeaturesToExtract,
                                        numOfQueryFeatures);

    mainAuxPrintGlobalDescriptor(imagesHist, queryHist, numOfImages, 5);

    mainAuxPrintLocalDescriptor(imagesSift, queryFeature, numOfQueryFeatures,
                                numOfImages, nFeaturesPerImage, 5);

    printf(DECISION_MSG);
    scanf("%s", decisonStr);
  } while (decisonStr[0] != '#');
  printf(EXIT_MSG);
  return 0;
}
== == == =
#include <opencv2/core.hpp>    //Mat
#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
//#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include "string.h"
#include <opencv2/features2d.hpp>
#include <vector>

             extern "C" {
#include "SPBPriorityQueue.h"
#include "SPPoint.h"
}

#include "main_aux.h"
#include "sp_image_proc_util.h"

using namespace std;

#define MAX_BUFFER_SIZE 1024

#define DECISION_MSG "Enter a query image or # to terminate:\n"
#define EXIT_MSG "Exiting...\n"

int main() {
  char dirPath[MAX_BUFFER_SIZE], imgPrefix[MAX_BUFFER_SIZE],
      imgSuffix[MAX_BUFFER_SIZE];
  int numOfImages, nBins, nFeaturesToExtract;
  int *nFeaturesPerImage;
  SPPoint **queryFeature, **queryHist;
  SPPoint ***imagesHist, ***imagesSift;
  // Input
  numOfImages = nBins = nFeaturesToExtract = 0;
  if (!mainAuxGetParameters(dirPath, imgPrefix, imgSuffix, numOfImages, nBins,
                            nFeaturesToExtract)) {
    return 0;
  }

  if ((imagesHist = (SPPoint ***)malloc(numOfImages * sizeof(SPPoint **))) ==
      NULL) {
    // TODO malloc error
  }
  if ((imagesSift = (SPPoint ***)malloc(numOfImages * sizeof(SPPoint **))) ==
      NULL) {
    // TODO malloc error
  }

  if ((nFeaturesPerImage = (int *)malloc(numOfImages * sizeof(int))) == NULL) {
    // TODO malloc error
  }

  char path[MAX_BUFFER_SIZE];
  // For each image in {dir_path} calculate histogram and SIFT
  for (int i = 0; i < numOfImages; i++) {
    mainAuxBuildPath(dirPath, imgPrefix, i, imgSuffix, path);
    imagesHist[i] = spGetRGBHist((const char *)path, i, nBins);
    imagesSift[i] = spGetSiftDescriptors(
        (const char *)path, i, nFeaturesToExtract, nFeaturesPerImage + i);
  }

  char decisonStr[MAX_BUFFER_SIZE];
  printf(DECISION_MSG);
  scanf("%s", decisonStr);

  if (decisonStr[0] == '#') {
    printf(EXIT_MSG);
    return 0;
  }

  // TODO Can we get rid of it?
  int *numOfQueryFeatures = 0;

  queryHist = spGetRGBHist(decisonStr, -1, nBins);
  queryFeature = spGetSiftDescriptors(decisonStr, -1, nFeaturesToExtract,
                                      numOfQueryFeatures);
  // free(queryNumberOfFeature);

  mainAuxPrintGlobalDescriptor(imagesHist, queryHist, numOfImages, 5);

  mainAuxPrintLocalDescriptor(imagesSift, queryFeature, numOfQueryFeatures,
                              numOfImages, nFeaturesPerImage, 5);

  return 0;
}
>>>>>>> 7cd5a88cab246c8cfd1dcda45a19e546038f2ac1
