#include "utils.hpp"

using namespace cv;

void bgr2grayscale(Mat& frame, int height, int width)
{
    Mat result(Size(width, height), CV_8UC1);
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++)
        {
            //0.299 ∙ Red + 0.587 ∙ Green + 0.114 ∙ Blue
            auto cur = frame.ptr<BGR>(j)[i];
            result.ptr<uchar>(j)[i] = cur.red * 0.299 + cur.green * 0.587 + 0.114 * cur.blue;
        }
    }
    frame = result;
}

void convolve(Mat& frame, const double matrix[], int ih, int iw, int mh, int mw)
{
    int cx = mw / 2;
    int cy = mh / 2;

    Mat result(Size(iw, ih), CV_8UC1);
    for (int i = 0; i < iw; ++i)
        for (int j = 0; j < ih; ++j)
        {
            uchar pixel = 0;
            for (int x = 0; x < mw; ++x)
                for (int y = 0; y < mh; ++y)
                {
                    if (i - cx + x < 0 || i - cx + x >= iw
                        || j - cy + y < 0 || j - cy + y >= ih)
                        pixel += 0.5 * 255 * matrix[y * mw + x];
                    else
                        pixel += frame.ptr<uchar>(j - cy + y)[i - cx + x] * matrix[y * mw + x];
                }
            result.ptr<uchar>(j)[i] = pixel < 0 ? 0
                                    : pixel > 255 ? 255
                                    : pixel; 
        }
    frame = result;
}

void gaussian(Mat& frame, int image_h, int image_w)
{
    double matrix[25] = { 1. / 256,  4. / 256,  6. / 256,  4. / 256,
                              1. / 256,  4. / 256,  16. / 256, 24. / 256,
                              16. / 256, 4. / 256,  6. / 256,  24. / 256,
                              36. / 256, 24. / 256, 6. / 256,  4. / 256,
                              16. / 256, 24. / 256, 16. / 256, 4. / 256,
                              1. / 256,  4. / 256,  6. / 256,  4. / 256,
                              1. / 256 };
        return convolve(frame, matrix, image_h, image_w, 5, 5);
}

void compute_edges(Mat& frame, int ih, int iw)
{
    // double matrix1[25] = { 0, 0, 0, 0, 0,
    //                         1, 0, 0, 0,-1,
    //                         1, 0, 0, 0,-1,
    //                         1, 0, 0, 0,-1,
    //                         0, 0, 0, 0, 0 };
    // double matrix2[25] = { 0, 0, 0, 0, 0,
    //                         -1, 0, 0, 0, 1,
    //                         -1, 0, 0, 0, 1,
    //                         -1, 0, 0, 0, 1,
    //                         0, 0, 0, 0, 0 };
    // double matrix3[25] = { 0,-1,-1,-1, 0,
    //                         0, 0, 0, 0, 0,
    //                         0, 0, 0, 0, 0,
    //                         0, 0, 0, 0, 0,
    //                         0, 1, 1, 1, 0 };
    // double matrix4[25] = { 0, 1, 1, 1, 0,
    //                         0, 0, 0, 0, 0,
    //                         0, 0, 0, 0, 0,
    //                         0, 0, 0, 0, 0,
    //                         0,-1,-1,-1, 0 };
    double matrix1[9] = { 1, 0,-1,
                            1, 0,-1,
                            1, 0,-1 };
    double matrix2[9] = { -1, 0, 1,
                            -1, 0, 1,
                            -1, 0, 1 };
    double matrix3[9] = { -1,-1,-1,
                            0, 0, 0,
                            1, 1, 1 };
    double matrix4[9] = { 1, 1, 1,
                            0, 0, 0,
                            -1,-1,-1 };
    

    Mat image1 = frame.clone();
    Mat image2 = frame.clone();
    Mat image3 = frame.clone();
    Mat image4 = frame.clone();

    convolve(image1, matrix1, ih, iw, 3, 3);
    convolve(image2, matrix2, ih, iw, 3, 3);
    convolve(image3, matrix3, ih, iw, 3, 3);
    convolve(image4, matrix4, ih, iw, 3, 3);


    for (int i = 0; i < iw; ++i)
        for (int j = 0; j < ih; ++j)
        {
            double threshold = 250;
            if (image1.ptr<uchar>(j)[i] > threshold || image2.ptr<uchar>(j)[i] > threshold || image3.ptr<uchar>(j)[i] > threshold || image4.ptr<uchar>(j)[i] > threshold)
                frame.ptr<uchar>(j)[i] = 0;
            else
                frame.ptr<uchar>(j)[i] = 255;
        }
}