#include "filters.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;
using namespace std;


void test_filter(Mat &frame, int height, int width)
{
    Mat gray;
    bgr2grayscale(frame, gray, height, width);
    gaussian(gray, height, width);
    //GaussianBlur(frame, frame, Size(5, 5), 1.4);
    canny(gray, 120, 60, 3);
    // cv::Sobel(frame, Gx, CV_8UC1, 1, 0, 5);
    // cv::Sobel(frame, Gy, CV_8UC1, 0, 1, 5);
    //cv::Sobel(frame, frame, CV_64FC1, 1, 1, 5);

    //mayCanny(G, Gx, Gy, height, width, 0, 30, 3);
 
    int spread = 2;
    for (int i = spread; i < width - spread; ++i)
        for (int j = spread; j < height - spread; j++)
        {
            if (gray.ptr<uchar>(j)[i])
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