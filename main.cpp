#include <iostream>
#include "sp_image_proc_util.h"

using namespace std;

int main() {
    SPPoint **shit = spGetRGBHist("/home/matan/SoftwareProject/Ex_3/images/img1.png", 0, 255);

    return 0;
}