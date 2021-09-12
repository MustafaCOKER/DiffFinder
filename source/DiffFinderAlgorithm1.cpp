#include <iostream>
#include "DiffFinderAlgorithm1.hpp"

Result DiffFinderAlgorithm1::execute(const cv::Mat &reference,
                                     const cv::Mat &another,
                                     double threshold)
{
    cv::Mat paintedBackgroundReference,
        paintedBackgroundAnother;

    paintBackground(reference, paintedBackgroundReference);
    paintBackground(another, paintedBackgroundAnother);

    cv::Mat mask;
    diffImage(paintedBackgroundReference, paintedBackgroundAnother, mask);
    double maxDiffRatio = findMaxBlobRatio(mask);

#ifdef _COKER_DEBUG_
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", mask);
    cv::waitKey(0);

    std::cout << "maxDiffRatio : [" << maxDiffRatio << "]\n";
#endif

    return maxDiffRatio > threshold ? Result::Different : Result::Same;
}

int DiffFinderAlgorithm1::applyThreshold2Diff(cv::Mat &mask, cv::Point p) const
{
    cv::Mat temp = mask.clone();
    std::vector<cv::Point> pts;
    pts.push_back(p);
    int count = 0;

    cv::Size sizeofIm = mask.size();

    while (!pts.empty())
    {
        cv::Point t = pts.back();
        pts.pop_back();

        temp.at<unsigned char>(t.y, t.x) = 100;

        if (t.y > 1 && temp.at<unsigned char>(t.y - 1, t.x) == 255)
            pts.push_back(cv::Point(t.x, t.y - 1));

        if (t.y > 1 && t.x < sizeofIm.width - 2 && temp.at<unsigned char>(t.y - 1, t.x + 1) == 255)
            pts.push_back(cv::Point(t.x + 1, t.y - 1));

        if (t.x < sizeofIm.width - 2 && temp.at<unsigned char>(t.y, t.x + 1) == 255)
            pts.push_back(cv::Point(t.x + 1, t.y));

        if (t.y < sizeofIm.height - 2 && t.x < sizeofIm.width - 2 && temp.at<unsigned char>(t.y + 1, t.x + 1) == 255)
            pts.push_back(cv::Point(t.x + 1, t.y + 1));

        if (t.y < sizeofIm.height - 2 && temp.at<unsigned char>(t.y + 1, t.x) == 255)
            pts.push_back(cv::Point(t.x, t.y + 1));

        if (t.y < sizeofIm.height - 2 && t.x > 1 && temp.at<unsigned char>(t.y + 1, t.x - 1) == 255)
            pts.push_back(cv::Point(t.x - 1, t.y + 1));

        if (t.x > 1 && temp.at<unsigned char>(t.y, t.x - 1) == 255)
            pts.push_back(cv::Point(t.x - 1, t.y + 1));

        if (t.x > 1 && temp.at<unsigned char>(t.y, t.x - 1) == 255)
            pts.push_back(cv::Point(t.x - 1, t.y));

        if (t.x > 1 && t.y > 1 && temp.at<unsigned char>(t.y - 1, t.x - 1) == 255)
            pts.push_back(cv::Point(t.x - 1, t.y - 1));

        ++count;
    }

#ifdef _COKER_DEBUG_
    if (count > 100)
    {
        std::cout << "count : [" << count << "]\n";

        cv::imshow("TempImage", temp);
        cv::waitKey(0);
    }
#endif

    mask = temp.clone();
    return count;
}

double DiffFinderAlgorithm1::findMaxBlobRatio(const cv::Mat &mask) const
{
    int maxDiffSize = 0,
        tempSize = 0;
    const int imSize = mask.size().width * mask.size().height;

    cv::Mat finalDif = mask.clone();

    for (int y = 0; y < mask.rows; ++y)
    {
        for (int x = 0; x < mask.cols; ++x)
        {
            if (mask.at<unsigned char>(y, x) == 255)
            {
                tempSize = applyThreshold2Diff(finalDif, cv::Point(x, y));

                if (tempSize > maxDiffSize)
                    maxDiffSize = tempSize;
            }
        }
    }

    return (double)maxDiffSize / (double)imSize;
}

double DiffFinderAlgorithm1::diffImage(const cv::Mat &im1, const cv::Mat &im2, cv::Mat &mask) const
{
    cv::Mat diff;
    absdiff(im1, im2, diff);
    mask = cv::Mat::zeros(diff.rows, diff.cols, CV_8UC1);

    float threshold = 30.0f;
    float dist = 0;

    int sizeOfDiffPixels = 0;

    for (int y = 0; y < diff.rows; ++y)
        for (int x = 0; x < diff.cols; ++x)
        {
            cv::Vec3b pix = diff.at<cv::Vec3b>(y, x);

            dist = (pix[0] * pix[0] + pix[1] * pix[1] + pix[2] * pix[2]);
            dist = sqrt(dist);

            if (dist > threshold)
            {
                mask.at<unsigned char>(y, x) = 255;
                ++sizeOfDiffPixels;
            }
        }

    return (double)sizeOfDiffPixels / (double)(diff.rows * diff.cols);
}

void DiffFinderAlgorithm1::paintBackground(const cv::Mat &source, cv::Mat &painted) const
{
    if (source.empty())
    {
        std::cerr << __PRETTY_FUNCTION__ << " : "
                  << " Image is Empty, Exiting !\n";
        exit(-1);
    }

    std::vector<cv::Mat> pngChannels(4);
    cv::split(source, pngChannels);

    pngChannels[0].setTo(cv::Scalar(255), pngChannels[3] == 0);
    pngChannels[1].setTo(cv::Scalar(255), pngChannels[3] == 0);
    pngChannels[2].setTo(cv::Scalar(255), pngChannels[3] == 0);

    cv::merge(pngChannels, painted);
}