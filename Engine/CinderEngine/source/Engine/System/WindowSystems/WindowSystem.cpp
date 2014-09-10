/******************************************************************************/
/*!
\file   WindowSystem.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This system creates a window for the graphics system. It also holds the
function to handle windows Messages.
*/
/******************************************************************************/

#include "windowsystem.h"

namespace Framework
{
  //! Global pointer to  the windows system.
  WindowSystem* WINDOWSYSTEM = NULL;

  namespace WindowData
  {
    /*! Use by the windows to identify the window class type, this is needed
    for registering, creagin, unregistering the window*/
    const char windowClassName[] = "CinderEngine_WindowClass";

    //! The icon for the window is in the same folder as the code. @TODO make a resource folder
    LPCTSTR WindowIcon = "CinderEngine_Icon.ico";
    HICON WindowIconHandle;
    //! The animated Icon for the cursor is int he same folder as the code. @TODd make resource folder
    LPCTSTR WindowCursor = "CinderEngine_Cursor";
    HCURSOR WindowCursorHandle;
  }

  //! This is for the message handling precedure in the game
  LRESULT WINAPI MessageHandler(HWND hWnd,      //!The window the message is for (our game)
                                UINT msg,       //!The message type
                                WPARAM wParam,  //!The primary data for the message.
                                LPARAM lParam)  //!The secondary data for the message (if any)

  {
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }

  //! @@ TODO MAKE THIS HANDLE INPUT MESSAGES... Probably make a new system called input and have it be a friend class to talk to this message handler


  WindowSystem::WindowSystem(HINSTANCE hInst, const char* WindowTitle, int ClientWidth, int ClientHeight)
  {
    //! @TODO make a check so that if th windows system is already created to not make another!
    

    //!Set the Global pointer to the windows system
    WINDOWSYSTEM = this;

    //! Set window demensions
    WindowHeight = ClientHeight;
    WindowWidth = ClientWidth;

    //!Load the windows icon /|\ Loaded Cursor icon
    WindowData::WindowIconHandle = LoadIcon(NULL, WindowData::WindowIcon);
    WindowData::WindowCursorHandle = LoadCursorFromFile(WindowData::WindowCursor);

    /*! The size passed to CreateWindow is the full size including the window border and caption
    AdjucstWindowRect will adjust the provided rect so that the client size of the window is the
    desired size.*/
    RECT fullWinRect = { 0, 0, ClientWidth, ClientHeight };
    AdjustWindowRect(&fullWinRect,        //!The Rectangle for the full size of the window
                     WS_OVERLAPPEDWINDOW, //!The Style of the window, which must match what is passed in to CreateWindow below
                     FALSE);              //!Does this window have a menu?
    
    //!Register the window class for the game. 
    WNDCLASSEX wcEx;                                   //! The windows class ex setup so we can register is later
    wcEx.cbSize = sizeof(WNDCLASSEX);                  //! The size of the structure (For compatability)
    wcEx.style = CS_CLASSDC;                           //! The style of the window class, (CS_CLASSDC is a base type)
    wcEx.lpfnWndProc = MessageHandler;                 //! The name of the message handling function
    wcEx.cbClsExtra = 0L; wcEx.cbWndExtra = 0L;        //! The amound of extram memory to allocate for this class and window
    wcEx.hInstance = hInst;                            //! The Handle to the instance that has to window procedure (NULL makes it use this file.
    wcEx.hIcon = WindowData::WindowIconHandle;         //! handle to the loaded icon
    wcEx.hCursor = WindowData::WindowCursorHandle;     //! handle to the loaded cursor
    wcEx.hbrBackground = NULL;                         //! NULL for default
    wcEx.lpszMenuName = NULL;                          //! NULL for default
    wcEx.lpszClassName = WindowData::windowClassName;  //! NULL for default
    wcEx.hIconSm = WindowData::WindowIconHandle;       //!handl to the loaded small icon

    ATOM wcATOM = RegisterClassEx(&wcEx);   //Reigister windwos class so that we can activate it later.

    Assert(wcATOM, "Failed to RegisterWindowClass");

    //! Store the handle ot the instance
    hInstance = wcEx.hInstance;

    //!create the game's window
    hWnd = CreateWindow(
      WindowData::windowClassName,           //! The window class name
      WindowTitle,                           //! The name for the title bar
      WS_OVERLAPPEDWINDOW,                   //! The Style of the window (can be broken down into more parts: (WS_BORDER | BS_MINIMIZEDBOX | WS_MAXIMIZE | etc... check msdn for more options)
      CW_USEDEFAULT, CW_USEDEFAULT,          //! The x and y position of the window (coordinates for the base windows, also relative coordinates for the chile windows)
      (fullWinRect.right - fullWinRect.left),//! These are screen coordinates, so to get the width (including borders) we subtract the right from the left
      (fullWinRect.bottom - fullWinRect.top),//! These are screen coordinates, so to get the height (including borders) we subtract the bottom from the top
      GetDesktopWindow(),                    //! The parent window
      NULL,                                  //! The menu for the window (null: there is no menu)
      hInstance,                             //! handle to the window
      NULL);                                 //! lpParam for the UM_CREATE message that will be sent to the message handler                         

    SetFocus(hWnd);										      //! Sets Keyboard Focus To The Window	

    DragAcceptFiles(hWnd, true);   //*This will tell windows that it can accept files which are dragged into the window.
  }


  WindowSystem::~WindowSystem()
  {
    //! Destroy the Icon /|\ Destroy Cursor. These must not be in use to destroy
    if (DestroyIcon(WindowData::WindowIconHandle))
      Trace("Window Icon Destroyed");
    else
      Trace("ERROR: Windows Icon was not destroyed");

    if (DestroyCursor(WindowData::WindowCursorHandle))
      Trace("Window Cursor Destroyed");
    else
      Trace("ERROR: Windows Cursor was not destroyed");

  }

  void WindowSystem::ActivateWindow()
  {
    /*!Show the window (check: http://msdn.microsoft.com/en-us/library/windows/desktop/ms633548(v=vs.85).aspx for other options)*/
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    //!Send a WM_PAIN message to the window
    UpdateWindow(hWnd);
  }

  void WindowSystem::Update(const double dt)
  {
    return;
  }
}
