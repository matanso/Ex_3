#include "sp_image_proc_util.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
//#include <opencv2/features2d.hpp>
#include <iostream>
#include <opencv2/nonfree/features2d.hpp>
#include <vector>



#define IMG_LOAD_ERR "Image cannot be loaded - %s\n"
#define DIVIDED_BY_THREE *0.33

/**
 * Calculates the RGB channels histogram. The histogram will be stored in an array of
 * of points, each point has the index imageIndex. The array has three entries,
 * the first entry (i.e the array at index 0) is the red channel histogram, the second entity
 * is the green channel histogram and the third entry is the blue channel histogram.
 *
 * @param str - The path of the image for which the histogram will be calculated
 * @param nBins - The number of subdivision for the intensity histogram
 * @param imageIndex - The index of the given image
 * @return NULL if str is NULL or nBins <= 0 or allocation error occurred,
 *  otherwise a two dimensional array representing the histogram.
 */

SPPoint **spGetRGBHist(const char *str, int imageIndex, int nBins) {
    cv::Mat img = cv::imread(str);
    if (!img.data) {
        printf(IMG_LOAD_ERR, str);
        return NULL;
    }

    int channels[3];
    for (int i = 0; i < 3; i++) channels[i] = i;

    float range[] = {0, 256};
    const float *histRange = {range};

    bool uniform = true;
    bool accumulate = false;

    cv::Mat hist;
    SPPoint **out = (SPPoint **) malloc(3 * sizeof(SPPoint *));

/// Compute the histograms:
    for (int i = 0; i < 3; i++) {
        calcHist(&img, 1, &i, cv::Mat(), hist, 1, &nBins, &histRange, uniform, accumulate);
        double *data = (double *) malloc(nBins * sizeof(double));
        for (int j = 0; j < nBins; j++) data[j] = hist.data[j];
        out[i] = spPointCreate(data, nBins, imageIndex);
    }
    return out;
}

/**
 * Returns the average L2-squared distance between rgbHistA and rgbHistB.
 * Both histograms must have the same number of channels (in the case of RGB histogram its 3).
 * @param rgbHistA - RGB histogram of image A
 * @param rgbHistB - RGB histogram of image B
 * @return
 *		  -1 if:
 *			rgbHistA/rgbHistB is null,
 *		  otherwise the average L2-squared distance.
 */
double spRGBHistL2Distance(SPPoint **rgbHistA, SPPoint **rgbHistB) {
    if (rgbHistA == NULL || rgbHistB == NULL) return -1;
    double s = 0;
    for (int i = 0; i < 3; i++) {
        s += spPointL2SquaredDistance(rgbHistA[i], rgbHistB[i]);
    }
    return s DIVIDED_BY_THREE;
}

/**
 * Tries to extract SIFT descriptors from the image given by the string str (the number of features
 * to retain is given by the integer nFeaturesToExtract).
 * The function returns an array of points of size *numOfFeatures, each point represents a certain
 * SIFT feature in the image. Further all points have an index given by the parameter imageIndex.
 *
 * Notice that the actual number of features extracted may be less or more
 * then nFeaturesToExtract (hence we need to store the number of features
 * in address given by nFeatures).
 *
 * @param str - A string representing the path of the image
 * @param imageIndex - The index of the given image
 * @param nFeaturesToExtract - The number of features to retain
 * @param nFeatures - A pointer in which the actual number of features retained will be stored.
 * @return
 *         NULL if:
 * 		   	- str is NULL
 * 		   	- the image given by str didn't open
 * 		   	- nFeatures is NULL
 * 		   	- maxNFeatures <= 0
 * 		   	- Memory allocation failure
 *
 *		   Otherwise, the total number of features retained will be stored in
 * 		   nFeatures, and the actual features will be returned.
 */

SPPoint **spGetSiftDescriptors(const char *str, int imageIndex, int nFeaturesToExtract, int *nFeatures) {
/* // todo look at this
 * Works with OpenCV 3.2.0
 * download the shell script @ https://github.com/jayrambhia/Install-OpenCV
 * Just read the tutorial, works for with ubuntu 16.04
 * It includes nonfree/features2d
 * Still have problem to run the program
 * cv::SiftDescriptorExtractor.compute()
 */

}

/**
 * Given sift descriptors of the images in the database (databaseFeatures), finds the
 * closest kClosest to a given SIFT feature (queryFeature). The function returns the
 * INDEXES of the images to which the closest features belong, stored in ascending order
 * (Closest feature image index is first, second closest feature image index is second, etc...).
 * Assumptions:
 *   - Tie break - In case queryFeature has the same distance (L2Squared distance) from two features,
 *     then the feature that corresponds to the smallest image
 *     index in the database is closer.
 *
 *   - The returned result may contain duplicates in case two features belongs to the same image.
 *
 *   - databaseFeatures is a two dimensional array of sift features , the number of rows is given
 *     by the parameter numberOfImages. Each row (say row i) corresponds to the SIFT features
 *     of image i.
 *
 *   - The number of descriptors for the ith image is nFeaturesPerImage[i]
 *
 *   - For example, databaseFeatures[0] is an array of type SPPoint**, its size is nFeaturesPerImage[0].
 *     Each entry in databaseFeatures[0] is a sift feature of the image with the index i.
 *
 * @param kClosest          - The kClosest features to the queryFeature
 * @param queryFeature      - A sift descriptor which will be compared with the other descriptors
 * @param databaseFeatures  - All SIFT features in the database.
 * @param numberOfImages    - The number of images in the database. (Number of entries in databaseFeatures)
 * @param nFeaturesPerImage - The number of features per each image.
 *
 * @return - NULL if either the following occurs:
 * 			   * queryFeature is NULL
 * 			   * databaseFeatures is NULL
 * 			   * numberOfImages <= 1
 * 			   * nFeaturesPerImage is NULL
 * 			   * allocation error occurred
 *
 *         - Otherwise- an array of size kClosest is returned such that:
 * 			 * Given that f1, f2, ... the closest features to queryFeature (i.e d(queryFeature,f1) <= d(queryFeature,f2) <= ...)
 * 			 * i1, i2, .... are the indexes of the images to which fi belongs (i.e f1 is a SIFT descriptor of image i1,
 * 			   f2 is a SIFT descriptor of image i2 etc..)
 * 			 Then the array returned is {i1,i2,...,i_kClosest}
 */

int *spBestSIFTL2SquaredDistance(int kClosest, SPPoint *queryFeature,
                                 SPPoint ***databaseFeatures, int numberOfImages,
                                 int *nFeaturesPerImage) {

}

