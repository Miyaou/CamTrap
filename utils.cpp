#include "utils.hpp"
#include <iostream>
using namespace cv;

void bgr2grayscale(Mat &frame, Mat &result, int height, int width)
{
    Mat res = Mat(Size(width/2, height/2), CV_8UC1);
    for (int i = 0; i < width; i += 2)
    {
        for (int j = 0; j < height; j += 2)
        {
            // 0.299 ∙ Red + 0.587 ∙ Green + 0.114 ∙ Blue
            auto cur = frame.ptr<BGR>(j)[i];
            res.ptr<uchar>(j/2)[i/2] = cur.red * 0.299 + cur.green * 0.587 + 0.114 * cur.blue;
        }
    }
    result = res;
}

void convolve(Mat &frame, Mat &res, const double matrix[], int ih, int iw, int mh, int mw)
{
    int cx = mw / 2;
    int cy = mh / 2;

    Mat result(Size(iw, ih), CV_8UC1);
    for (int i = 0; i < iw; ++i)
        for (int j = 0; j < ih; ++j)
        {
            double pixel = 0;
            for (int x = 0; x < mw; ++x)
                for (int y = 0; y < mh; ++y)
                {
                    if (i - cx + x < 0 || i - cx + x >= iw || j - cy + y < 0 || j - cy + y >= ih)
                        pixel += 0.5 * 255 * matrix[y * mw + x];
                    else
                        pixel += frame.ptr<uchar>(j - cy + y)[i - cx + x] * matrix[y * mw + x];
                }
            result.ptr<uchar>(j)[i] = pixel < 0     ? 0
                                      : pixel > 255 ? 255
                                                    : pixel;
        }
    res = result;
}

void gaussian(Mat &frame, int image_h, int image_w)
{
    double matrix[25] = {1. / 256, 4. / 256, 6. / 256, 4. / 256,
                         1. / 256, 4. / 256, 16. / 256, 24. / 256,
                         16. / 256, 4. / 256, 6. / 256, 24. / 256,
                         36. / 256, 24. / 256, 6. / 256, 4. / 256,
                         16. / 256, 24. / 256, 16. / 256, 4. / 256,
                         1. / 256, 4. / 256, 6. / 256, 4. / 256,
                         1. / 256};
    convolve(frame, frame, matrix, image_h, image_w, 5, 5);
}

void box_blur(Mat &frame, int image_h, int image_w)
{
    double matrix[25] = {1. / 25, 4. / 25, 6. / 25, 4. / 25,
                         1. / 25, 4. / 25, 16. / 25, 24. / 25,
                         16. / 25, 4. / 25, 6. / 25, 24. / 25,
                         36. / 25, 24. / 25, 6. / 25, 4. / 25,
                         16. / 25, 24. / 25, 16. / 25, 4. / 25,
                         1. / 25, 4. / 25, 6. / 25, 4. / 25,
                         1. / 256};
    return convolve(frame, frame, matrix, image_h, image_w, 5, 5);
}

void sobel(Mat &frame, Mat &Gx, Mat &Gy, int ih, int iw)
{
    // Classic Sobel
    // const double mGx[] = {-5, -4, 0, 4, 5,
    //                       -8, -10, 0, 10, 8,
    //                       -10, -20, 0, 20, 10,
    //                       -8, -10, 0, 10, 8,
    //                       -5, -4, 0, 4, 5};

    // const double mGy[] = {-5, -8, -10, -8, -5,
    //                       -4, -10, -20, -10, 4,
    //                       0, 0, 0, 0, 0,
    //                       4, 10, 20, 10, 4,
    //                       5, 8, 10, 8, 5};
    // int ksize = 5;

    // Scharr
    const double mGx[] = {-3, 0, 3,
                          -10, 0, 10,
                          -3, 0, 3};

    const double mGy[] = {-3, -10, -3,
                          0, 0, 0,
                          3, 10, 3};
    int ksize = 3;

    convolve(frame, Gx, mGx, ih, iw, ksize, ksize);
    convolve(frame, Gy, mGy, ih, iw, ksize, ksize);
}

void compute_edges(Mat &frame, int ih, int iw)
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
    double matrix1[9] = {1, 0, -1,
                         1, 0, -1,
                         1, 0, -1};
    double matrix2[9] = {-1, 0, 1,
                         -1, 0, 1,
                         -1, 0, 1};
    double matrix3[9] = {-1, -1, -1,
                         0, 0, 0,
                         1, 1, 1};
    double matrix4[9] = {1, 1, 1,
                         0, 0, 0,
                         -1, -1, -1};

    Mat image1, image2, image3, image4;

    convolve(frame, image1, matrix1, ih, iw, 3, 3);
    convolve(frame, image2, matrix2, ih, iw, 3, 3);
    convolve(frame, image3, matrix3, ih, iw, 3, 3);
    convolve(frame, image4, matrix4, ih, iw, 3, 3);

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
static uchar approx_angle(double angle)
{
    if ((angle >= -22.5 && angle <= 22.5) || (angle <= -157.5) || (angle >= 157.5))
        return 0; // "-"
    else if ((angle > 22.5 && angle <= 67.5) || (angle > -157.5 && angle <= -112.5))
        return 45; // "/"
    else if ((angle > 67.5 && angle <= 112.5) || (angle >= -112.5 && angle < -67.5))
        return 90; // "|"
    else if ((angle >= -67.5 && angle < -22.5) || (angle > 112.5 && angle < 157.5))
        return 135; // "\""
    return 255;     // ??????
}

