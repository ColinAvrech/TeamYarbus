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

//#include "Comtainers.h"

//! External Dependencies
#include <iostream>
#include <fstream>
#include <chrono>
#include <functional>
#include <thread>
#include <list>
#include <vector>
#include <utility>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <algorithm> //! std::sorts




//Windows
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

//! Global Headers
#include "Console.h"
#include "DebugCode.h"
#include "Helpers.h"

