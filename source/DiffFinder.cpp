#include <iostream>
#include <opencv2/highgui.hpp> // OpenCV window I/O

#include "DiffFinder.hpp"
#include "DiffFinderAlgorithm1.hpp"
#include "DiffFinderAlgorithm2.hpp"

cv::Mat DiffFinder::reference = cv::Mat();
cv::Mat DiffFinder::another = cv::Mat();
DiffFinder *DiffFinder::instance = nullptr;

DiffFinder *DiffFinder::getInstance(void)
{
    if (instance == nullptr)
    {
        instance = new DiffFinder();
    }

    return instance;
}

void DiffFinder::deleteInstance(void)
{
    if (instance != nullptr)
    {
        delete instance;
    }

    instance = nullptr;
}

template <class Algorithm>
Result DiffFinder::applyAlgorithm(Algorithm algorithm, double threshold)
{
    return algorithm.execute(reference, another, threshold);
}

Result DiffFinder::isDifferent(const char *const path, double threshold)
{
    if (path != nullptr)
    {
        if (this->setAnother(path) == Result::Image_CanNOT_Load)
            return Result::Image_CanNOT_Load;
    }

    if (reference.empty())
    {
        std::cerr << "Reference Image is Empty, Exciting !\n";
        return Result::Image_CanNOT_Load;
    }

    if (another.empty())
    {
        std::cerr << "Another Image is Empty, Exciting !\n";
        return Result::Image_CanNOT_Load;
    }

    cv::Size refSize = reference.size();

    if (refSize != another.size())
    {
        std::cerr << "Images Are NOT Same Size\n";
        exit(1);
    }

    std::cout << "Both Images Loaded Succesfull\n";

    return applyAlgorithm(DiffFinderAlgorithm2(), threshold);
}

Result DiffFinder::setReference(const char *const path)
{
    reference = cv::imread(path, cv::IMREAD_UNCHANGED);

    if (reference.empty())
    {
        std::cerr << "Reference Image Path Invalid, Exiting !\n";
        return Result::Image_CanNOT_Load;
    }

    return Result::Init;
}

Result DiffFinder::setAnother(const char *const path)
{
    another = cv::imread(path, cv::IMREAD_UNCHANGED);

    if (another.empty())
    {
        std::cerr << "Another Image Path Invalid, Exiting !\n";
        return Result::Image_CanNOT_Load;
    }

    return Result::Init;
}