void canny(cv::Mat &frame, double h1, double h2, int aperture)
{
    int ih = frame.rows;
    int iw = frame.cols;
    int offset = aperture / 2;
    double gx, gy, g;

    // Sobel Gradients
    Mat Gx, Gy;
    sobel(frame, Gx, Gy, ih, iw);

    // Intensity and direction gradients
    Mat G(Size(iw, ih), CV_8UC1);
    Mat Gdir(Size(iw, ih), CV_8UC1);
    for (int i = 0; i < iw; i++)
    {
        for (int j = 0; j < ih; j++)
        {
            gx = Gx.ptr<uchar>(j)[i];
            gy = Gy.ptr<uchar>(j)[i];
            g = abs(gx) + abs(gy); // Rather than std::sqrt(gx * gx + gy * gy) to optimise
            // g = sqrt(gx * gx + gy * gy);
            G.ptr<uchar>(j)[i] = uchar(g);
            Gdir.ptr<uchar>(j)[i] = approx_angle(atan(gy/gx) * 180 / M_PI);
            // std::cout << atan2(gx, gy) << "\n";
        }
    }

    // Non maximum suppression
    for (int i = offset; i < iw - offset; ++i)
    {
        for (int j = offset; j < ih - offset; ++j)
        {
            switch (Gdir.ptr<uchar>(j)[i])
            {
            case 0: // "-"
                if (G.ptr<uchar>(j)[i] <= G.ptr<uchar>(j)[i - 1] || G.ptr<uchar>(j)[i] <= G.ptr<uchar>(j)[i + 1])
                    G.ptr<uchar>(j)[i] = 0;
                break;
            case 45:
                if (G.ptr<uchar>(j)[i] <= G.ptr<uchar>(j - 1)[i + 1] || G.ptr<uchar>(j)[i] <= G.ptr<uchar>(j + 1)[i - 1])
                    G.ptr<uchar>(j)[i] = 0;
                break;

            case 90:
                if (G.ptr<uchar>(j)[i] <= G.ptr<uchar>(j - 1)[i] || G.ptr<uchar>(j)[i] <= G.ptr<uchar>(j + 1)[i])
                    G.ptr<uchar>(j)[i] = 0;
                break;

            case 135:
                if (G.ptr<uchar>(j)[i] <= G.ptr<uchar>(j - 1)[i - 1] || G.ptr<uchar>(j)[i] <= G.ptr<uchar>(j + 1)[i + 1])
                    G.ptr<uchar>(j)[i] = 0;
                break;
            default:
                G.ptr<uchar>(j)[i] = 0;
                break;
            }
        }
    }

    // Double Threshold
    for (int i = 0; i < iw; i++)
    {
        for (int j = 0; j < ih; j++)
        {
            if (G.ptr<uchar>(j)[i] > h1)
                frame.ptr<uchar>(j)[i] = 255;
            else if (G.ptr<uchar>(j)[i] > h2)
                frame.ptr<uchar>(j)[i] = 100;
            else
                frame.ptr<uchar>(j)[i] = 0;
        }
    }

    for (int k = 0; k < 2; k++)
        for (int i = offset; i < iw - offset; ++i)
        {
            for (int j = offset; j < ih - offset; ++j)
            {
                if (frame.ptr<uchar>(j)[i] == 255)
                    frame.ptr<uchar>(j)[i] = 255;
                if (frame.ptr<uchar>(j)[i] == 100)
                {
                    switch (Gdir.ptr<uchar>(j)[i])
                    {
                    case 0:
                        if (frame.ptr<uchar>(j)[i - 1] == 255 || frame.ptr<uchar>(j)[i + 1] == 255)
                            frame.ptr<uchar>(j)[i] = 255;
                        break;
                    case 45:
                        if (frame.ptr<uchar>(j + 1)[i - 1] == 255 || frame.ptr<uchar>(j - 1)[i + 1] == 255)
                            frame.ptr<uchar>(j)[i] = 255;
                        break;
                    case 90:
                        if (frame.ptr<uchar>(j - 1)[i] == 255 || frame.ptr<uchar>(j + 1)[i] == 255)
                            frame.ptr<uchar>(j)[i] = 255;
                        break;
                    case 135:
                        if (frame.ptr<uchar>(j - 1)[i - 1] == 255 || frame.ptr<uchar>(j + 1)[i + 1] == 255)
                            frame.ptr<uchar>(j)[i] = 255;
                        break;
                    default:
                        frame.ptr<uchar>(j)[i] = 0;
                        break;
                    }
                }
            }
        }
}
