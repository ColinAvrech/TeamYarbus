/*****************************************************************************/
/*!
\file   Trees.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate trees of different types.
*/
/*****************************************************************************/

#pragma once
#include "BaseSystem.h"

namespace Framework
{
  namespace Procedural
  {
    enum Flora
    {
      GRASS,
      WEEDS,
      AQUATIC,
      SHORE_LARGE,
      LAND_LARGE,
      SHORE_SMALL,
      LAND_SMALL
    };

    ////////////////////////////////////////////////////////////
    //      ECO SYSTEM
    ////////////////////////////////////////////////////////////
    class EcoSystem : public BaseSystem
    {
    public:

      /*-----------------------------------------------------------------------
      // Constructors
      -----------------------------------------------------------------------*/
#pragma region Constructors

      EcoSystem();

#pragma endregion
      /*-----------------------------------------------------------------------
      // Static Public Variables
      -----------------------------------------------------------------------*/
#pragma region Static Public Variables



#pragma endregion


      /*-----------------------------------------------------------------------
      // Static Public Functions
      -----------------------------------------------------------------------*/
#pragma region Static Public Functions

#pragma endregion


      /*-----------------------------------------------------------------------
      // Destructor
      -----------------------------------------------------------------------*/
#pragma region Destructors
      ~EcoSystem();

#pragma endregion


      /*-----------------------------------------------------------------------
      // Public Variables
      -----------------------------------------------------------------------*/
#pragma region Public Variables

#pragma endregion

      /*---------------------------------------------------------------------*/
      // Public Structs
      /*---------------------------------------------------------------------*/
#pragma region Public Structs

#pragma endregion

      /*-----------------------------------------------------------------------
      // Public Functions
      -----------------------------------------------------------------------*/
#pragma region Public Functions
      //!Initialize the system
      bool Initialize();

      // Called every frame
      void Update(const double dt);

    private:

      /*-----------------------------------------------------------------------
      // Private Variables
      -----------------------------------------------------------------------*/
#pragma region Private Variables
     
#pragma endregion


      /*-----------------------------------------------------------------------
      // Private Structs
      -----------------------------------------------------------------------*/
#pragma region Private Structs

      
#pragma endregion

      /*-----------------------------------------------------------------------
      // Private Functions
      -----------------------------------------------------------------------*/
#pragma region Private Functions
      
#pragma endregion


      /*-----------------------------------------------------------------------
      // Static Functions
      -----------------------------------------------------------------------*/
#pragma region Static Functions

#pragma endregion

    }; //class thermodynamics


  } //Procedural
}  //Framework

