#pragma once

#include "DiffFinderAlgorithmBase.hpp"

class DiffFinderAlgorithm2 : public DiffFinderAlgorithmBase
{
public:
    Result execute(const cv::Mat &reference, const cv::Mat &another, double threshold);

private:
    double getPSNR(const cv::Mat &I1, const cv::Mat &I2);
};