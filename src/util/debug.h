/*
 * debug.h
 *
 *  Created on: 14 Jul 2016
 *      Author: kobus
 */

#pragma once

//#define LOG

//#define PRINT_FORT_COMMS    //I never bothered to create a fort printout since the fort simulator gave me that info
#define PRINT_TI_COMMS      0 //Prints the serial data to from the TI to stdout

#define PRINT_TI_THREAD_DEBUG       0
#define PRINT_GUI_THREAD_DEBUG      0

#define PRINT_TI_COMMAND_DEBUG      0
#define PRINT_GUI_COMMAND_DEBUG     0

#define PRINT_STATUS_COMMANDS       0


//Module specific debug switches
#if ( (defined(TI_THREAD_CPP) && PRINT_TI_THREAD_DEBUG) ||            \
      (defined(GUI_THREAD_CPP) && PRINT_GUI_THREAD_DEBUG) ||        \
      (defined(TI_COMMAND_CPP) && PRINT_TI_COMMAND_DEBUG) ||          \
      (defined(GUI_COMMAND_CPP) && PRINT_GUI_COMMAND_DEBUG) )
    #define DEBUG
#endif

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) printf((fmt "\r\n"), ##__VA_ARGS__);
#else
    #define DEBUG_PRINT(...)
#endif

#ifdef LOG
      #define LOG_PRINT(fmt, ...) fprintf(stderr, ("[%s:%d] %s: " fmt "\r\n"), __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);
#else
      #define LOG_PRINT(...)
#endif

#define CONSOLE_PRINT(fmt, ...) printf(fmt "\r\n", ##__VA_ARGS__);

