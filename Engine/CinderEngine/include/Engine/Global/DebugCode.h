/******************************************************************************/
/*!
\file   DebugCode.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#pragma once


//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
#include <stdio.h>    //! sprintf_s, vsnprintf_s
#include <stdarg.h>   //! va_start, va_end
#include <string.h>   //! strcpy_s, str_cat_s
#include <debugapi.h> //! OutputDebugString
#include <crtdbg.h>   //! EnableMemoryLeakChecking

/*!
***********************************************************
Assert, ErrorIf, Trace
***********************************************************
*/


//!If using Microsoft Visual Studio, use their debugbreak
#ifdef _MSV_VER
#define DEBUG_BREAK __debugbreak();
#else
//! inline assembly interrupt commend
#define DEBUG_BREAK do { __asm int 3 } while(0)
#endif


/*!Used to explicity enable or disable debugging and diagnostics macros*/
#if !defined(ENABLE_DEBUG_CODE)
#   if defined(_DEBUG)
#       define ENABLE_DEBUG_CODE 1
#   else
#       define ENABLE_DEBUG_CODE 0
#   endif
#endif

//!Debug Functions which are called through the macros

//! SignalError is passed the Error message, filename, line of that file, and the
bool SignalError(const char* exp, const char * filename, int line_number, const char * formatMessage = 0, ...);

/*! Trace print is called for when systems are initialized
or specific events (not in the event system) happen.*/
void TracePrint(const char * msg, ...);

//!When DEBUG_CODE is active
#if ENABLE_DEBUG_CODE

/*! Assert Macro:
->do/while block for for if statment.
->if exp is true, signal Error Handler is called
->if Error handler is called, Debug break! 
*/
#define Assert(exp, ...)                                  \
  do { if (!(exp) && SignalError(#exp, __FILE__, __LINE__, __VA_ARGS__)) \
    DEBUG_BREAK;}  while (0)

#define ErrorIf(exp, ...)                                  \
  do { if ((exp) && SignalError(#exp, __FILE__, __LINE__, __VA_ARGS__)) \
  DEBUG_BREAK;}  while (0)


/*! Trace Macro:
->prints the statement to the consule
*/
//#define Trace(...) TracePrint(__VA_ARGS__)
#else

/*!If diagnostics are not enabled, functions do nothing*/
#define Assert(...) ((void)0)
#define ErrorIf(...) ((void)0)
//#define Trace(...) ((void)0)

#endif

/*!
***********************************************************
EnableMemoryLeakChecking
***********************************************************
*/

void EnableMemoryLeakChecking(int breakAlloc = -1);
