#pragma once

#include "DiffFinderAlgorithmBase.hpp"

class DiffFinderAlgorithm1 : public DiffFinderAlgorithmBase
{
public:
    virtual Result execute(const cv::Mat& reference, const cv::Mat& another);

private:

};