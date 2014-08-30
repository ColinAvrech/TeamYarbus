/*!

/auther Micah
/file CinderWindows.h

/brief Holds all the windows.h includes so that windows.h is only included once

*/


#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>  //! Windows types

//! WIN32_LEAN_AND_MEAN does not define DragAcceptFiles
#include <shellapi.h>
#pragma comment (lib, "shell32.lib")

#undef CreateEvent