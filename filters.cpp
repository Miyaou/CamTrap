#include "filters.hpp"

using namespace cv;
using namespace std;


void test_filter(Mat &frame, int height, int width)
{
    bgr2grayscale(frame, height, width);
    gaussian(frame, height, width);
}

void step1(Mat &frame, int height, int width)
{
    bgr2grayscale(frame, height, width);
}

void step2(Mat &frame, int height, int width)
{
    gaussian(frame, height, width);
    //compute_edges(frame, height, width);
}

void borderlands_filter(Mat &frame, int height, int width)
{
    Mat cpy = frame.clone();

    bgr2grayscale(cpy, height, width);
    gaussian(cpy, height, width);
    compute_edges(cpy, height, width);

    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; j++)
    {
        if (!cpy.ptr<uchar>(j)[i])
        {
            frame.ptr<uchar>(j)[3 * i] = 0;
            frame.ptr<uchar>(j)[3 * i+1] = 0;
            frame.ptr<uchar>(j)[3 * i+2] = 0;
        }
    }
}