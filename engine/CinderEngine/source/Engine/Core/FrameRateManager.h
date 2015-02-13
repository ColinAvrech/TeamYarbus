/******************************************************************************/
/*!
\file   FrameRateManager.h
\author Damien Low
\par    Course: GAM250
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Manages frame rate for the game
*/
/******************************************************************************/
#pragma once

namespace Framework
{
	static LARGE_INTEGER frequency;
	static double currentDT;

	class FrameRateManager
	{
	public:
		FrameRateManager(void);
		void StartFrame(void);		// Start time check for a frame
		void EndFrame(void);		// End time check for a frame
		
		float GetDeltaTime(void);			// Get the actual delta time
		unsigned GetDesiredFrameRate(void);	// Get the desired frame rate

	private:
		LARGE_INTEGER GetCurrTime(void);	// Gets current time

		LARGE_INTEGER engineStartTime_;	// Time when engine starts
		LARGE_INTEGER frameStartTime_;	// Starting time of the current frame
		LARGE_INTEGER frameEndTime_;	// Ending time of the current frame
		LARGE_INTEGER currentTime_;		// Current time of the game

		float deltaTime_;				// Time taken for the last frame 
		unsigned frameRate_;			// Amount of frames per second
	};
}