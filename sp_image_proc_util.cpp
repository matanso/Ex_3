#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "sp_image_proc_util.h"
#include "main_aux.h"


#define IMG_LOAD_ERR "Image cannot be loaded - %s\n"

SPPoint **spGetRGBHist(const char *str, int imageIndex, int nBins) {
    // Check if str is NULL argument or nBins <= 0
    if (str == NULL || nBins <= 0 || nBins > 255) return NULL;
    // Load image
    cv::Mat img = cv::imread(str, CV_LOAD_IMAGE_COLOR);
    // Check if image is loaded correctly
    if (img.empty()) {
        printf(IMG_LOAD_ERR, str);
        return NULL;
    }

    // Set the ranges ( for (B,G,R) )
    float range[] = {0, 256};
    const float *histRange = {range};

    // Set the other parameters
    int nImages = 1;

    cv::Mat hist;
    SPPoint **out = (SPPoint **) malloc(3 * sizeof(SPPoint *));
    if (out == NULL)
        return NULL;


    double *data = (double *) malloc(nBins * sizeof(double));
    if (data == NULL) {
        printError(ALLOC_ERR);
        free(out);
        return NULL;
    }

    // Compute the histograms:
    for (int i = 0; i < 3; i++) {
        calcHist(&img, nImages, &i, cv::Mat(), hist, 1, &nBins, &histRange);
        for (int j = 0; j < nBins; j++) data[j] = hist.data[j];
        out[i] = spPointCreate(data, nBins, imageIndex);
    }
    free(data);
    return out;
}

double spRGBHistL2Distance(SPPoint **rgbHistA, SPPoint **rgbHistB) {
    if (rgbHistA == NULL || rgbHistB == NULL) return -1;
    double s = 0;
    for (int i = 0; i < 3; i++) {
        s += spPointL2SquaredDistance(rgbHistA[i], rgbHistB[i]);
    }
    return s * 0.33;
}


SPPoint **spGetSiftDescriptors(const char *str, int imageIndex,
                               int nFeaturesToExtract, int *nFeatures) {

    if (str == NULL || nFeatures == NULL || nFeaturesToExtract <= 0) return NULL;

    // Load image
    cv::Mat img = cv::imread(str);
    // Check if the image didn't load
    if (img.empty()) {
        printf(IMG_LOAD_ERR, str);
        return NULL;
    }

    // Key points will be stored in kp;
    std::vector<cv::KeyPoint> kp;
    // Feature values will be stored in ds;
    cv::Mat ds;
    // Creating  a Sift Descriptor extractor
    cv::SiftDescriptorExtractor detect = cv::SIFT(nFeaturesToExtract);
    // Extracting features
    // The features will be stored in ds
    // The output type of ds is CV_32F (float)
    detect.detect(img, kp, cv::Mat());
    detect.compute(img, kp, ds);

    *nFeatures = ds.rows;
    int featuresDim = ds.cols;

    SPPoint **features = (SPPoint **) malloc(*nFeatures * sizeof(SPPoint *));
    if (features == NULL) {
        printError(ALLOC_ERR);
        return NULL;
    }


    double *tmp_features = (double *) malloc(featuresDim * sizeof(double));
    if (tmp_features == NULL) {
        printError(ALLOC_ERR);
        free(features);
        return NULL;
    }

    for (int i = 0; i < *nFeatures; i++) {
        for (int j = 0; j < featuresDim; j++) {
            tmp_features[j] = ds.at<float>(i, j);
        }
        features[i] = spPointCreate(tmp_features, featuresDim, imageIndex);
    }
    free(tmp_features);
    return features;
}

int *spBestSIFTL2SquaredDistance(int kClosest, SPPoint *queryFeature,
                                 SPPoint ***databaseFeatures, int numberOfImages,
                                 int *nFeaturesPerImage) {
    if (queryFeature == NULL || databaseFeatures == NULL || nFeaturesPerImage == NULL || numberOfImages <= 1)
        return NULL;

    // Create a min-priority queue of size kClosest
    SPBPQueue *KClosestImages = spBPQueueCreate(kClosest);
    if (KClosestImages == NULL) return NULL;

    for (int i = 0; i < numberOfImages; i++) {
        for (int j = 0; j < nFeaturesPerImage[i]; j++) {
            double dist = spPointL2SquaredDistance(databaseFeatures[i][j], queryFeature);
            if (spBPQueueEnqueue(KClosestImages, i, dist) != SP_BPQUEUE_SUCCESS) {
                spBPQueueDestroy(KClosestImages);
                return NULL;
            }
        }
    }

    // The returned array of closest features to queryFeature
    int size = spBPQueueSize(KClosestImages);
    int *sortedImagesByFeatures = (int *) malloc(size * sizeof(int));
    if (sortedImagesByFeatures == NULL) {
        printError(ALLOC_ERR);
        spBPQueueDestroy(KClosestImages);
        return NULL;
    }
    BPQueueElement element;
    for (int i = 0; i < size; i++) {
        spBPQueuePeek(KClosestImages, &element);
        sortedImagesByFeatures[i] = element.index;
        spBPQueueDequeue(KClosestImages);
    }

    // Free memory
    spBPQueueDestroy(KClosestImages);
    return sortedImagesByFeatures;
}