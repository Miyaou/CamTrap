#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

// Read Video
void displayWebcam()
{
    VideoCapture cap(0);
    Mat frame;

    while (1)
    {
        cap >> frame;

        imshow("Image", frame);

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
