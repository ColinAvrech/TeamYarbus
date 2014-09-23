/******************************************************************************/
/*!
\file   TimeQuery.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "TimeQuery.h"
#include "GraphicsCommon.h"

namespace Framework
{

  ///////////////////////////////////////////////////////////////////////////////
  GpuTimerQuery::GpuTimerQuery ()
  {
    mQuery = 0;
    resetTime ();
  }

  ///////////////////////////////////////////////////////////////////////////////
  GpuTimerQuery::~GpuTimerQuery ()
  {
    deleteQuery ();
  }

  ///////////////////////////////////////////////////////////////////////////////
  void GpuTimerQuery::init ()
  {
    deleteQuery ();
    resetTime ();
    glGenQueries (1, &mQuery);
  }

  ///////////////////////////////////////////////////////////////////////////////
  void GpuTimerQuery::updateResults (WaitOption wait)
  {
    // query time results
    int stopTimerAvailable = (wait == WaitOption::WaitForResults ? 0 : 1);
    while (!stopTimerAvailable)
    {
      glGetQueryObjectiv (mQuery, GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailable);
    }

    GLuint64 t;
    glGetQueryObjectui64v (mQuery, GL_QUERY_RESULT, &t);
    mWholeTime += t;
    mCounter++;

    double msec = (double) t / 1000000.0;
    mTime = 0.25*(msec + mTimes [0] + mTimes [1] + mTimes [2]);
    mTimes [2] = mTimes [1];
    mTimes [1] = mTimes [0];
    mTimes [0] = msec;
  }



  ///////////////////////////////////////////////////////////////////////////////
  void GpuTimerQuery::deleteQuery ()
  {
    if (mQuery > 0)
      glDeleteQueries (1, &mQuery);
    mQuery = 0;
  }

  namespace utils
  {
    void calculateFps (float *fps)
    {
      static unsigned int frame = 0;
      static int timeBase = 0;

      frame++;

      int t = (int)glfwGetTime ();
      if (t - timeBase > 1000)
      {
        *fps = 0.5f*(*fps) + 0.5f*(frame*1000.0f / (float) (t - timeBase));
        timeBase = t;
        frame = 0;
      }

    }

    void updateTimer (double *deltaTime, double *appTime, const double MAX_REFRESH_TIME)
    {
      static double lastDeltas [3] = { 0.0, 0.0, 0.0 };

      // in milisec
      int t = (int)glfwGetTime();
      double newTime = (double) t*0.001;

      *deltaTime = newTime - *appTime;
      if ((*deltaTime) < 0) *deltaTime = 0.0;
      if ((*deltaTime) > MAX_REFRESH_TIME) *deltaTime = MAX_REFRESH_TIME;

      // average:
      *deltaTime = ((*deltaTime) + lastDeltas [0] + lastDeltas [1] + lastDeltas [2]) * 0.25;

      *appTime = (*appTime) + (*deltaTime);

      // save delta:
      lastDeltas [0] = lastDeltas [1];
      lastDeltas [1] = lastDeltas [2];
      lastDeltas [2] = *deltaTime;
    }

    bool checkGLErrors (const char *func, const char * file, int line)
    {
      GLenum err;
      bool good = true;

      err = glGetError ();
      while (err != GL_NO_ERROR)
      {
        const char * message = nullptr;
        switch (err)
        {
        case GL_INVALID_ENUM:
          message = "Invalid enum";
          break;
        case GL_INVALID_VALUE:
          message = "Invalid value";
          break;
        case GL_INVALID_OPERATION:
          message = "Invalid operation";
          break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
          message = "Invalid framebuffer operation";
          break;
        case GL_OUT_OF_MEMORY:
          message = "Out of memory";
          break;
        default:
          message = "Unknown error";
        }

        std::cout << "OpenGL ERROR in [%s] %s(%d): %s\n", func;
        good = false;
        err = glGetError ();
      }
      return good;
    }
  }

}