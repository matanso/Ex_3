#include <opencv2/core.hpp> //Mat
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp> //SiftDescriptorExtractor
#include <vector>

#ifndef EX_3_MAIN_AUX_H
#define EX_3_MAIN_AUX_H

extern "C" {
#include "SPBPriorityQueue.h"
#include "SPPoint.h"
}

#include "sp_image_proc_util.h"

int readNumberForMessage(const char *msg);

void readStringForMessage(const char *msg, char *result);

void mainAuxBuildPath(char *dir, char *prefix, int i, char *suffix, char *path);

bool mainAuxGetParameters(char *dirPath, char *imgPrefix, char *imgSuffix,
                          int numOfImages, int nBins, int nFeaturesToExtract);

void mainAuxPrintGlobalDescriptor(SPPoint ***imagesHist, SPPoint **queryHist,
                                  int numberOfImages, int k);

void mainAuxPrintLocalDescriptor(SPPoint ***imagesSift, SPPoint **queryFeature,
                                 int *numOfQueryFeatures, int numberOfImages,
                                 int *nFeaturesPerImage, int k);

void mainAuxPrintQueueIndex(SPBPQueue *queue);

int stringCompare(char *str1, char *str2);
#endif
