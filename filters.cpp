#include "filters.hpp"

using namespace cv;
using namespace std;


void test_filter(Mat &frame, int height, int width)
{
    bgr2grayscale(frame, height, width);
}


void borderlands_filter(Mat &frame, int height, int width)
{
    std::cout << "START\n";
}