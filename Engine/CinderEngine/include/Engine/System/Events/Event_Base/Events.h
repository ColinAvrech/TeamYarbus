/******************************************************************************/
/*!
\file   Events.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  contains all the enums for non-registered events.
*/
/******************************************************************************/


#pragma once

#define CollisionEventName(ObjId) std::string(COLLISION##ObjId )

#define DeclareEvent(Name) namespace Events { static const std::string Name(#Name); }
#define DeclareEventType(Name) namespace Events { namespace Types { static const std::string Name(#Name); }}


namespace Framework
{
  DeclareEventType(GAME)
  DeclareEventType(MOUSE)
  DeclareEventType(UPDATE)
  DeclareEventType(KEY)
  DeclareEventType(COLLISION)

  /*********************************************/
  // Input
  /*********************************************/

  //Mouse Input

  DeclareEvent(MOUSE_MOVE)
  DeclareEvent(MOUSE_LEFT)
  DeclareEvent(MOUSE_RIGHT)
  DeclareEvent(MOUSE_MIDDLE)
  DeclareEvent(MOUSE_SCROLLUP)
  DeclareEvent(MOUSE_SCROLLDOWN)

  // Key Modifiers

  DeclareEvent(KEY_CTRLIN)
  DeclareEvent(KEY_CTRLOUT)
  DeclareEvent(KEY_ALTIN)
  DeclareEvent(KEY_ALTOUT)
  DeclareEvent(KEY_SHIFTOUT)
  DeclareEvent(KEY_SHIFTIN)

  // Arrow Keys

  DeclareEvent(KEY_UP)
  DeclareEvent(KEY_DOWN)
  DeclareEvent(KEY_LEFT)
  DeclareEvent(KEY_RIGHT)

  //Control Keys
  
  DeclareEvent(KEY_BACKSPACE)
  DeclareEvent(KEY_TAB)
  DeclareEvent(KEY_ENTER)
  DeclareEvent(KEY_ESCAPE)
  DeclareEvent(KEY_SPACE)
  DeclareEvent(KEY_GRAVEACCENT)

  // Numbers

  DeclareEvent(KEY_0)
  DeclareEvent(KEY_1)
  DeclareEvent(KEY_2)
  DeclareEvent(KEY_3)
  DeclareEvent(KEY_4)
  DeclareEvent(KEY_5)
  DeclareEvent(KEY_6)
  DeclareEvent(KEY_7)
  DeclareEvent(KEY_8)
  DeclareEvent(KEY_9)

  // Letters: LowerCase || Upper Case

  DeclareEvent(KEY_A)
  DeclareEvent(KEY_B)
  DeclareEvent(KEY_C)
  DeclareEvent(KEY_D)
  DeclareEvent(KEY_E)
  DeclareEvent(KEY_F)
  DeclareEvent(KEY_G)
  DeclareEvent(KEY_H)
  DeclareEvent(KEY_I)
  DeclareEvent(KEY_J)
  DeclareEvent(KEY_K)
  DeclareEvent(KEY_L)
  DeclareEvent(KEY_M)
  DeclareEvent(KEY_N)
  DeclareEvent(KEY_O)
  DeclareEvent(KEY_P)
  DeclareEvent(KEY_Q)
  DeclareEvent(KEY_R)
  DeclareEvent(KEY_S)
  DeclareEvent(KEY_T)
  DeclareEvent(KEY_U)
  DeclareEvent(KEY_V)
  DeclareEvent(KEY_W)
  DeclareEvent(KEY_X)
  DeclareEvent(KEY_Y)
  DeclareEvent(KEY_Z)
  
  /*********************************************/
  // Misc Events Created by Systems always
  /*********************************************/

  // Windows System Related Events

  DeclareEvent(GAME_WINDOW)

  // Time Related Events

  DeclareEvent(LOGICUPDATE)

}
