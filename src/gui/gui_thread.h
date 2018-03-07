/*
 * gui_thread.h
 *
 *  Created on: 05 Jan 2016
 *      Author: kc
 */

#pragma once


#include <bits/unique_ptr.h>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include "../util/threads.h"

class gui_thread : public threads
{
public:
    static std::shared_ptr<threads> create(Queues &qs);    //Singleton pattern
    gui_thread(gui_thread const&) = delete;               //delete because singleton
    gui_thread & operator=(gui_thread const&) = delete;   //delete because singleton
    void run(void) override;

private:
    static std::shared_ptr<threads> singletonInstance;
    gui_thread(Queues &qs);                             //Private because singleton
    Queues& queues;
};

