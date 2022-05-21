#pragma once

#include <iostream>
#include <opencv2/core/mat.hpp>
#include "utils.hpp"

void borderlands_filter(cv::Mat &frame, int height, int width);

void test_filter(cv::Mat &frame, int height, int width);
