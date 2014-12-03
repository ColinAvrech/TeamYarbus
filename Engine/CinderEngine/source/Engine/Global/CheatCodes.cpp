/******************************************************************************/
/*!
\file   CheatCodes.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Cheats!
*/
/******************************************************************************/

#include "CheatCodes.h"
#include "EventSystem.h"
#include "KeyEvent.h"
#include "GLFW3.h"

namespace Framework
{
  namespace Cheats
  {
    bool PlayerIsInvincible;

    void InitializeCheats()
    {
      PlayerIsInvincible = false;
      EVENTSYSTEM->gConnect<KeyEvent>(Events::KEY_ANY, &CheckToEnableCheats);
    }

    void CheckToEnableCheats(KeyEvent * e)
    {
      // Toggle Invincibility:   Shift + I
      if (e->SHIFTPressed && e->KeyDown && e->KeyValue == GLFW_KEY_I)
      {
        std::cout << "Toggle Invincibility Cheat!" << std::endl;

        if (PlayerIsInvincible)
        {
          PlayerIsInvincible = false;
        }
        else
        {
          PlayerIsInvincible = true;
        }
      }

      // Goto NextLevel: Shift + N
      if (e->SHIFTPressed && e->KeyDown && e->KeyValue == GLFW_KEY_N)
      {
        std::cout << "Goto NextLevel Cheat!" << std::endl;

      }

    }
  }// namespace Cheats  
}// namespace Framework
