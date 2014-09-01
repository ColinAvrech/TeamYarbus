/******************************************************************************/
/*!
\file   CinderWindows.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Include File for all windows functionality
*/
/******************************************************************************/


#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>  //! Windows types

//! WIN32_LEAN_AND_MEAN does not define DragAcceptFiles
#include <shellapi.h>
#pragma comment (lib, "shell32.lib")

#undef CreateEvent