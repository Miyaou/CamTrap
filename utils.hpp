#pragma once

#include <opencv2/core/mat.hpp>

struct BGR {
    uchar blue;
    uchar green;
    uchar red;  
    };

void bgr2grayscale(cv::Mat& frame, cv::Mat& result, int height, int width);

void convolve(cv::Mat& frame, cv::Mat& res, const double matrix[], int ih, int iw, int mh, int mw);

void gaussian(cv::Mat& frame, int image_h, int image_w);
void box_blur(cv::Mat& frame, int image_h, int image_w);

void sobel(cv::Mat& frame, cv::Mat& Gx, cv::Mat& Gy, int ih, int iw);
void compute_edges(cv::Mat& frame, int image_h, int image_w);
void canny(cv::Mat& frame, double h1, double h2, int aperture);
