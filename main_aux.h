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

/**
 * Frees the memory for each point in the histogram
 * @assertion - hist != NULL
 * @param hist - The histogram
 * @param size - The size of the histogram
 */
void freePoints(SPPoint **hist, int size);


/**
 * Builds a path to the image using the directory,
 * the prefix and the suffix of the image and the
 * index of the image, the path is saved in *path.
 * @param dir - The directory of the image
 * @param prefix - The prefix of the image
 * @param i - The index of the image
 * @param suffix - The suffix of the image
 * @param path - The path of the image
 */
void mainAuxBuildPath(char *dir, char *prefix, int i, char *suffix, char *path);

/**
 * A method to receive the parameters are used to locate and then
 * extract global and local descriptors.
 * The method validates that the input from the user is valifrom d
 * according to the requirements.
 * @param dirPath - a pointer which receives the directory path
 * @param imgPrefix - a pointer which receives the image prefix
 * @param imgSuffix - a pointer which receives the image suffix
 * @param numOfImages - a pointer which receives the number of images
 * @param nBins - a pointer which receives the number of bins for the histogram
 * @param nFeaturesToExtract - a pointer which receives the number features to extract
 * @return returns true if all the parameters received from the user
 * are valid, otherwise false.
 */
bool mainAuxGetParameters(char *dirPath, char *imgPrefix, char *imgSuffix,
                          int &numOfImages, int &nBins, int &nFeaturesToExtract);

/**
 * Prints the indexes of the  k closest images according to
 * the L2 distance of the histograms from the query histogram.
 * @param imagesHist - a pointer to the histograms
 * @param queryHist - a pointer to the query histogram
 * @param numberOfImages - the total number of images
 * @param k - the closeness required
 */
void mainAuxPrintGlobalDescriptor(SPPoint ***imagesHist, SPPoint **queryHist,
                                  int numberOfImages, int k);

/**
 * Prints the indexes of the k closest images to the query Sift
 * according to the number of hits, the hits are measured by k
 * closest features from the query for each of it features.
 * @param imagesSift - a pointer to the SIFT descriptors of the images
 * @param queryFeature - a pointer to the SIFT descriptor of the query image
 * @param numOfQueryFeatures - the number of features of the query image
 * @param numberOfImages - the number of images
 * @param nFeaturesPerImage - a pointer to the number of features per image
 *        such that the i entry is the number of features of the i image
 * @param k - the closeness required
 */
void mainAuxPrintLocalDescriptor(SPPoint ***imagesSift, SPPoint **queryFeature,
                                 int numOfQueryFeatures, int numberOfImages,
                                 int *nFeaturesPerImage, int k);

/**
 * Prints the indexes of the elements in increasing order
 * according to the value.
 * @assertion - queue != NULL
 * @param queue - a pointer to the queue
 */
void mainAuxPrintQueueIndex(SPBPQueue *queue);

/**
 * Prints an error message in a the required format
 * @param message - the error message to be printed
 */
void printError(const char *message);

#endif
