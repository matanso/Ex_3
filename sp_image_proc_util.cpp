#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>//Mat
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>
#include <vector>
#include "sp_image_proc_util.h"

#include "SPPoint.h"
#include "SPBPriorityQueue.h"


// extern "C" {
// #include "SPPoint.h"
// #include "SPBPriorityQueue.h"
// } // todo find out if to put this in extern or not 


#define IMG_LOAD_ERR "Image cannot be loaded - %s\n"
// #define DIVIDED_BY_THREE *0.33


SPPoint **spGetRGBHist(const char *str, int imageIndex, int nBins) {
    // Check if str is NULL argument or nBins <= 0
    if(str == NULL || nBins <= 0){ return NULL; }
    // Load image
    cv::Mat img = cv::imread(str);
    // Check if image is loaded correctly 
    if (img.empty()) {
        printf(IMG_LOAD_ERR, str);
        return NULL;
    }

    // Set the ranges ( for (B,G,R) )
    float range[] = {0, 256};
    const float *histRange = {range};

    // Set the other parameters:
    int nImages = 1;

    //bool uniform = true; Not needed
    //bool accumulate = false; Not neneed

    cv::Mat hist;
    SPPoint **out = (SPPoint **) malloc(3 * sizeof(SPPoint *));

    // Compute the histograms:
    for (int i = 0; i < 3; i++) {
        calcHist(&img, nImages, &i, cv::Mat(), hist, 1, &nBins, &histRange);
        double *data = (double *) malloc(nBins * sizeof(double));
        for (int j = 0; j < nBins; j++) data[j] = hist.data[j];
        out[i] = spPointCreate(data, nBins, imageIndex);
        free(data);
    }
    return out;
}

double spRGBHistL2Distance(SPPoint **rgbHistA, SPPoint **rgbHistB) {
    if (rgbHistA == NULL || rgbHistB == NULL) return -1;
    double s = 0;
    for (int i = 0; i < 3; i++) {
        s += spPointL2SquaredDistance(rgbHistA[i], rgbHistB[i]);
    }
    return s * 0.33; //DIVIDED_BY_THREE;
}


SPPoint **spGetSiftDescriptors(const char *str, int imageIndex, 
                                int nFeaturesToExtract, int *nFeatures) {
    // Check if str is NULL argument
    if(str == NULL){ return NULL ;}
    // Check if nFeatures is NULL
    if(nFeatures == NULL){ return NULL ;}
    // Check if nFeaturesToExtract argument is valid
    if(nFeaturesToExtract <= 0){ return NULL ;}

    // Load image
    cv::Mat img = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
    // Check if image didn't load
    if (img.empty()) {
        printf(IMG_LOAD_ERR, str);
        return NULL;
    }

    int featuresDim;
    SPPoint** features;

    // Key points will be stored in kp1;
    std::vector<cv::KeyPoint> kp1;
    // Feature values will be stored in ds1;
    cv::Mat ds1;
    // Creating  a Sift Descriptor extractor
    cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor> detect =
            cv::xfeatures2d::SIFT::create(nFeaturesToExtract);
    // Extracting features
    // The features will be stored in ds1
    // The output type of ds1 is CV_32F (float)
    detect->detect(img, kp1, cv::Mat());
    detect->compute(img, kp1, ds1);
    
    *nFeatures = ds1.rows;
    featuresDim = ds1.cols;

    features = (SPPoint**) malloc(*nFeatures * sizeof(SPPoint*));
    if (features == NULL){ return NULL; }


    double* tmp_features = (double*) malloc(*nFeatures*sizeof(*tmp_features));
    for(int i = 0; i< *nFeatures; i++){
        for (int j = 0; j < featuresDim; j++){
            tmp_features[j] = ds1.at<float>(i,j);
        }
        features[i] = spPointCreate(tmp_features, featuresDim, imageIndex);
    }
    free(tmp_features);
    return features;
}

int *spBestSIFTL2SquaredDistance(int kClosest, SPPoint *queryFeature,
                                 SPPoint ***databaseFeatures, int numberOfImages,
                                 int *nFeaturesPerImage) {
    // Check if queryFeature is NULL
    if(queryFeature == NULL) { return NULL; }
    // Check if databaseFeatures is NULL
    if(databaseFeatures == NULL) { return NULL; }
    // Check if nFeaturesPerImage is NULL
    if(nFeaturesPerImage == NULL) { return NULL; }
    // Check if numberOfImages <= 1
    if(numberOfImages <= 1){ return NULL; }

    // Create a min-priority queue of size kClosest
    SPBPQueue *KClosestImages = spBPQueueCreate(kClosest);
    if(KClosestImages == NULL){ return NULL; }

    for(int i = 0; i < numberOfImages; i++){
        for(int j = 0; j<nFeaturesPerImage[i]; j++){
            int dist = spPointL2SquaredDistance(databaseFeatures[i][j], queryFeature);
            // TODO the enqueue should take to consideration the index, if 
            // two featutes have same distance with queryFeature, then
            // the smallest index is considered to be closer.
            if(spBPQueueEnqueue(KClosestImages, i, dist) == SP_BPQUEUE_INVALID_ARGUMENT){
                // KClosestImages is NULL argument
            }
        }
    }

    // The returned array of closest features to queryFeature
    int* sortedImagesByFeatures = (int*) malloc(spBPQueueSize(KClosestImages)*sizeof(int));
    BPQueueElement element;
    for (int i = 0; i < spBPQueueSize(KClosestImages) ; i++){
        spBPQueuePeek(KClosestImages, &element);
        sortedImagesByFeatures[i] = element.index;
    }

    spBPQueueDestroy(KClosestImages);
    return sortedImagesByFeatures;

}

