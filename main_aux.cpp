#include <stdio.h>
#include <cstdlib>
#include <assert.h>

extern "C" {
#include "SPBPriorityQueue.h"
#include "SPPoint.h"
}

#include "main_aux.h"

#define MAX_BUFFER_SIZE 1024

#define ERR_MSG "An error occurred - %s\n"
#define IMG_NUM_ERR (const char *) "invalid number of images"
#define BIN_NUM_ERR (const char *) "invalid number of bins"
#define FEAT_NUM_ERR (const char *) "invalid number of features"
#define GLOBAL_DESC "Nearest images using global descriptors:\n"
#define LOCAL_DESC "Nearest images using local descriptors:\n"

void readNumberUsingMessage(const char *msg, int &number) {
    printf("%s", msg);
    scanf("%d", &number);
}

void readStringUsingMessage(const char *msg, char *result) {
    printf("%s", msg);
    scanf("%s", result);
}

int cmpfunc(const void *a, const void *b) {
    return (int) (((BPQueueElement *) b)->value - ((BPQueueElement *) a)->value);
}


void freeQuery(SPPoint **hist, int size) {
    if (hist == NULL) return;
    for (int i = 0; i < size; i++) spPointDestroy(hist[i]);
    free(hist);
}


void mainAuxBuildPath(char *dir, char *prefix, int i, char *suffix,
                      char *path) {
    sprintf(path, "%s%s%d%s", dir, prefix, i, suffix);
}

bool mainAuxGetParameters(char *dirPath, char *imgPrefix, char *imgSuffix,
                          int &numOfImages, int &nBins, int &nFeaturesToExtract) {
    // Directory path to images
    readStringUsingMessage("Enter images directory path:\n", dirPath);

    // Images prefix
    readStringUsingMessage("Enter images prefix:\n", imgPrefix);

    // Number of images
    readNumberUsingMessage("Enter number of images:\n", numOfImages);
    if (numOfImages < 1) {
        printError(IMG_NUM_ERR);
        return false;
    }

    // Images suffix
    readStringUsingMessage("Enter images suffix:\n", imgSuffix);

    // Number of Bins
    readNumberUsingMessage("Enter number of bins:\n", nBins);
    if (nBins < 1 || nBins > 255) {
        printError(BIN_NUM_ERR);
        return false;
    }

    // Number of features to extract
    readNumberUsingMessage("Enter number of features:\n", nFeaturesToExtract);
    if (nFeaturesToExtract < 1) {
        printError(FEAT_NUM_ERR);
        return false;
    }
    return true;
}

void mainAuxPrintGlobalDescriptor(SPPoint ***imagesHist, SPPoint **queryHist,
                                  int numberOfImages, int k) {
    // Create a min-priority queue of size kClosest
    SPBPQueue *KClosestImages = spBPQueueCreate(k);
    if (KClosestImages == NULL) {
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
                                 int numOfQueryFeatures, int numberOfImages,
                                 int *nFeaturesPerImage, int k) {
    printf(LOCAL_DESC);
    int *indexes;
    BPQueueElement hits[numberOfImages];
    for (int i = 0; i < numberOfImages; i++) {
        hits[i].index = i;
        hits[i].value = 0;
    }

    for (int j = 0; j < numOfQueryFeatures; j++) {
        indexes = spBestSIFTL2SquaredDistance(k, queryFeature[j], imagesSift,
                                              numberOfImages, nFeaturesPerImage);
        for (int i = 0; i < k; i++) hits[indexes[i]].value++;
        free(indexes);
    }
    qsort(hits, (size_t) numberOfImages, sizeof(BPQueueElement), cmpfunc);
    for (int i = 0; i < k - 1; i++) printf("%d, ", hits[i].index);
    printf("%d\n", hits[k - 1].index);
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

void printError(const char *message) {
    printf(ERR_MSG, message);
}