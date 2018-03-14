/*
 * Thread.h
 *
 *  Created on: 12 Jan 2016
 *      Author: kobus
 */

#pragma once

#include <string>
#include <iostream>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/shared_ptr.hpp>

enum class Threads
{
    Gui_Thread,
    Ti_Thread_SignalProcessing,
    Ti_Thread_CameraBuffer
};

enum class OutputSelect
{
    Raw,
    Downscale,
    EdgeEnhance,
    Upscale,
    Ezoom
};

enum class EdgeEnhanceType
{
    Gaussian,
    Sharpen,
    Unsharp_mask
};

enum class Ezoom
{
    NoZoom,
    x2,
    x4,
    x8
};

struct TiDspParameters
{
    OutputSelect outputSelect;

    bool downScaleOn;
    bool edgeEnhancementOn;
    bool upscaleOn;
    bool eZoomOn;

    //Frame averager parameters
    int frameAveragerThreshold;

    //EdgeEnhancement parameters
    EdgeEnhanceType edgeEnhanceType;

    //Ezoom
    Ezoom  ezoom;
    int verticalOffset;
    int horisontalOffset;

    //Histogram stretching parameters

    TiDspParameters() :
            outputSelect{OutputSelect::Ezoom},

            downScaleOn{true},
            edgeEnhancementOn{false},
            upscaleOn{true},
            eZoomOn{true},

            //EdgeEnhancement parameters
            edgeEnhanceType{EdgeEnhanceType::Gaussian},

            //Ezoom
            ezoom{Ezoom::NoZoom},
            verticalOffset{50},
            horisontalOffset{50}
    {

    }
};

struct TiCameraParameters
{
    int placeholder;

    TiCameraParameters() :
            placeholder{0}
    {

    }
};

struct TiCameraInfo
{
    int horisontalPixels;
    int verticalPixels;
    int colourSpace;
    int bitDepth;
    int frameRate;

    TiCameraInfo() :
            horisontalPixels{0},
            verticalPixels{0},
            colourSpace{0},
            bitDepth{0},
            frameRate{0}
    {

    }
};

struct TiDspInfo
{
    int pipeLineFrameRate;

    TiDspInfo() :
            pipeLineFrameRate{0}
    {

    }
};

enum class FromTiCameraReason : unsigned char
{
    FrameRateUpdate,
    Invalid
};

enum class ToTiCameraReason : unsigned char
{
    Invalid
};

enum class FromTiDspReason : unsigned char
{
    FrameRateUpdate,
    Invalid
};

enum class ToTiDspReason : unsigned char
{
    OutputSelectChange,
    DownScaleChange,
    EdgeEnhanceChange,
    UpscaleChange,
    EzoomChange,
    Invalid,
};

enum class ToGuiReason : unsigned char
{
    Invalid
};

//Copy of the above enum for naming reasons in the code
using FromGuiReason = ToGuiReason;

struct FromTiDspThread
{
    FromTiDspReason reason;
    TiDspInfo info;
};

struct ToTiDspThread
{
    ToTiDspReason reason;
    TiDspParameters params;
};

struct FromTiCameraThread
{
    FromTiCameraReason reason;
    TiCameraInfo info;
};

struct ToTiCameraThread
{
    ToTiCameraReason reason;
    TiCameraParameters params;
};

struct ToGuiThread
{
    ToGuiReason reason;
};

//Copy of the above threads for naming reasons in the code
using FromGuiThread = ToGuiThread;

constexpr int queueDepth = 32;
struct Queues
{
    boost::lockfree::spsc_queue<FromTiCameraThread> &fromTiCameraToGui;
    boost::lockfree::spsc_queue<ToTiCameraThread> &fromGuiToTiCamera;
    boost::lockfree::spsc_queue<FromTiDspThread> &fromTiDspToGui;
    boost::lockfree::spsc_queue<ToTiDspThread> &fromGuiToTiDsp;
    boost::lockfree::spsc_queue<FromGuiThread> &fromGuiToGui;
};

class threads
{
public:
    static std::shared_ptr<threads> create(Threads t, Queues &qs); //Factory
    virtual void run(void) = 0;
    static bool useCamera;
    static std::string framePath;

private:
};

