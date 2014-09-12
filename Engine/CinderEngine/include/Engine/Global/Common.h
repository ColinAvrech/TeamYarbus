/******************************************************************************/
/*!
\file   Common.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The precompiled header file which contains all the includes 
from externals, and stable elements within the engine.
*/
/******************************************************************************/


#pragma once

//! External Dependencies
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm> //! std::sort


//! Stable includes and dependencies
#include "CinderWindows.h"
#include "CinderDebug.h"
#include "Console.h"
#include "OpenGLIncludes.h"

#pragma comment (lib, "fmod_vc.lib")


//! Unstable dependency
#include "ComponentTypeIds.h"
#include "GOC.h"
#include "BaseSystem.h"
#include "Events.h"
#include "BaseEvent.h"
#include "Vec2.h"
#include "Affine.h"