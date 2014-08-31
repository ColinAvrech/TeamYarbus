/******************************************************************************/
/*!
\file   Template.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The Base system which all System inherit and are called by.
*/
/******************************************************************************/

//!makes sure this is only included once
#pragma once

#include <string>
#ifdef _DEBUG
#include <chrono> //! high_resolution_clock
using namespace std::chrono;
#endif

namespace Framework
{
  /*!System is a pure virtual base class (an interface) which is
  inherited by other classes.*/
  class BaseSystem
  {
  public:

    //!All systems are updated every frame.
    virtual void Update(const double dt) = 0;

    //!All systems need a name.
    virtual const std::string GetName() = 0;

    //!Initialize the system.
    virtual bool Initialize(){ return false; };

    //!All systems need a virtual destrutor to have their distructor Called
    virtual ~BaseSystem(){}

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
    void StartSystem_dt(void);
    //!Call at the end of the update function
    void EndSystem_dt(void);
#endif
  };
}