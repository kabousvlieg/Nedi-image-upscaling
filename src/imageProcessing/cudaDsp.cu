//
// Created by kobus on 2017/04/06.
//

#include "cudaDsp.h"
#include "cpuDsp.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

#if USE_CUDA
#include <cuda.h>
#include <opencv2/cudacodec.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudabgsegm.hpp>
#include <opencv2/cudalegacy.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudafeatures2d.hpp>
#include <opencv2/cudaoptflow.hpp>
#include <opencv2/cudastereo.hpp>
#include <opencv2/cudawarping.hpp>
#endif


#if USE_CUDA

__global__ void convolutionGPU(
		std::vector<cv::cuda::GpuMat> inputChannels,
		std::vector<cv::cuda::GpuMat> outputChannels,
		cv::cuda::GpuMat kernel);

void cudaDsp::executeTiPipeline(cv::Mat& frame, cv::Mat& originalFrame, TiDspParameters params, bool showComparisonWindows)
{
    static bool runOnce = false;
    if (!runOnce)
    {
		int runTimeVersion;
		cudaRuntimeGetVersion(&runTimeVersion);
		int driverVersion;
		cudaDriverGetVersion(&driverVersion);

		std::cout << "Cuda runtime version : " << runTimeVersion << std::endl;
		std::cout << "Cuda api version : " << driverVersion << std::endl;

		runOnce = true;
    }

    cv::cuda::GpuMat gpuFrame;
    gpuFrame.upload(frame);
    //convertScaleAbs(frame, frame);
    if (showComparisonWindows)
    {
        originalFrame = frame.clone();
    }

    //Image processing pipeline
    if (params.frameAverageOn)
        frameAverage(gpuFrame);
//    if (params.outputSelect == OutputSelect::FrameAverage)
//        gpuFrame.download(outputFrame);

//    if (params.outputSelect == OutputSelect::Nuc)
//        gpuFrame.download(outputFrame);

//    if (params.outputSelect == OutputSelect::TemporalFilter)
//        gpuFrame.download(outputFrame);

//    if (params.outputSelect == OutputSelect::SpatialFilter)
//        gpuFrame.download(outputFrame);

    if (params.edgeEnhanceOn)
    	edgeEnhance(gpuFrame, params);
//    if (params.outputSelect == OutputSelect::EdgeEnhance)
//        gpuFrame.download(outputFrame);

//    if (params.outputSelect == OutputSelect::Stab)
//        gpuFrame.download(outputFrame);

    if (params.eZoomOn)
    {
        eZoom(gpuFrame, params);
        if (showComparisonWindows)
        {
            cpuDsp::eZoom(originalFrame, params);
        }
    }
//    if (params.outputSelect == OutputSelect::Ezoom)
//        frame.copyTo(outputFrame);

    if (params.histogramStretchOn)
        equalizeHistogram(gpuFrame);
//    if (params.outputSelect == OutputSelect::HistogramStretch)
//        gpuFrame.download(outputFrame);

//    if (params.outputSelect == OutputSelect::Colorize)
//        gpuFrame.download(outputFrame);

    //sharpen(frame);
    gpuFrame.download(frame);
//	return;
}

void cudaDsp::equalizeHistogram(cv::cuda::GpuMat& frame)
{
    std::vector<cv::cuda::GpuMat> channels;
    cv::cuda::GpuMat img_hist_equalized;

    cv::cuda::cvtColor(frame, img_hist_equalized, CV_BGR2YCrCb); //change the color image from BGR to YCrCb format
    cv::cuda::split(img_hist_equalized,channels); //split the image into channels
    cv::cuda::equalizeHist(channels[0], channels[0]); //equalize histogram on the 1st channel (Y)
    cv::cuda::merge(channels,img_hist_equalized); //merge 3 channels including the modified 1st channel into one image
    cv::cuda::cvtColor(img_hist_equalized, frame, CV_YCrCb2BGR); //change the color image from YCrCb to BGR format (to display image properly)

}

