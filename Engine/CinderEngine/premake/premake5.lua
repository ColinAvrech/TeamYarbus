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
  linker    =   {"glfw3.lib", "glfw3dll.lib", "glew32.lib", "SOIL.lib",
                 "glut32.lib", "opengl32.lib", "fmod_vc.lib", "Zilch.lib", "AntTweakBar.lib", "freetype.lib"}
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
      "../source/**.cpp",
      "../source/**.c",
      "../source/**.tpp",
      "../../Resources/Shaders/**.glsl",
      "../../Resources/**.txt",
      "../../Resources/**.data",
      "../../Resources/**.z",
    }
    
    excludes
    {
      "../source/Engine/System/Physics/grid2d.cpp"
    }
    
    
    includedirs { "../dependency/opengl/include**",
                  "../dependency/zilch/include",
                  "../dependency/fmod/include",
									"../dependency/AntTweakBar/include",
                  "../include/**",
			      "../source/**",
				  }
    
    libdirs { "../dependency/opengl/library",
              "../dependency/zilch/library",
              "../dependency/fmod/library",
							"../dependency/AntTweakBar/library"}


--[[ Debug Configuration ]]
    configuration "Debug"
      flags   { "Symbols" }
      defines { "_USE_ANTWEAK" , "_DEBUG"  , "_CRT_SECURE_NO_WARNINGS" }
      links {linker}
      targetdir "../bin/debug"
      postbuildcommands
              {
                "copy ..\\dependency\\opengl\\library\\glfw3.dll ..\\bin\\debug\\",
                "copy ..\\dependency\\opengl\\library\\glew32.dll ..\\bin\\debug\\",
                "copy ..\\dependency\\opengl\\library\\freetype6.dll ..\\bin\\debug\\",
                "copy ..\\dependency\\opengl\\library\\glut32.dll ..\\bin\\debug\\",
                "copy ..\\dependency\\fmod\\library\\fmod.dll ..\\bin\\debug\\",
                "copy ..\\dependency\\AntTweakBar\\library\\AntTweakBar.dll ..\\bin\\debug\\",
              }
        
--[[ Release Configuration ]]        
    configuration "Release"
      flags   { "Symbols" }
      defines { "_USE_ANTWEAK" , "NDEBUG"  , "_CRT_SECURE_NO_WARNINGS" }      
      optimize  "Full"
      links {linker}
      targetdir "../bin/release"
      postbuildcommands
              {
                "copy ..\\dependency\\opengl\\library\\glfw3.dll ..\\bin\\release\\",
                "copy ..\\dependency\\opengl\\library\\glew32.dll ..\\bin\\release\\",
                "copy ..\\dependency\\opengl\\library\\freetype6.dll ..\\bin\\release\\",
                "copy ..\\dependency\\opengl\\library\\glut32.dll ..\\bin\\release\\",
		"copy ..\\dependency\\fmod\\library\\fmod.dll ..\\bin\\release\\",
		"copy ..\\dependency\\AntTweakBar\\library\\AntTweakBar.dll ..\\bin\\release\\"        
              }
