//
// Created by kobus on 2017/09/21.
//

#pragma once

#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>

class Tracker {
public:
    Tracker();
    ~Tracker() {};

    void processImages(cv::Mat img[], int numFrames);
    void processImage(cv::Mat img);
    void start(cv::Mat img);

private:
    cv::Mat_<float>     rigidTransform;
    std::vector<cv::Point2f> trackedFeatures;
    cv::Mat             trackedImage;

    void processThisImage(cv::Mat &img);
};
