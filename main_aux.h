#ifndef EX_3_MAIN_AUX_H
#define EX_3_MAIN_AUX_H

extern "C" {
#include "SPBPriorityQueue.h"
#include "SPPoint.h"
}

#include "sp_image_proc_util.h"

#define ERR_MSG "An error occurred - %s\n"
#define IMG_NUM_ERR (const char *) "invalid number of images"
#define BIN_NUM_ERR (const char *) "invalid number of bins"
#define FEAT_NUM_ERR (const char *) "invalid number of features"
#define ALLOC_ERR (const char *) "allocation failure"

#define GLOBAL_DESC "Nearest images using global descriptors:\n"
#define LOCAL_DESC "Nearest images using local descriptors:\n"

#define PATH_MSG "Enter images directory path:\n"
#define PREFIX_MSG "Enter images prefix:\n"
#define NUM_IMGS_MSG "Enter number of images:\n"
#define SUFFIX_MSG "Enter images suffix:\n"
#define NUM_BINS_MSG "Enter number of bins:\n"
#define NUM_FEATS_MSG "Enter number of features:\n"

void freePoints(SPPoint **hist, int size);

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
