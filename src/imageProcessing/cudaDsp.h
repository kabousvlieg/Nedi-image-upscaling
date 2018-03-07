//
// Created by kobus on 2017/04/06.
//

#pragma once

//#define USE_CUDA true  //Only for the editor and eclipse, cmake actually provides this define

#include "../util/threads.h"
#if USE_CUDA
#include <opencv2/core/cuda.hpp>
#endif

class cudaDsp
{
public:
#if USE_CUDA
    void static executeTiPipeline(cv::Mat& frame, cv::Mat& originalFrame, TiDspParameters params, bool showComparisonWindows);
#endif
private:
#if USE_CUDA
    void static equalizeHistogram(cv::cuda::GpuMat& frame);
    void static frameAverage(cv::cuda::GpuMat& frame);
    void static edgeEnhance(cv::cuda::GpuMat& frame, TiDspParameters params);
    void static eZoom(cv::cuda::GpuMat &frame, TiDspParameters params);
    void static moveFrame(const cv::cuda::GpuMat &frame, const cv::cuda::GpuMat &dest, TiDspParameters params);
#endif
    std::string static type2str(int type); //TODO move to a opencv util class
};