void cudaDsp::frameAverage(cv::cuda::GpuMat& frame)
{
    static cv::cuda::GpuMat previousFrame;

    if (previousFrame.empty())
    {
        previousFrame = frame;
        return;
    }

    cv::cuda::GpuMat avg, diff;
    avg.create(frame.cols, frame.rows, CV_32FC3);
    diff.create(frame.cols, frame.rows, CV_32FC1);
    cv::cuda::addWeighted(frame, 0.5, previousFrame, 0.5, 0, avg);

    frame.copyTo(previousFrame);
}

void cudaDsp::edgeEnhance(cv::cuda::GpuMat& frame, TiDspParameters params)
{
    switch (params.edgeEnhanceType)
	{
		case EdgeEnhanceType::Sharpen:
		{
            cv::Mat kernel = (cv::Mat_<float>(3,3) <<
                     0,-1, 0,
                    -1, 5,-1,
                     0,-1, 0);

			cv::cuda::GpuMat alpha(frame.rows, frame.cols, CV_32FC1, double(0));
			cv::cuda::GpuMat frameFC3;
			frame.convertTo(frameFC3, CV_32FC3);

			std::vector<cv::cuda::GpuMat> channels;

			cv::cuda::split(frameFC3, channels); //split the image into channels
			channels.push_back(alpha);

			cv::cuda::GpuMat frameWithAlpha;
			cv::cuda::merge(channels, frameWithAlpha);

			cv::Ptr<cv::cuda::Filter> filter2D = cv::cuda::createLinearFilter(CV_32FC4, -1, kernel);
			filter2D->apply(frameWithAlpha, frameWithAlpha);

			cv::cuda::split(frameWithAlpha, channels); //split the image into channels
			channels.pop_back();

			cv::cuda::merge(channels, frameFC3);

			frameFC3.convertTo(frame, CV_8UC3);
			break;
		}

		case EdgeEnhanceType::Gaussian:
		{
			cv::Mat kernel = (cv::Mat_<float>(5,5) <<
							 -1,-1,-1,-1,-1,
							 -1, 2, 2, 2,-1,
							 -1, 2, 8, 2,-1,
							 -1, 2, 2, 2,-1,
							 -1,-1,-1,-1,-1) / 8.0;

			cv::cuda::GpuMat alpha(frame.rows, frame.cols, CV_32FC1, double(0));
			cv::cuda::GpuMat frameFC3;
			frame.convertTo(frameFC3, CV_32FC3);

			std::vector<cv::cuda::GpuMat> channels;

			cv::cuda::split(frameFC3, channels); //split the image into channels
			channels.push_back(alpha);

			cv::cuda::GpuMat frameWithAlpha;
			cv::cuda::merge(channels, frameWithAlpha);

			cv::Ptr<cv::cuda::Filter> filter2D = cv::cuda::createLinearFilter(CV_32FC4, -1, kernel);
			filter2D->apply(frameWithAlpha, frameWithAlpha);

			cv::cuda::split(frameWithAlpha, channels); //split the image into channels
			channels.pop_back();

			cv::cuda::merge(channels, frameFC3);

			frameFC3.convertTo(frame, CV_8UC3);

			break;
		}

		case EdgeEnhanceType::Unsharp_mask:
		{
            cv::Mat kernel = (cv::Mat_<float>(5,5) <<
                     1, 4,   6, 4, 1,
                     4,16,  24,16, 4,
                     6,24,-476,24, 6,
                     4,16,  24,16, 4,
                     1, 4,   6, 4, 1) / -256.0;

			cv::cuda::GpuMat alpha(frame.rows, frame.cols, CV_32FC1, double(0));
			cv::cuda::GpuMat frameFC3;
			frame.convertTo(frameFC3, CV_32FC3);

			std::vector<cv::cuda::GpuMat> channels;

			cv::cuda::split(frameFC3, channels); //split the image into channels
			channels.push_back(alpha);

			cv::cuda::GpuMat frameWithAlpha;
			cv::cuda::merge(channels, frameWithAlpha);

			cv::Ptr<cv::cuda::Filter> filter2D = cv::cuda::createLinearFilter(CV_32FC4, -1, kernel);
			filter2D->apply(frameWithAlpha, frameWithAlpha);

			cv::cuda::split(frameWithAlpha, channels); //split the image into channels
			channels.pop_back();

			cv::cuda::merge(channels, frameFC3);

			frameFC3.convertTo(frame, CV_8UC3);
            break;
		}

		default:
		{
			break;
		}
	}
    //frame.convertTo(frame, CV_8U);
    //cv::cuda::convertScaleAbs(frame, frame);
}



