/******************************************************************************/
/*!
\file   Test_Mouse.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "Test_Mouse.h"
#include "Fluid_Physics_Particle_Array.h"

namespace Framework
{
#define LOCAL_Descaling_factor 30.0f


  void Test_Mouse::ApplyMouseBehaviour (
    Fluid_Physics_Object & game_entity,
    const REAL32 max_dis_attractor
    )
  {
    REAL32
      attractor_distance_from_particle_squared;

    attractor_distance_from_particle_squared = (game_entity.GetPosition () - MouseAttractorVector).GetSquareLength ();

    if (IsAttracting)
    {
      if (attractor_distance_from_particle_squared < max_dis_attractor * max_dis_attractor)
      {
        game_entity.SetForce (
          game_entity.GetForce ()
          - (game_entity.GetPosition ()
          - MouseAttractorVector) / LOCAL_Descaling_factor
          );
      }
    }

    if (IsRepelling)
    {
      if (attractor_distance_from_particle_squared < max_dis_attractor * max_dis_attractor)
      {
        game_entity.SetForce (
          game_entity.GetForce ()
          + (game_entity.GetPosition ()
          - MouseAttractorVector) / LOCAL_Descaling_factor
          );
      }
    }
  }
}