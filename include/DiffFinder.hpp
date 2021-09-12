#pragma once

#include <iostream>
#include <opencv2/imgproc.hpp>

#include "DiffFinderAlgorithmBase.hpp"

class DiffFinder
{
public:
    static DiffFinder *getInstance(void);
    static void deleteInstance(void);

    Result isDifferent(const char *const path, double threshold);
    Result setReference(const char *const path);
    Result setAnother(const char *const path);

    void sayHi() { std::cout << "glad to see you use difference finder app\n"; }

private:
    DiffFinder() = default;
    ~DiffFinder() = default;

    template <class Algorithm>
    Result applyAlgorithm(Algorithm algorithm, double threshold);

    // private class member data
    static cv::Mat reference;
    static cv::Mat another;
    static DiffFinder *instance;
};