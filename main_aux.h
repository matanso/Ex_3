#ifndef EX_3_MAIN_AUX_H
#define EX_3_MAIN_AUX_H

extern "C" {
#include "SPBPriorityQueue.h"
#include "SPPoint.h"
}

#include "sp_image_proc_util.h"


void freeQuery(SPPoint **hist, int size);

void mainAuxBuildPath(char *dir, char *prefix, int i, char *suffix, char *path);

bool mainAuxGetParameters(char *dirPath, char *imgPrefix, char *imgSuffix,
                          int &numOfImages, int &nBins, int &nFeaturesToExtract);

void mainAuxPrintGlobalDescriptor(SPPoint ***imagesHist, SPPoint **queryHist,
                                  int numberOfImages, int k);

void mainAuxPrintLocalDescriptor(SPPoint ***imagesSift, SPPoint **queryFeature,
                                 int numOfQueryFeatures, int numberOfImages,
                                 int *nFeaturesPerImage, int k);

void mainAuxPrintQueueIndex(SPBPQueue *queue);

void printError(const char *message);

#endif
