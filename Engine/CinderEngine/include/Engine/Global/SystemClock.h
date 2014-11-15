/******************************************************************************/
/*!
\file   SystemClock.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SYSTEM_CLOCK_H
#define _SYSTEM_CLOCK_H

#include "Common.h"

namespace Framework
{

  typedef unsigned long long SystemClocks;

  inline SystemClocks GetSystemClock ()
  {
    LARGE_INTEGER time;
    QueryPerformanceCounter (&time);
    return time.QuadPart;
  }

  inline double SystemClocksToSeconds (SystemClocks clocks)
  {
    static double frequency = 0;
    if (frequency == 0)	// First time only.
    {
      static LARGE_INTEGER liFrequency;
      QueryPerformanceFrequency (&liFrequency);
      frequency = static_cast<double>(liFrequency.QuadPart);
    }

    return clocks / frequency;
  }

  inline double SystemClocksToMilliseconds (SystemClocks clocks)
  {
    return 1000.0 * SystemClocksToSeconds (clocks);
  }
}

#endif