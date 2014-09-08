/******************************************************************************/
/*!
\file   SpaceMap.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The space map is a grid that approximately maps the the level layout 
        into separate regions with uniform physics properties. This facilitates
        the computation of the thermodynamics system.
*/
/******************************************************************************/

/*
All the crap in this file will require further research in thermodynamics
before logic is implemented.
*/

#include "Common.h"

namespace Framework
{
  namespace PhysicsSystem
  {
    class SpaceMap
    {
    public:
      //Constructor

      //Destructor

      //Initialize

      //Get movement vector (this will call the cell's getter function)

      //Get cell temperature (this will call the cell's getter function)

    private:
      //----------------------------------------------------------------------
      //Cell Class
      //----------------------------------------------------------------------
      class cell
      {
      public:
        //Constructor

        //Destructor

        //Initialize (Map level objects that are in this cell)

        //Get movement vector

        //Get temperature (may not be necessary)

        //Get particle count

        //Set particle count

        //Get ignition particle count

        //Check collision

      private:
        //(vec2) air current vector

        //array of ignition particles (these will affect the burn rate)

        //array of game objects within the space of this cell
        //(Will probably use the average values in case of conflicts)

        //current temperature

        //
        //-------------------------------------------------------------------
        //Update methods
        //-------------------------------------------------------------------
        //Calculate air current within the cell's space



      };
      //---------------------------------------------------------------------
      //2D array of cells

      //x offset (used for level streaming)

      //y offset (used for level streaming)

      //-----------------------------------------------------------------------
      //Update methods
      //-----------------------------------------------------------------------
      //Heat diffusion

      //Brownian motion (move particles from cell to cell)

    };
  }
}

