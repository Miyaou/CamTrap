#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "filters.hpp"

using namespace cv;
using namespace std;


// Read Video
void displayWebcam()
{
    VideoCapture cap(0);
    

    Mat frame = imread("../test.png");
    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);
    std::cout << width << "X" << height << "\n";
    // Mat frame = imread("../tiger.jpg");
    // int width = frame.cols;
    // int height = frame.rows;
    // test_filter(frame, height, width);

    //namedWindow("gaussian");
    namedWindow("test");
    while (true)
    {
        cap >> frame;
        test_filter(frame, height, width);
        imshow("test", frame);
        //step2(frame, height, width);
        //imshow("gaussian", frame);

        if ((char)waitKey(24) == 27)
            break;
    }
    cap.release();
    destroyAllWindows();
}

int main()
{
    displayWebcam();
    return 0;
}
