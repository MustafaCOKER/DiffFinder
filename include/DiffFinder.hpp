#pragma once

#include <iostream>
#include <opencv2/imgproc.hpp> 

#include "DiffFinderAlgorithmBase.hpp"

class DiffFinder {
public:
    static DiffFinder  *getInstance(void);
    static void         deleteInstance(void);

    Result  isDifferent(const char * const path, double threshold);
    Result  setReference(const char * const path);
    Result  setAnother(const char * const path);

    void sayHi() { std::cout << "glad to see you use difference finder app\n"; }

private:
    DiffFinder() = default;
    ~DiffFinder() = default;

    // private functions
    void    paintBackground(const cv::Mat& source, cv::Mat& painted) const;
    double  diffImage(const cv::Mat& im1, const cv::Mat& im2, cv::Mat& mask) const;
    double  findMaxBlobRatio(const cv::Mat& mask) const;
    int     applyThreshold2Diff(cv::Mat& mask, cv::Point p) const;
    
    // private class member data
    static cv::Mat      reference;
    static cv::Mat      another;
    static DiffFinder   *instance;
};