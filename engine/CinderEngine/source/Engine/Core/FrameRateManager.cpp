/******************************************************************************/
/*!
\file   FrameRateManager.cpp
\author Damien Low
\par    Course: GAM250
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Manages frame rate for the game
*/
/******************************************************************************/
#include "Precompiled.h"
#include "FrameRateManager.h"

namespace Framework
{
	FrameRateManager::FrameRateManager(void)
	{
		// Initialize times when engine starts
		engineStartTime_ = GetCurrTime();
		frameStartTime_ = engineStartTime_;
		frameEndTime_ = engineStartTime_;
		currentTime_ = engineStartTime_;

		// Start at 60 FPS
		frameRate_ = 60;
		deltaTime_ = 1.f / frameRate_;
	}

	void FrameRateManager::StartFrame(void)
	{
		// Get time for start of frame
		QueryPerformanceCounter(&frameStartTime_);
	}

	void FrameRateManager::EndFrame(void)
	{
		// Get time for end of frame
		QueryPerformanceCounter(&frameEndTime_);
		QueryPerformanceFrequency(&frequency);

		// Calculate current delta time
		currentDT = double(frameEndTime_.QuadPart - frameStartTime_.QuadPart) / double(frequency.QuadPart);

		// Wait till its time to update again
		while (currentDT < double(1.0 / frameRate_))
		{
			QueryPerformanceCounter(&frameEndTime_);
			currentDT = double(frameEndTime_.QuadPart - frameStartTime_.QuadPart) / double(frequency.QuadPart);
		}

		// Set the new delta time
		deltaTime_ = float(currentDT);

		// Delta Time Test
//		std::cout << CinderConsole::darkyellow << "delta time:" << deltaTime_ << std::endl;
	}

	float FrameRateManager::GetDeltaTime(void)
	{
		return deltaTime_;
	}

	unsigned FrameRateManager::GetDesiredFrameRate(void)
	{
		return frameRate_;
	}

	LARGE_INTEGER FrameRateManager::GetCurrTime(void)
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return time;
	}
}

