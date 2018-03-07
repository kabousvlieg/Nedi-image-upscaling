/*
 * fort_thread.cpp
 *
 *  Created on: 05 Jan 2016
 *      Author: kc
 */

#define FORT_THREAD_CPP //Debugging

#include "gui_thread.h"
#include "widgets.h"
#include "../util/exceptions.h"
#include "../util/debug.h"
#include "../util/timeClock.h"
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <boost/pointer_cast.hpp>
#include <string.h>

std::shared_ptr<threads> gui_thread::singletonInstance = nullptr;
class MyApp* wxGui = new MyApp();


gui_thread::gui_thread(Queues &qs)
	: queues{qs}
{
	//TODO Setup wxWidgets here
	wxApp::SetInstance( wxGui );
	int dummy_argc = 0;
	char** dummy_argv;
	wxEntryStart( dummy_argc, dummy_argv );
    //wxEntryStart( );
	wxGui->OnInit();
	wxGui->InitQueues(&qs);
}


std::shared_ptr<threads> gui_thread::create(Queues &qs) //Singleton pattern
{
    if (!singletonInstance)   // Only allow one instance of class to be generated.
    {
        singletonInstance = std::shared_ptr<threads>(new gui_thread(qs));
        return singletonInstance;
    }
    throw Except("Only one instance of gui_thread allowed");
}


void gui_thread::run(void)
{
    for(;;)
    {
        try
        {
        	wxGui->OnRun(); //This is blocking black hole
        	std::cout << "GUI problem wx returned" << std::endl;
        	break;
        }
        catch (std::exception& e)
        {
            std::cout << "Caught exception:" << std::endl;
            std::cout << e.what() << std::endl;
        }
        catch (std::exception* e)
        {
            std::cout << "Caught exception:" << std::endl;
            std::cout << e->what() << std::endl;
        }
    }
    wxGui->OnExit();
	wxEntryCleanup();
}
