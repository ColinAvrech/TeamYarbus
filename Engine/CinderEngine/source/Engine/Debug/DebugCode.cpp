/******************************************************************************/
/*!
\file   DebugCode.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains all functions for Debugging code:

->TracePrint: Prints the message to the consule.
->SignalError: Prints the message to the consule, and opens a message box for the user with the error.

*/
/******************************************************************************/

#include "CinderDebug.h"
#include "Console.h"
#include <iostream>

//! SignalError is passed the Error message, filename, line of that file, and the
bool SignalError(const char* exp, const char * filename, int line_number, const char * formatMessage, ...)
{
  const unsigned BufferSize = 1024;
  char Message[BufferSize];
  
  //!if there is a message we need to add it to the end
  if (formatMessage != NULL)
  {
    va_list args;
    va_start(args, formatMessage);
    vsnprintf_s(Message, BufferSize, _TRUNCATE, formatMessage, args);
    va_end(args);
  }
  else
    strcpy_s(Message, BufferSize, "No Error Message! :( ");

  //! Add newline to the end
  strcat_s(Message, BufferSize, "\n");

  std::cerr << Console::red;
  std::cerr << "************************************************************" << std::endl;
  std::cerr << "********************AN ERROR HAS OCCURRED*******************" << std::endl;
  std::cerr << "************************************************************" << std::endl;

  std::cerr << Console::darkyellow << "File: " << Console::yellow << filename << std::endl;
  std::cerr << Console::darkgray << "Line Number: " << Console::gray << line_number << std::endl;
  std::cerr << Console::darkred << "Error Message: " << Console::red << Message << std::endl;

  //!Display a message box for the user
  MessageBox(NULL, Message, "ERROR: Oh noes!", MB_OK);

  //!Debug Break after printing message
  return true;
}

/*! Trace print is called for when systems are initialized
or specific events (not in the event system) happen.*/
void TracePrint(const char * msg, ...)
{
  const unsigned BufferSize = 1024;
  char Message[BufferSize];

  va_list args;
  va_start(args, msg);
  vsnprintf_s(Message, BufferSize, _TRUNCATE, msg, args);
  va_end(args);

  std::cout << Console::green << Message << std::endl;
}

/*! Enables memory checking.
This should help prevent leaking in Debug mode ONLY*/
void EnableMemoryLeakChecking(int breakAlloc)
{
  //! set the leak checking flag
  int tempDebugFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
  tempDebugFlag |= _CRTDBG_LEAK_CHECK_DF;
  _CrtSetDbgFlag(tempDebugFlag);

  if (breakAlloc != -1)
    _CrtSetBreakAlloc(breakAlloc);
}