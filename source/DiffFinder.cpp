#include <iostream>
#include <opencv2/highgui.hpp>  // OpenCV window I/O

#include "DiffFinder.hpp"

cv::Mat DiffFinder::reference = cv::Mat();
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

Result DiffFinder::isDifferent(const char * const path, double threshold)
{
    cv::Mat another = cv::imread(path, cv::IMREAD_UNCHANGED);

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

    cv::Mat paintedBackgroundReference,
            paintedBackgroundAnother;
    
    cv::Size refSize =  reference.size();

    if (refSize != another.size())
    {
        std::cerr << "Images Are NOT Same Size\n";
        exit(1);
    }

    std::cout << "Both Images Loaded Succesfull\n";

    paintBackground(reference, paintedBackgroundReference);
    paintBackground(another, paintedBackgroundAnother);

    cv::Mat mask;
    diffImage(paintedBackgroundReference, paintedBackgroundAnother, mask);
    double maxDiffRatio = findMaxBlobRatio(mask);

#ifdef _COKER_DEBUG_
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", mask);
    cv::waitKey(0);

    std::cout << "maxDiffRatio : [" << maxDiffRatio << "]\n";
#endif

    return maxDiffRatio > threshold ? Result::Different : Result::Same;
}

Result DiffFinder::setReference(const char * const path)
{
    reference = cv::imread(path, cv::IMREAD_UNCHANGED);

    if (reference.empty())
    {
        std::cerr << "Reference Image Path Invalid, Exiting !\n";
        return Result::Image_CanNOT_Load;
    }

    return Result::Init;
}

Result DiffFinder::setAnother(const char * const path)
{
    cv::Mat im = cv::imread(path, cv::IMREAD_UNCHANGED);
    
    if (im.empty())
    {
        std::cerr << "Another Image Path Invalid, Exiting !\n";
        return Result::Image_CanNOT_Load;
    }

    return Result::Init;

}

int DiffFinder::applyThreshold2Diff(cv::Mat& mask, cv::Point p)
{
    cv::Mat temp = mask.clone();
    std::vector<cv::Point> pts;
    pts.push_back(p);
    int count = 0;

    cv::Size sizeofIm = mask.size();

    while(!pts.empty())
    {
        cv::Point t = pts.back();
        pts.pop_back();

        temp.at<unsigned char>(t.y, t.x) = 100;

        if (t.y>1 && temp.at<unsigned char>(t.y-1, t.x) == 255)
            pts.push_back(cv::Point(t.x, t.y-1));
        
        if (t.y>1 && t.x< sizeofIm.width-2 && temp.at<unsigned char>(t.y-1, t.x+1) == 255)
            pts.push_back(cv::Point(t.x+1, t.y-1));
        
        if (t.x< sizeofIm.width-2 && temp.at<unsigned char>(t.y, t.x+1) == 255)
            pts.push_back(cv::Point(t.x+1, t.y));
        
        if (t.y<sizeofIm.height-2 && t.x<sizeofIm.width-2 && temp.at<unsigned char>(t.y+1, t.x+1) == 255)
            pts.push_back(cv::Point(t.x+1, t.y+1));
        
        if (t.y<sizeofIm.height-2 && temp.at<unsigned char>(t.y+1, t.x) == 255)
            pts.push_back(cv::Point(t.x, t.y+1));
        
        if (t.y<sizeofIm.height-2 && t.x>1 && temp.at<unsigned char>(t.y+1, t.x-1) == 255)
            pts.push_back(cv::Point(t.x-1, t.y+1));

        if (t.x>1 && temp.at<unsigned char>(t.y, t.x-1) == 255)
            pts.push_back(cv::Point(t.x-1, t.y+1));

        if (t.x>1 && temp.at<unsigned char>(t.y, t.x-1) == 255)
            pts.push_back(cv::Point(t.x-1, t.y));

        if (t.x>1 && t.y>1 && temp.at<unsigned char>(t.y-1, t.x-1) == 255)
            pts.push_back(cv::Point(t.x-1, t.y-1));
        
        ++count;
    }

#ifdef _COKER_DEBUG_
    if (count>100)
    {
        std::cout << "count : [" << count << "]\n";
        
        cv::imshow("TempImage", temp);
        cv::waitKey(0);
    }
#endif

    mask = temp.clone();
    return count;
}

double DiffFinder::findMaxBlobRatio(const cv::Mat& mask)
{
    int maxDiffSize =0,
        tempSize =0;
    const int imSize = mask.size().width * mask.size().height;

    cv::Mat finalDif = mask.clone();

    for(int y=0; y<mask.rows; ++y)
    {
        for(int x=0; x<mask.cols; ++x)
        {   
            if (mask.at<unsigned char>(y,x) == 255)
            {
                tempSize = applyThreshold2Diff(finalDif, cv::Point(x, y));
                
                if (tempSize > maxDiffSize)
                    maxDiffSize = tempSize;
            }
        }
    }  

    // std::cout << "maxDiffSize " << maxDiffSize << "- imSize " << imSize << std::endl; 

    return (double) maxDiffSize / (double) imSize;
}

double DiffFinder::diffImage(const cv::Mat& im1, const cv::Mat& im2, cv::Mat& mask)
{
    cv::Mat diff;
    absdiff(im1, im2, diff);
    mask = cv::Mat::zeros(diff.rows, diff.cols, CV_8UC1);

    float threshold = 30.0f;
    float dist = 0;

    int sizeOfDiffPixels = 0;

    for(int y=0; y<diff.rows; ++y)
        for(int x=0; x<diff.cols; ++x)
        {
            cv::Vec3b pix = diff.at<cv::Vec3b>(y,x);

            dist = (pix[0]*pix[0] + pix[1]*pix[1] + pix[2]*pix[2]);
            dist = sqrt(dist);

            if(dist>threshold)
            {
                mask.at<unsigned char>(y,x) = 255;
                ++sizeOfDiffPixels;
            }
        }

    return (double) sizeOfDiffPixels / (double) (diff.rows*diff.cols);
}

void DiffFinder::paintBackground(const cv::Mat& source, cv::Mat& painted)
{
    if (source.empty())
    {
        std::cerr << __PRETTY_FUNCTION__ << " : " << " Image is Empty, Exiting !\n"; 
        exit(-1);
    }

    std::vector<cv::Mat> pngChannels(4);
    cv::split(source, pngChannels);

    pngChannels[0].setTo(cv::Scalar(255), pngChannels[3]==0);
    pngChannels[1].setTo(cv::Scalar(255), pngChannels[3]==0);
    pngChannels[2].setTo(cv::Scalar(255), pngChannels[3]==0);

    cv::merge(pngChannels, painted);
}