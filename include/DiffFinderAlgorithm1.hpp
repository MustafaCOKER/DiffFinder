#pragma once

#include "DiffFinderAlgorithmBase.hpp"

class DiffFinderAlgorithm1 : public DiffFinderAlgorithmBase
{
public:
    Result execute(const cv::Mat &reference, const cv::Mat &another, double threshold);

private:
    // private functions
    void paintBackground(const cv::Mat &source, cv::Mat &painted) const;
    double diffImage(const cv::Mat &im1, const cv::Mat &im2, cv::Mat &mask) const;
    double findMaxBlobRatio(const cv::Mat &mask) const;
    int applyThreshold2Diff(cv::Mat &mask, cv::Point p) const;
};