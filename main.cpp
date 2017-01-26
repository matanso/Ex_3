#include <iostream>
#include "sp_image_proc_util.h"

using namespace std;

int main() {
    SPPoint **shit = spGetRGBHist("./images/img2.jpg", 0, 255);
    if (shit == NULL) {
    	exit(1);
    }
    return 0;
}