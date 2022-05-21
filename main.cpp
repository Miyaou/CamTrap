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
    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);
    std::cout << width << "X" << height << "\n";
    Mat frame;
    namedWindow("test");
    //namedWindow("gaussian");

    int i = 0;
    while (i == 0)
    {
        cap >> frame;
        //cvtColor(frame, frame, COLOR_RGB2GRAY);
        borderlands_filter(frame, height, width);
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
    return 1;
}
