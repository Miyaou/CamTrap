#include "filters.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;
using namespace std;


void test_filter(Mat &frame, int height, int width)
{
    Mat gray;
    bgr2grayscale(frame, gray, height, width);
    width /= 2;
    height /= 2;
    gaussian(gray, height, width);

    canny(gray, 100, 80, 3);
    //Canny(gray, gray, 0, 30);

    int spread = 4;
    for (int i = spread; i < (2 * width) - spread; ++i)
        for (int j = spread; j < (2 * height) - spread; j++)
        {
            if (gray.ptr<uchar>(j/2)[i/2])
            {
                for (int oi = -spread/2; oi <= spread/2; oi++)
                {
                    for (int oj = -spread/2; oj <= spread/2; oj++)
                    {
                        frame.ptr<uchar>(j + oj)[3 * (i + oi)] = 0;
                        frame.ptr<uchar>(j + oj)[3 * (i + oi) + 1] = 0;
                        frame.ptr<uchar>(j + oj)[3 * (i + oi) + 2] = 0;
                    }
                }
            }
        }
    // frame = gray;
}

void borderlands_filter(Mat &frame, int height, int width)
{
    Mat cpy = frame.clone();
    Mat i = frame.clone();
    bgr2grayscale(i, cpy, height, width);
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