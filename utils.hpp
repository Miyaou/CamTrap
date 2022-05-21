#pragma once

#include <opencv2/core/mat.hpp>

struct BGR {
    uchar blue;
    uchar green;
    uchar red;  
    };

void bgr2grayscale(cv::Mat& frame, int height, int width);