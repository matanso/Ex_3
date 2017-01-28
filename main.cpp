#include <stdio.h>
#include <cstdlib>

extern "C" {
#include "SPPoint.h"
}

#include "main_aux.h"

#define MAX_BUFFER_SIZE 1024

#define DECISION_MSG "Enter a query image or # to terminate:\n"
#define EXIT_MSG "Exiting...\n"

int main() {
    char dirPath[MAX_BUFFER_SIZE],
            imgPrefix[MAX_BUFFER_SIZE],
            imgSuffix[MAX_BUFFER_SIZE],
            decisionStr[MAX_BUFFER_SIZE];

    int numOfImages, nBins, nFeaturesToExtract, numOfQueryFeatures;
    SPPoint **query;
    // Input
    if (!mainAuxGetParameters(dirPath, imgPrefix, imgSuffix, numOfImages, nBins,
                              nFeaturesToExtract)) {
        return 0;
    }

    SPPoint **imagesHist[numOfImages], **imagesSift[numOfImages];
    int nFeaturesPerImage[numOfImages];

    char path[MAX_BUFFER_SIZE];
    // For each image in {dir_path} calculate histogram and SIFT
    for (int i = 0; i < numOfImages; i++) {
        mainAuxBuildPath(dirPath, imgPrefix, i, imgSuffix, path);
        imagesHist[i] = spGetRGBHist(path, i, nBins);
        imagesSift[i] = spGetSiftDescriptors(path, i, nFeaturesToExtract, nFeaturesPerImage + i);
        if(imagesHist[i] == NULL || imagesSift[i] == NULL) return 0;

    }

    printf(DECISION_MSG);
    scanf("%s", decisionStr);
    while (decisionStr[0] != '#') {
        // Global descriptors
        query = spGetRGBHist(decisionStr, -1, nBins);
        if(query == NULL) continue;
        mainAuxPrintGlobalDescriptor(imagesHist, query, numOfImages, 5);
        freePoints(query, 3);

        // Local descriptors
        query = spGetSiftDescriptors(decisionStr, -1, nFeaturesToExtract,
                                            &numOfQueryFeatures);
        if(query == NULL) continue;
        mainAuxPrintLocalDescriptor(imagesSift, query, numOfQueryFeatures,
                                    numOfImages, nFeaturesPerImage, 5);
        freePoints(query, numOfQueryFeatures);

        printf(DECISION_MSG);
        scanf("%s", decisionStr);
    }
    for(int i = 0; i < numOfImages; i++) {
        freePoints(imagesHist[i], 3);
        freePoints(imagesSift[i], nFeaturesPerImage[i]);
    }

    printf(EXIT_MSG);

    return 0;
}