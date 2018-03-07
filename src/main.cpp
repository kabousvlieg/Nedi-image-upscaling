//============================================================================
// Name        : main.cpp
// Author      : kc
// Version     :
// Copyright   : Your copyright notice
// Description : POC TI Pipeline
//============================================================================


//#include <cuda.h>
#include <iostream>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/thread/thread.hpp>
#include "util/timeClock.h"
#include "util/threads.h"
#include <X11/Xlib.h>
#include <gtk/gtk.h>


static void spawnThreads(void);

int main(int argc, char **argv)
{
	std::cout << "Date compiled: " << __DATE__ << " - " << __TIME__  << std::endl;
	try
	{
		timeClock();
        //Deprecated, not necessary anymore
//		if( ! g_thread_supported() )
//			g_thread_init( NULL );

		/* Secure gtk */
		gdk_threads_init();
		XInitThreads();
		gtk_disable_setlocale();
		spawnThreads();
		for(;;)
		{
			//System monitor
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Caught exception:" << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << "Exciting application" << std::endl;
	}
	catch (std::exception* e)
	{
		std::cout << "Caught exception:" << std::endl;
		std::cout << e->what() << std::endl;
	}
}

static void spawnThreads(void)
{
    //create message queues
    static boost::lockfree::spsc_queue<FromTiCameraThread> fromTiCameraToGui(queueDepth);
    static boost::lockfree::spsc_queue<ToTiCameraThread> fromGuiToTiCamera(queueDepth);
    static boost::lockfree::spsc_queue<FromTiDspThread> fromTiDspToGui(queueDepth);
    static boost::lockfree::spsc_queue<ToTiDspThread> fromGuiToTiDsp(queueDepth);
    static boost::lockfree::spsc_queue<FromGuiThread> fromGuiToGui(queueDepth);
    Queues qs {
        .fromTiCameraToGui = fromTiCameraToGui,
        .fromGuiToTiCamera = fromGuiToTiCamera,
        .fromTiDspToGui = fromTiDspToGui,
        .fromGuiToTiDsp = fromGuiToTiDsp,
        .fromGuiToGui = fromGuiToGui
    };

    //create threads objects
	static auto tiCamera = threads::create(Threads::Ti_Thread_CameraBuffer, qs);
    static auto tiDsp = threads::create(Threads::Ti_Thread_SignalProcessing, qs);
    static auto gui = threads::create(Threads::Gui_Thread, qs);

    //launch threads
#pragma clang diagnostic push
#pragma ide diagnostic ignored "IncompatibleTypes"
    boost::thread tiCameraThread {&threads::run, tiCamera};
    boost::thread tiDspThread {&threads::run, tiDsp};
    boost::thread guiThread {&threads::run, gui};
#pragma clang diagnostic pop
}
