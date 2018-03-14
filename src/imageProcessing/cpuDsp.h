//
// Created by kobus on 2017/04/06.
//

#pragma once


#include <opencv2/core/mat.hpp>
#include "../util/threads.h"
#include "imageStabKalman.h"

class cpuDsp
{
public:
    static void executeTiPipeline(cv::Mat& frame,
                                  cv::Mat& originalFrame,
                                  cv::Mat& nnFrame,
                                  cv::Mat& biqFrame,
                                  cv::Mat& nediFrame,
                                  cv::Mat& bilinearFrame,
                                  TiDspParameters params);
    static void eZoom(cv::Mat &frame, TiDspParameters params);
private:
    static void edgeEnhance(cv::Mat& frame, TiDspParameters params);
    static void sharpen(cv::Mat& frame);
    static void moveFrame(const cv::Mat &frame, const cv::Mat &dest, TiDspParameters params);
    static void nediUpscale(const cv::Mat &frame, cv::Mat &dest);
};


