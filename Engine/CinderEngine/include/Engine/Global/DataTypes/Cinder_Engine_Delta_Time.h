/******************************************************************************/
/*!
\file   Cinder_Engine_Delta_Time.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _CINDER_ENGINE_DELTA_TIME
#define _CINDER_ENGINE_DELTA_TIME


#include "Cinder_Engine_Data_Types.h"
using namespace  Framework;

  class Cinder_DT
  {
  public:

    Cinder_DT ()
    {}

    virtual ~Cinder_DT ()
    {}

    static const REAL32 GetDeltaTime ()
    {
      QueryPerformanceCounter (&EndTime);
      DeltaTime = REAL32 (EndTime.QuadPart - StartTime.QuadPart)
        / REAL32 (Frequency.QuadPart);
      QueryPerformanceFrequency (&Frequency);
      QueryPerformanceCounter (&StartTime);

      return DeltaTime;
    }

  private:
    static REAL32 DeltaTime;
    static LARGE_INTEGER Frequency;
    static LARGE_INTEGER StartTime;
    static LARGE_INTEGER EndTime;
  };

#endif

REAL32 Cinder_DT::DeltaTime = 0.0f;
LARGE_INTEGER Cinder_DT::Frequency = LARGE_INTEGER ();
LARGE_INTEGER Cinder_DT::StartTime = LARGE_INTEGER ();
LARGE_INTEGER Cinder_DT::EndTime = LARGE_INTEGER ();