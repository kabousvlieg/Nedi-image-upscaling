/*
 * thread.cpp
 *
 *  Created on: 12 Jan 2016
 *      Author: kobus
 */

#include "../ti/ti_thread.h"
#include "../gui/gui_thread.h"
#include "../ti/camera.h"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

std::shared_ptr<threads> threads::create(Threads t, Queues &qs)
{
    switch (t)
    {
        case Threads::Gui_Thread:
            return std::shared_ptr<threads>(gui_thread::create(qs));
        case Threads::Ti_Thread_CameraBuffer:
            return std::shared_ptr<threads>(new Camera_thread(qs.fromTiCameraToGui, qs.fromGuiToTiCamera));
        case Threads::Ti_Thread_SignalProcessing:
            return std::shared_ptr<threads>(new TiDsp_thread(qs.fromTiDspToGui, qs.fromGuiToTiDsp));
    }
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
    return nullptr;
#pragma clang diagnostic pop
}