//__global__ void convolutionGPU(
//		std::vector<cv::cuda::GpuMat> inputChannels,
//		std::vector<cv::cuda::GpuMat> outputChannels,
//		cv::cuda::GpuMat kernel)
//{
//	int KERNEL_W = (2 * KERNEL_RADIUS + 1);
//    //////////////////////////////////////////////////////////////////////
//    // most slowest way to compute convolution
//    //////////////////////////////////////////////////////////////////////
//
//    // global mem address for this thread
//    const int gLoc = threadIdx.x +
//                    blockIdx.x * blockDim.x +
//                    threadIdx.y * dataW +
//                    blockIdx.y * blockDim.y * dataW;
//
//    float sum = 0;
//    float value = 0;
//
//    for (int i = -KERNEL_RADIUS; i <= KERNEL_RADIUS; i++) // row wise
//        for (int j = -KERNEL_RADIUS; j <= KERNEL_RADIUS; j++) // col wise
//        {
//            // check row first
//            if (blockIdx.x == 0 && (threadIdx.x + i) < 0) // left apron
//                value = 0;
//            else if ( blockIdx.x == (gridDim.x - 1) &&
//                        (threadIdx.x + i) > blockDim.x-1 ) // right apron
//                value = 0;
//            else
//            {
//                // check col next
//                if (blockIdx.y == 0 && (threadIdx.y + j) < 0) // top apron
//                    value = 0;
//                else if ( blockIdx.y == (gridDim.y - 1) &&
//                            (threadIdx.y + j) > blockDim.y-1 ) // bottom apron
//                    value = 0;
//                else // safe case
//                    value = d_Data[gLoc + i + j * dataW];
//            }
//            sum += value * d_Kernel[KERNEL_RADIUS + i] * d_Kernel[KERNEL_RADIUS + j];
//        }
//        d_Result[gLoc] = sum;
//}


void cudaDsp::eZoom(cv::cuda::GpuMat &frame, TiDspParameters params)
{
	/*
    cv::cuda::GpuMat dest;
    switch (params.ezoom)
    {
        case Ezoom::NoZoom:
            break;
        case Ezoom::x2:
            cv::cuda::pyrUp(frame, dest, cv::cuda::Size(frame.cols * 2, frame.rows * 2));
            moveFrame(frame, dest, params);
            break;
        case Ezoom::x4:
            cv::cuda::pyrUp(frame, dest, cv::cuda::Size(frame.cols * 2, frame.rows * 2));
            cv::cuda::pyrUp(dest, dest, cv::cuda::Size(dest.cols * 2, dest.rows * 2));
            moveFrame(frame, dest, params);
            break;
        case Ezoom::x8:
            cv::cuda::pyrUp(frame, dest, cv::cuda::Size(frame.cols * 2, frame.rows * 2));
            cv::cuda::pyrUp(dest, dest, cv::cuda::Size(dest.cols * 2, dest.rows * 2));
            cv::cuda::pyrUp(dest, dest, cv::cuda::Size(dest.cols * 2, dest.rows * 2));
            moveFrame(frame, dest, params);
            break;
    }
    */
}

void cudaDsp::moveFrame(const cv::cuda::GpuMat &frame, const cv::cuda::GpuMat &dest, TiDspParameters params)
{
	/*
    double availableXPixels, availableYPixels;
    double xMove, yMove;

    availableXPixels = dest.cols - frame.cols;
    availableYPixels = dest.rows - frame.rows;
    xMove = params.horisontalOffset / 100.0;
    yMove = params.verticalOffset / 100.0;
    dest(cv::cuda::Rect(xMove * availableXPixels, yMove * availableYPixels, frame.cols, frame.rows)).copyTo(frame);
    */
}
#endif

std::string cudaDsp::type2str(int type)
{
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}
