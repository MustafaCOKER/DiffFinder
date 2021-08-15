#pragma once

#include <iostream>
#include <opencv2/imgproc.hpp> 

enum class Result {
    Same = 0,
    Different
};

class DiffFinder {
public:
    static DiffFinder  *getInstance(void);
    static void         deleteInstance(void);

    Result  isDifferent(const char * const path, double threshold);
    void    setReference(const char * const path);

    void sayHi() { std::cout << "glad to see you use difference finder app\n"; }

private:
    DiffFinder() = default;
    ~DiffFinder() = default;

    // private functions
    void    paintBackground(const cv::Mat& source, cv::Mat& painted);
    double  diffImage(const cv::Mat& another, const cv::Mat& im2, cv::Mat& mask);
    double  findMaxBlobRatio(const cv::Mat& mask);
    int     applyThreshold2Diff(cv::Mat& mask, cv::Point p);

    // private class member data
    static cv::Mat      reference;
    static DiffFinder   *instance;
};