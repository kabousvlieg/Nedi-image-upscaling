//
// Created by kobus on 2017/04/06.
//

#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <list>
#include <deque>
#include <iomanip>
#include <cv.hpp>
#include "cpuDsp.h"
#include <armadillo>
//#include <Eigen/Dense>
//#include <Eigen/Sparse>
//#include <Eigen/IterativeLinearSolvers>
//#include <Eigen/SparseLU>

void printMat(cv::Mat image)
{
    std::cout << "Mat:" << std::endl;
    for (int i = 0; i < image.cols; i++)
    {
        std::cout << "  " << std::setfill('0') << std::setw(5) << i << "  ";
    }
    std::cout << std::endl;

    for (int i = 0; i < image.rows; i++)
    {
        std::cout << "  " << std::setfill('0') << std::setw(5) << i << "  ";
        for (int j = 0; j < image.cols; j++)
        {
            std::cout << (image.at<int>(i, j) & 0xFF) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void printMatFloat(cv::Mat image)
{
    std::cout << "Mat:" << std::endl;
    for (int i = 0; i < image.cols; i++)
    {
        std::cout << "  " << std::setfill('0') << std::setw(5) << i << "  ";
    }
    std::cout << std::endl;

    for (int i = 0; i < image.rows; i++)
    {
        std::cout << "  " << std::setfill('0') << std::setw(5) << i << "  ";
        for (int j = 0; j < image.cols; j++)
        {
            std::cout << image.at<double>(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void printArray(double array[1280][720])
{
    std::cout << "Mat:" << std::endl;
    for (int i = 0; i < 1280; i++)
    {
        std::cout << "  " << std::setfill('0') << std::setw(5) << i << "  ";
    }
    std::cout << std::endl;

    for (int i = 0; i < 720; i++)
    {
        std::cout << "  " << std::setfill('0') << std::setw(5) << i << "  ";
        for (int j = 0; j < 1280; j++)
        {
            std::cout << ((int)(array[i][j])) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void cpuDsp::executeTiPipeline(cv::Mat& frame,
                               cv::Mat& originalFrame,
                               cv::Mat& nnFrame,
                               cv::Mat& biqFrame,
                               cv::Mat& nediFrame,
                               TiDspParameters params)
{
    static bool showStabilisation = false;
    static int stabFrame = 0;
    cv::Mat temp1;
    cv::Mat temp2;

    cv::cvtColor(frame, frame, CV_BGR2GRAY);
    originalFrame = frame.clone();

    if (params.outputSelect == OutputSelect::Raw)
    {
        frame.copyTo(nnFrame);
        frame.copyTo(biqFrame);
        frame.copyTo(nediFrame);
        return;
    }

    //    if (params.eZoomOn)
//    {
//        eZoom(frame, params);
//        if (showComparisonWindows)
//        {
//            eZoom(originalFrame, params);
//        }
//    }
//    if (params.outputSelect == OutputSelect::Ezoom)
//        frame.copyTo(outputFrame);

    if (params.downScaleOn)
    {
        if (params.ezoom == Ezoom::x2)
        {
            cv::pyrDown(frame, frame, cv::Size(frame.cols / 2, frame.rows / 2));
        }
        else if (params.ezoom == Ezoom::x4)
        {
            cv::pyrDown(frame, frame, cv::Size(frame.cols / 2, frame.rows / 2));
            cv::pyrDown(frame, frame, cv::Size(frame.cols / 2, frame.rows / 2));
        }
        else if (params.ezoom == Ezoom::x8)
        {
            cv::pyrDown(frame, frame, cv::Size(frame.cols / 2, frame.rows / 2));
            cv::pyrDown(frame, frame, cv::Size(frame.cols / 2, frame.rows / 2));
            cv::pyrDown(frame, frame, cv::Size(frame.cols / 2, frame.rows / 2));
        }
    }
    if (params.outputSelect == OutputSelect::Downscale)
    {
        frame.copyTo(nnFrame);
        frame.copyTo(biqFrame);
        frame.copyTo(nediFrame);
        return;
    }

    if (params.upscaleOn)
    {
        if (params.ezoom == Ezoom::NoZoom)
        {
            frame.copyTo(nnFrame);
            frame.copyTo(biqFrame);
            frame.copyTo(nediFrame);
        }
        else if (params.ezoom == Ezoom::x2)
        {
            cv::resize(frame, nnFrame, cv::Size(originalFrame.cols, originalFrame.rows), 0, 0, cv::INTER_NEAREST);

            cv::resize(frame, biqFrame, cv::Size(originalFrame.cols, originalFrame.rows), 0, 0, cv::INTER_CUBIC);

            nediUpscale(frame, nediFrame);
        }
        else if (params.ezoom == Ezoom::x4)
        {
            cv::resize(frame, temp1, cv::Size(originalFrame.cols, originalFrame.rows), 0, 0, cv::INTER_NEAREST);
            cv::resize(temp1, nnFrame, cv::Size(originalFrame.cols, originalFrame.rows), 0, 0, cv::INTER_NEAREST);

            cv::resize(frame, temp1, cv::Size(originalFrame.cols, originalFrame.rows), 0, 0, cv::INTER_CUBIC);
            cv::resize(temp1, biqFrame, cv::Size(originalFrame.cols, originalFrame.rows), 0, 0, cv::INTER_CUBIC);

            nediUpscale(frame, temp1);
            nediUpscale(temp1, nediFrame);
        }
        else if (params.ezoom == Ezoom::x8)
        {
            cv::resize(frame, temp1, cv::Size(originalFrame.cols, originalFrame.rows), 0, 0, cv::INTER_NEAREST);
            cv::resize(temp1, temp2, cv::Size(originalFrame.cols, originalFrame.rows), 0, 0, cv::INTER_NEAREST);
            cv::resize(temp2, nnFrame, cv::Size(originalFrame.cols, originalFrame.rows), 0, 0, cv::INTER_NEAREST);

            cv::resize(frame, temp1, cv::Size(originalFrame.cols, originalFrame.rows), 0, 0, cv::INTER_CUBIC);
            cv::resize(temp1, temp2, cv::Size(originalFrame.cols, originalFrame.rows), 0, 0, cv::INTER_CUBIC);
            cv::resize(temp2, biqFrame, cv::Size(originalFrame.cols, originalFrame.rows), 0, 0, cv::INTER_CUBIC);

            nediUpscale(frame, temp1);
            nediUpscale(temp1, temp2);
            nediUpscale(temp2, nediFrame);
        }
    }
    else
    {
        frame.copyTo(nnFrame);
        frame.copyTo(biqFrame);
        frame.copyTo(nediFrame);
    }

    if (params.outputSelect == OutputSelect::Upscale)
        return;

    if (params.edgeEnhancementOn)
    {
        edgeEnhance(originalFrame, params);
        edgeEnhance(nnFrame, params);
        edgeEnhance(biqFrame, params);
        edgeEnhance(nediFrame, params);
    }
    if (params.outputSelect == OutputSelect::EdgeEnhance)
        return;


    return;
}

void cpuDsp::edgeEnhance(cv::Mat& frame, TiDspParameters params)
{
    switch (params.edgeEnhanceType)
    {
        case EdgeEnhanceType::Sharpen:
        {
            cv::Mat kernel = (cv::Mat_<float>(3,3) <<
                     0,-1, 0,
                    -1, 5,-1,
                     0,-1, 0);

            filter2D(frame, frame, CV_32F, kernel);
        }

        case EdgeEnhanceType::Gaussian:
        {
            cv::Mat kernel = (cv::Mat_<float>(5,5) <<
                    -1,-1,-1,-1,-1,
                    -1, 2, 2, 2,-1,
                    -1, 2, 8, 2,-1,
                    -1, 2, 2, 2,-1,
                    -1,-1,-1,-1,-1) / 8.0;

            filter2D(frame, frame, CV_32F, kernel);
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

            filter2D(frame, frame, CV_32F, kernel);
            break;
        }

        default:
        {
            break;
        }
    }
    frame.convertTo(frame, CV_8U);
    convertScaleAbs(frame, frame);
}

void cpuDsp::eZoom(cv::Mat &frame, TiDspParameters params)
{
    cv::Mat dest;
    switch (params.ezoom)
    {
        case Ezoom::NoZoom:
            break;
        case Ezoom::x2:
            cv::pyrUp(frame, dest, cv::Size(frame.cols * 2, frame.rows * 2));
            moveFrame(frame, dest, params);
            break;
        case Ezoom::x4:
            cv::pyrUp(frame, dest, cv::Size(frame.cols * 2, frame.rows * 2));
            cv::pyrUp(dest, dest, cv::Size(dest.cols * 2, dest.rows * 2));
            moveFrame(frame, dest, params);
            break;
        case Ezoom::x8:
            cv::pyrUp(frame, dest, cv::Size(frame.cols * 2, frame.rows * 2));
            cv::pyrUp(dest, dest, cv::Size(dest.cols * 2, dest.rows * 2));
            cv::pyrUp(dest, dest, cv::Size(dest.cols * 2, dest.rows * 2));
            moveFrame(frame, dest, params);
            break;
    }

}

void cpuDsp::moveFrame(const cv::Mat &frame, const cv::Mat &dest, TiDspParameters params)
{
    double availableXPixels, availableYPixels;
    double xMove, yMove;

    availableXPixels = dest.cols - frame.cols;
    availableYPixels = dest.rows - frame.rows;
    xMove = params.horisontalOffset / 100.0;
    yMove = params.verticalOffset / 100.0;
    dest(cv::Rect(xMove * availableXPixels, yMove * availableYPixels, frame.cols, frame.rows)).copyTo(frame);
}

void cpuDsp::sharpen(cv::Mat& frame)
{
    cv::Mat blurred; double sigma = 1, threshold = 5, amount = 1;
    GaussianBlur(frame, blurred, cv::Size(), sigma, sigma);
    cv::Mat lowContrastMask = cv::abs(frame - blurred) < threshold;
    cv::Mat sharpened = frame*(1+amount) + blurred*(-amount);
    frame.copyTo(frame, lowContrastMask);
}

void cpuDsp::nediUpscale(const cv::Mat &frame, cv::Mat &dest) {
    const int upscaleFactor = 2;
    dest = cv::Mat::zeros(cv::Size(frame.cols * upscaleFactor, frame.rows * upscaleFactor), CV_8UC1);

    for (int y = 0; y < frame.rows; y++)
    {
        for (int x = 0; x < frame.cols; x++)
        {
            dest.ptr<unsigned char>(y * upscaleFactor)[x * upscaleFactor] = frame.ptr<unsigned char>(y)[x];
        }
    }

    arma::mat Y = arma::zeros(25, 1);
    arma::mat C = arma::zeros(25, 4);
    arma::mat CtC;
    arma::mat Cty;
    arma::mat Cinv;
    arma::mat a;
    int temp;
    //int m = 5; //todo review size of filter

    //Solve the "b" pixels
    //todo edge conditions not handled yet
    for (int i = 4; i < (dest.rows - 4) / 2; i++)
    {
        for (int j = 4; j < (dest.cols - 4) / 2; j++)
        {
            temp = 0;
            for (int ii = i - 2; ii <= i + 2; ii++)
            {
                for (int jj = j - 2; jj <= j + 2; jj++)
                {
                    Y(temp, 0) = (dest.ptr<unsigned char>(2*ii)[2*jj]);

                    C(temp, 0) = (dest.ptr<unsigned char>(2*ii - 2)[2*jj - 2]);
                    C(temp, 1) = (dest.ptr<unsigned char>(2*ii + 2)[2*jj - 2]);
                    C(temp, 2) = (dest.ptr<unsigned char>(2*ii + 2)[2*jj + 2]);
                    C(temp, 3) = (dest.ptr<unsigned char>(2*ii - 2)[2*jj + 2]);
                    temp++;
                }
            }

            Cty = C.t() * Y;
            CtC = C.t() * C;
            if (arma::solve(a, Cty, CtC))
            {
                (dest.ptr<unsigned char>(2*i + 1)[2*j + 1]) = (unsigned char) (a(0, 0) / 4 * (dest.ptr<unsigned char>(2*i)[2*j]) + \
                a(0, 1) / 4 * (dest.ptr<unsigned char>(2*i + 2)[2*j]) + \
                a(0, 2) / 4 * (dest.ptr<unsigned char>(2*i + 2)[2*j + 2]) + \
                a(0, 3) / 4  * (dest.ptr<unsigned char>(2*i)[2*j + 2]));
            }
            else
            {
                (dest.ptr<unsigned char>(2*i + 1)[2*j + 1]) = 0;//(unsigned char) (dest.ptr<unsigned char>(y - 1)[x - 1]);
            }
        }
    }

    for (int i = 4; i < (dest.rows - 4) / 2; i++)
    {
        for (int j = 4; j < (dest.cols - 4) / 2; j++)
        {
            temp = 0;
            for (int ii = i - 2; ii <= i + 2; ii++)
            {
                for (int jj = j - 2; jj <= j + 2; jj++)
                {
                    Y(temp, 0) = (dest.ptr<unsigned char>(2*ii + 1)[2*jj - 1]);

                    C(temp, 0) = (dest.ptr<unsigned char>(2*ii - 1)[2*jj - 1]);
                    C(temp, 1) = (dest.ptr<unsigned char>(2*ii + 1)[2*jj - 3]);
                    C(temp, 2) = (dest.ptr<unsigned char>(2*ii + 3)[2*jj - 1]);
                    C(temp, 3) = (dest.ptr<unsigned char>(2*ii + 1)[2*jj + 1]);
                    temp++;
                }
            }

            Cty = C.t() * Y;
            CtC = C.t() * C;
            if (arma::solve(a, Cty, CtC))
            {
                (dest.ptr<unsigned char>(2*i + 1)[2*j]) = (unsigned char) (a(0, 0) / 4 * (dest.ptr<unsigned char>(2*i)[2*j]) + \
                a(0, 1) / 4 * (dest.ptr<unsigned char>(2*i + 1)[2*j - 1]) + \
                a(0, 2) / 4 * (dest.ptr<unsigned char>(2*i + 2)[2*j]) + \
                a(0, 3) / 4  * (dest.ptr<unsigned char>(2*i + 1)[2*j + 1]));
            }
            else
            {
                (dest.ptr<unsigned char>(2*i + 1)[2*j]) = 0;//(unsigned char) (dest.ptr<unsigned char>(y - 1)[x - 1]);
            }
        }
    }


    for (int i = 4; i < (dest.rows - 4) / 2; i++)
    {
        for (int j = 4; j < (dest.cols - 4) / 2; j++)
        {
            temp = 0;
            for (int ii = i - 2; ii <= i + 2; ii++)
            {
                for (int jj = j - 2; jj <= j + 2; jj++)
                {
                    Y(temp, 0) = (dest.ptr<unsigned char>(2*ii + 1)[2*jj - 1]);

                    C(temp, 0) = (dest.ptr<unsigned char>(2*ii - 1)[2*jj - 1]);
                    C(temp, 1) = (dest.ptr<unsigned char>(2*ii + 1)[2*jj - 3]);
                    C(temp, 2) = (dest.ptr<unsigned char>(2*ii + 3)[2*jj - 1]);
                    C(temp, 3) = (dest.ptr<unsigned char>(2*ii + 1)[2*jj + 1]);
                    temp++;
                }
            }

            Cty = C.t() * Y;
            CtC = C.t() * C;
            if (arma::solve(a, Cty, CtC))
            {
                (dest.ptr<unsigned char>(2*i)[2*j + 1]) = (unsigned char) (a(0, 0) / 4 * (dest.ptr<unsigned char>(2*i - 1)[2*j + 1]) + \
                a(0, 1) / 4 * (dest.ptr<unsigned char>(2*i)[2*j]) + \
                a(0, 2) / 4 * (dest.ptr<unsigned char>(2*i + 1)[2*j + 1]) + \
                a(0, 3) / 4  * (dest.ptr<unsigned char>(2*i)[2*j + 2]));
            }
            else
            {
                (dest.ptr<unsigned char>(2*i)[2*j + 1]) = 0;//(unsigned char) (dest.ptr<unsigned char>(y - 1)[x - 1]);
            }
        }
    }
}

