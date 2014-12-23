/******************************************************************************/
/*!
\file   TimeQuery.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _GRAPHICS_TIME_H
#define _GRAPHICS_TIME_H


namespace Framework
{

  class CpuTimeQuery
  {
  public:
    double m_time;

  protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_cpuTimePointStart;

  public:
    void begin ()
    {
      m_cpuTimePointStart = std::chrono::high_resolution_clock::now ();
    }

    void end ()
    {
      auto diff = std::chrono::high_resolution_clock::now () - m_cpuTimePointStart;
      auto mili = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count ();
      m_time = 0.5*(m_time + (double) mili);
    }
  };

  /** simple wrapper for the GL_TIME_QUERY from OpenGL */
  class GpuTimerQuery
  {
  public:
    enum class WaitOption
    {
      WaitForResults,
      DoNotWaitForResults
    };

  private:
    GLuint   mQuery;
    GLuint64 mWholeTime;
    GLuint   mCounter;
    double   mTimes [3];
    double   mTime;
  public:
    GpuTimerQuery ();
    ~GpuTimerQuery ();

    /// inits the query, deletes the query if it is created
    void init ();

    inline void begin ();

    inline void end ();

    void updateResults (WaitOption wait);

    inline void resetTime ();

    inline double getAverageTime () const;

    inline double &getTime ();
  private:
    void deleteQuery ();
  };

  ///////////////////////////////////////////////////////////////////////////////
  // inline:

  ///////////////////////////////////////////////////////////////////////////////
  void GpuTimerQuery::begin ()
  {
    glBeginQuery (GL_TIME_ELAPSED, mQuery);
  }

  ///////////////////////////////////////////////////////////////////////////////
  void GpuTimerQuery::end ()
  {
    glEndQuery (GL_TIME_ELAPSED);
  }

  ///////////////////////////////////////////////////////////////////////////////
  inline void GpuTimerQuery::resetTime ()
  {
    mWholeTime = 0;
    mCounter = 0;
    mTimes [0] = mTimes [1] = mTimes [2] = 0.0;
    mTime = 0.0;
  }

  ///////////////////////////////////////////////////////////////////////////////
  inline double GpuTimerQuery::getAverageTime () const
  {
    double avg = mWholeTime / (double) mCounter;
    avg /= 1000000.0;
    return avg;
  }

  ///////////////////////////////////////////////////////////////////////////////
  inline double &GpuTimerQuery::getTime ()
  {
    return mTime;
  }

  namespace Utils
  {
    bool initGL (bool vsync);

    void calculateFps (float *fps);

    void updateTimer (double *deltaTime, double *appTime, const double MAX_REFRESH_TIME = 1.0 / 60.0);

    bool checkGLErrors (const char *func, const char * file, int line);
  } // namespce utils	
}

#endif