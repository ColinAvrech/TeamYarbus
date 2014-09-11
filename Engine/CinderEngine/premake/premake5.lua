--[[
/******************************************************************************/
/*!
 \file   premake5.lua
 \author Manas Sudhir Kulkarni
 \par    Course: GAM200
 \par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
 \brief
 */
/******************************************************************************/
]]
solution "CinderEngine"
  configurations { "Debug", "Release" }
  location "../"
  project "CinderEngine"
    targetname "CinderEngine"
    location "."
    language "C++"
    objdir "../obj"
    kind "WindowedApp"

    -- source files
    files
    {
      "../include/**.h",
      "../source/**.cpp"
    }
    
    includedirs { "../dependency/opengl/include",
                  "../dependency/Zilch/include",
                  "../dependency/fmod/include",
                  "../include/**" }
    
    libdirs { "../dependency/opengl/library",
              "../dependency/Zilch/library",
							"../dependency/fmod/library"}


--[[ Debug Configuration ]]
    configuration "Debug"
      flags   { "Symbols" , "WinMain" }
      defines { "_DEBUG"  , "_CRT_SECURE_NO_WARNINGS" }
      targetdir "../bin/debug"
      postbuildcommands
              {
                "copy ..\\dependency\\opengl\\library\\glut32.dll ..\\bin\\debug\\",
								"copy ..\\dependency\\fmod\\library\\fmod.dll ..\\bin\\debug\\"
              }
        
--[[ Release Configuration ]]        
    configuration "Release"
      flags   { "Symbols" , "WinMain" }
      defines { "NDEBUG"  , "_CRT_SECURE_NO_WARNINGS" }      
      optimize  "Full"
      targetdir "../bin/release"
      postbuildcommands
              {
                "copy ..\\dependency\\opengl\\library\\glut32.dll ..\\bin\\release\\",
								"copy ..\\dependency\\fmod\\library\\fmod.dll ..\\bin\\debug\\"								
              }
        