/******************************************************************************/
/*!
\file   ThreadFunctions.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _THREAD_FUNCTIONS_H
#define _THREAD_FUNCTIONS_H


#include "SystemClock.h"
#include "Thermodynamics.h"

namespace Framework
{
  using namespace Physics;

  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // THERMODYNAMICS::UpdateTemp
  //////////////////////////////////////////////////////////////////////////

  HANDLE eventStartTemperature [ThermodynamicsSystem::kNumThreads];
  HANDLE eventEndTemperature [ThermodynamicsSystem::kNumThreads];

  DWORD WINAPI UpdateTemperatureFunc (LPVOID lpparam)
  {
    //static double deltaTime = 0.016;
    //SystemClocks startClocks = GetSystemClock ();
    static int currentIndex = -1;
    InterlockedIncrement ((LONG*) &currentIndex);
    int threadIndex = currentIndex;
    const int numRows = (int)ThermodynamicsSystem::MapSize.x / ThermodynamicsSystem::kNumThreads;
    int startIndex = threadIndex * numRows;
    int endIndex = startIndex + numRows - 1;

    while (true)
    {
      //deltaTime = SystemClocksToSeconds (GetSystemClock () - startClocks);
      //startClocks = GetSystemClock ();
      //std::cout << deltaTime << "\n";
      WaitForSingleObject (eventStartTemperature [threadIndex], INFINITE);
      THERMODYNAMICS->UpdateTemp (startIndex, endIndex, 0.5);
      SetEvent (eventEndTemperature [threadIndex]);
    }

    return 0;
  }

  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // THERMODYNAMICS::ComputeVelocity
  //////////////////////////////////////////////////////////////////////////

  HANDLE eventStartVelocity [ThermodynamicsSystem::kNumThreads];
  HANDLE eventEndVelocity [ThermodynamicsSystem::kNumThreads];

  DWORD WINAPI UpdateVelocityFunc (LPVOID lpparam)
  {
    static int currentIndex = -1;
    InterlockedIncrement ((LONG*) &currentIndex);
    int threadIndex = currentIndex;
    const int numRows = (int)ThermodynamicsSystem::MapSize.x / ThermodynamicsSystem::kNumThreads;
    int startIndex = threadIndex * numRows;
    int endIndex = startIndex + numRows - 1;

    while (true)
    {
      WaitForSingleObject (eventStartVelocity [threadIndex], INFINITE);
      THERMODYNAMICS->ComputeVelocity (startIndex, endIndex, 0.016);
      SetEvent (eventEndVelocity [threadIndex]);
    }

    return 0;
  }


  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // THERMODYNAMICS::UpdateFire
  //////////////////////////////////////////////////////////////////////////

  HANDLE eventStartFire [ThermodynamicsSystem::kNumThreads];
  HANDLE eventEndFire [ThermodynamicsSystem::kNumThreads];

  DWORD WINAPI UpdateFireFunc (LPVOID lpparam)
  {
    static int currentIndex = -1;
    InterlockedIncrement ((LONG*) &currentIndex);
    int threadIndex = currentIndex;
    const int numRows = (int)ThermodynamicsSystem::MapSize.x / ThermodynamicsSystem::kNumThreads;
    int startIndex = threadIndex * numRows;
    int endIndex = startIndex + numRows - 1;

    while (true)
    {
      WaitForSingleObject (eventStartFire [threadIndex], INFINITE);
      //THERMODYNAMICS->UpdateFire (0.016);
      SetEvent (eventEndFire [threadIndex]);
    }

    return 0;
  }

  /////////////////////////////////////////////////////////////////////////////////
  ////		Euler Integration
  /////////////////////////////////////////////////////////////////////////////////

  //HANDLE eventStartEuler [ParticleSystem::kNumEulerThreads];
  //HANDLE eventEndEuler [ParticleSystem::kNumEulerThreads];

  //DWORD WINAPI ForwardEulerFunc (LPVOID lpparam)
  //{
  //  static int currentIndex = -1;
  //  InterlockedIncrement ((LONG*) &currentIndex);

  //  int threadIndex = currentIndex;

  //  const int numParticlesPerQuarter = ParticleSystem::kNumParticles / ParticleSystem::kNumEulerThreads;

  //  int startIndex = threadIndex * numParticlesPerQuarter;
  //  int endIndex = startIndex + numParticlesPerQuarter - 1;

  //  while (true)
  //  {
  //    WaitForSingleObject (eventStartEuler [threadIndex], INFINITE);
  //    ParticleSystem::GetRef ().UpdateParticleWind (startIndex, endIndex, 0.016);
  //    SetEvent (eventEndEuler [threadIndex]);
  //  }

  //  return 0;
  //}
}

#endif