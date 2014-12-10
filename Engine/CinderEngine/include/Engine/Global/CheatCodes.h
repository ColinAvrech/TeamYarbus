/******************************************************************************/
/*!
\file   CheatCodes.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Cheats!
*/
/******************************************************************************/

#pragma once

namespace Framework
{
  class KeyEvent;

  namespace Cheats
  {
    void InitializeCheats();

    void CheckToEnableCheats(KeyEvent * e);

  }// namespace Cheats
}// namespace Framework
