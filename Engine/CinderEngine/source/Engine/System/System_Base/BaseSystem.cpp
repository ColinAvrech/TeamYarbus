/*!

/auther Micah
/file System.h

/brief System is a 100% virtual function. This code is ONLY for debug
pruposes. This is debug code!
*/

#ifdef _DEBUG

#include "BaseSystem.h"

namespace Framework
{
  void BaseSystem::StartSystem_dt(void)
  {
    _start_tp = high_resolution_clock::now();
  }

  void BaseSystem::EndSystem_dt(void)
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
}



#endif