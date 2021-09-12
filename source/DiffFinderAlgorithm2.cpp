#include "DiffFinderAlgorithm2.hpp"

#include <iostream>
using namespace std;

Result DiffFinderAlgorithm2::execute(const cv::Mat &reference, const cv::Mat &another, double threshold)
{
    return getPSNR(reference, another) > threshold ? Result::Different : Result::Same;
}

double DiffFinderAlgorithm2::getPSNR(const cv::Mat &I1, const cv::Mat &I2)
{
    cv::Mat s1;
    double ret = 0.0;

    cv::absdiff(I1, I2, s1);                     // |I1 - I2|
    s1.convertTo(s1, CV_32F);                    // cannot make a square on 8 bits
    s1 = s1.mul(s1);                             // |I1 - I2|^2
    cv::Scalar s = sum(s1);                      // sum elements per channel
    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels
    if (sse <= 1e-10)                            // for small values return zero
        ret = 0.0;
    else
    {
        double mse = sse / (double)(I1.channels() * I1.total());
        double psnr = 10.0 * log10((255 * 255) / mse);
        ret = psnr;
    }

    return ret;
}