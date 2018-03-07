//
// Created by kobus on 2016/01/28.
//

#pragma once

#include <string>
#include <iostream>
#include <boost/lockfree/spsc_queue.hpp>
#include <termios.h>
#include <atomic>
#include "../util/threads.h"

using namespace std; //gpu requires for compilation
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>



class TiDsp_thread : public threads
{
public:
    TiDsp_thread(boost::lockfree::spsc_queue<FromTiDspThread> &produce,
                 boost::lockfree::spsc_queue<ToTiDspThread> &consume);
    void run(void) override;
private:
    boost::lockfree::spsc_queue<FromTiDspThread> &produceQ;
    boost::lockfree::spsc_queue<ToTiDspThread> &consumeQ;
    TiDspParameters params;
    TiDspInfo info;

    void updateFrameRate();
    void displayResults(cv::Mat& originalFrame, cv::Mat& nnFrame, cv::Mat& biqFrame, cv::Mat& nediFrame);
    void handleGuiCommand(ToTiDspThread TiMsg);
    void printMat(cv::Mat& mat);
    void calculateHistogram(cv::Mat& frame);

    void processFrame(cv::Mat &frame, cv::Mat &originalFrame, cv::Mat &nnFrame, cv::Mat &biqFrame, cv::Mat &nediFrame);
};

