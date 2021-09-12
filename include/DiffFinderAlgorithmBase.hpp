#pragma once

#include <opencv2/imgproc.hpp> 

enum class Result {
    Init = 0, 
    Same,
    Different,
    Image_CanNOT_Load
};

class DiffFinderAlgorithmBase
{
public:
    virtual Result execute(const cv::Mat& reference, const cv::Mat& another, double threshold) = 0;
};