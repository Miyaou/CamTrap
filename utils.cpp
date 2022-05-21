#include "utils.hpp"

void bgr2grayscale(cv::Mat& frame, int height, int width)
{
    cv::Mat result(cv::Size(width, height), CV_8UC1);
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++)
        {
            //0.299 ∙ Red + 0.587 ∙ Green + 0.114 ∙ Blue
            auto cur = frame.ptr<BGR>(i)[j];
            result.at<uchar>(i, j) = cur.red * 0.299 + cur.green * 0.587 + 0.114 * cur.blue;
        }
    }
    frame = result;
}
