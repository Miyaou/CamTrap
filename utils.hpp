#pragma once

#include <opencv2/core/mat.hpp>

struct BGR {
    uchar blue;
    uchar green;
    uchar red;  
    };

void bgr2grayscale(cv::Mat& frame, int height, int width);

void convolve(cv::Mat& frame, const double matrix[], int ih, int iw, int mh, int mw);

void gaussian(cv::Mat& frame, int image_h, int image_w);

void compute_edges(cv::Mat& frame, int image_h, int image_w);