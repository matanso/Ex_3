#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>//Mat
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>
#include <vector>
#include "sp_image_proc_util.h"

#ifndef EX_3_MAIN_AUX_H
#define EX_3_MAIN_AUX_H

extern "C" {
#include "SPPoint.h"
#include "SPBPriorityQueue.h"
}

int readNumberForMessage(char * msg);

char* readStringForMessage(char* msg);

const char* mainAuxBuildPath(char* dir, char* prefix, int i, char* suffix);

bool mainAuxGetParameters(char** stringParametars,int* numericParametars);

void mainAuxGetGlobalDescriptor(SPPoint*** imagesHist, SPPoint* queryHist, int k);

#endif