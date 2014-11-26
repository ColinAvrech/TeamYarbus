/******************************************************************************/
/*!
\file   BaseSystem.h
\author Micah
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains the interface class for all systems.
*/
/******************************************************************************/


//!makes sure this is only included once
#pragma once

#include "Common.h"

#ifdef _DEBUG
using namespace std::chrono;
#endif

namespace Framework
{
  /*!System is a pure virtual base class (an interface) which is
  inherited by other classes.*/
  class BaseSystem
  {
  public:
	ZilchDeclareBaseType(BaseSystem, Zilch::TypeCopyMode::ReferenceType);

    //!All systems are updated every frame.
    virtual void Update(const double& dt) = 0;

    virtual bool UpdatesOnPaused() { return m_ShouldUpdateOnPaused; }

    //!All systems need a name.
    virtual const string GetName() = 0;

    //!Initialize the system.
    virtual bool Initialize(){ return false; }

    //!All systems need a virtual destrutor to have their distructor Called
    virtual ~BaseSystem(){}

  private:
    bool m_ShouldUpdateOnPaused = true;

#ifdef _DEBUG
  private:
    //!The time taken by the update 
    double _system_dt = 0;
    //!The average time taken by the update
    double _system_dt_ave = 0;

    //!time points to measure
    high_resolution_clock::time_point _start_tp;
    high_resolution_clock::time_point _end_tp;

    //!Call at the begining of the update
    void StartSystem_dt(void)
    {
      _start_tp = high_resolution_clock::now();
    }
    //!Call at the end of the update function
    void EndSystem_dt(void)
    {
      _end_tp = high_resolution_clock::now();

      //!Get the number of ticks from teach time_point
      long long start_ticks = _start_tp.time_since_epoch().count();
      long long end_ticks = _end_tp.time_since_epoch().count();

      //!The length of time between each tick. the Period.
      const double period = high_resolution_clock::period::num
        / static_cast<double>(high_resolution_clock::period::den);

      /*!Calculate the delta time by multiplying the number of ticks by
      the length of time per tick (Period)  */
      _system_dt = static_cast<double>(end_ticks - start_ticks) * period;

      /*!The average system dt is affected more by recent deltas*/
      _system_dt_ave = (11 * _system_dt_ave + _system_dt) / 12;
    }
#endif
  };

}