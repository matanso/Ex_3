#include <stdio.h>
#include <cstdlib>
#include <assert.h>

extern "C" {
#include "SPPoint.h"
}

#include "main_aux.h"

#define MAX_BUFFER_SIZE 1024

#define ALLOC_ERR (const char *) "allocation failure"

#define DECISION_MSG "Enter a query image or # to terminate:\n"
#define EXIT_MSG "Exiting...\n"

int main() {
    char dirPath[MAX_BUFFER_SIZE],
            imgPrefix[MAX_BUFFER_SIZE],
            imgSuffix[MAX_BUFFER_SIZE],
            decisionStr[MAX_BUFFER_SIZE];

    int numOfImages, nBins, nFeaturesToExtract, numOfQueryFeatures;
    int *nFeaturesPerImage;
    SPPoint **query;
    SPPoint ***imagesHist, ***imagesSift;
    // Input
    numOfImages = nBins = nFeaturesToExtract = 0;
    if (!mainAuxGetParameters(dirPath, imgPrefix, imgSuffix, numOfImages, nBins,
                              nFeaturesToExtract)) {
        return 0;
    }

    imagesHist = (SPPoint ***) malloc(numOfImages * sizeof(SPPoint **));
    imagesSift = (SPPoint ***) malloc(numOfImages * sizeof(SPPoint **));
    nFeaturesPerImage = (int *) malloc(numOfImages * sizeof(int));

    if(imagesHist == NULL || imagesSift == NULL || nFeaturesPerImage == NULL) {
        printError(ALLOC_ERR);
        free(imagesHist);
        free(imagesSift);
        free(nFeaturesPerImage);
        return false;
    }

    char path[MAX_BUFFER_SIZE];
    // For each image in {dir_path} calculate histogram and SIFT
    for (int i = 0; i < numOfImages; i++) {
        mainAuxBuildPath(dirPath, imgPrefix, i, imgSuffix, path);
        imagesHist[i] = spGetRGBHist(path, i, nBins);
        imagesSift[i] = spGetSiftDescriptors(path, i, nFeaturesToExtract, nFeaturesPerImage + i);
    }

    printf(DECISION_MSG);
    scanf("%s", decisionStr);
    while (decisionStr[0] != '#') {
        // Global descriptors
        query = spGetRGBHist(decisionStr, -1, nBins);
        mainAuxPrintGlobalDescriptor(imagesHist, query, numOfImages, 5);
        freeQuery(query, 3);

        // Local descriptors
        query = spGetSiftDescriptors(decisionStr, -1, nFeaturesToExtract,
                                            &numOfQueryFeatures);
        mainAuxPrintLocalDescriptor(imagesSift, query, numOfQueryFeatures,
                                    numOfImages, nFeaturesPerImage, 5);
        freeQuery(query, numOfQueryFeatures);

        printf(DECISION_MSG);
        scanf("%s", decisionStr);
    }
    printf(EXIT_MSG);
    return 0;
}