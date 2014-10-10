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

#include "Common.h"


//EventPrefixes
#define UpdateEventPrefix 'U'
#define CollisionEventPrefix 'C'
#define MouseEventPrefix 'M'
#define KeyEventPrefix 'K'
#define GameEventPrefix 'G'

//#define DeclareEvent(Name) namespace Events { static const std::string Name(#Name); }
#define DeclareEvent(Name, String) namespace Events { static const std::string Name(#String); }

namespace Framework
{

  std::string CollisionEventName(unsigned GameObjectID);

  /*********************************************/
  // Input
  /*********************************************/

  //Mouse Input

  DeclareEvent(MOUSE_MOVE, MMOVE)
  DeclareEvent(MOUSE_LEFT, MLEFT)
  DeclareEvent(MOUSE_RIGHT, MRIGHT)
  DeclareEvent(MOUSE_MIDDLE, MMIDDLE)
  DeclareEvent(MOUSE_SCROLLUP, MSCROLLUP)
  DeclareEvent(MOUSE_SCROLLDOWN, MSCROLLDOWN)

  // Arrow Keys

  DeclareEvent(KEY_UP, KUP)
  DeclareEvent(KEY_DOWN, KDOWN)
  DeclareEvent(KEY_LEFT, KLEFT)
  DeclareEvent(KEY_RIGHT, KRIGHT)

  //Control Keys
  
  DeclareEvent(KEY_BACKSPACE, KBACKSPACE)
  DeclareEvent(KEY_TAB, KTAB)
  DeclareEvent(KEY_ENTER, KENTER)
  DeclareEvent(KEY_ESCAPE, KESCAPE)
  DeclareEvent(KEY_SPACE, KSPACE)
  DeclareEvent(KEY_GRAVEACCENT, KGRAVEACCENT)

  // Numbers

  DeclareEvent(KEY_0, K0)
  DeclareEvent(KEY_1, K1)
  DeclareEvent(KEY_2, K2)
  DeclareEvent(KEY_3, K3)
  DeclareEvent(KEY_4, K4)
  DeclareEvent(KEY_5, K5)
  DeclareEvent(KEY_6, K6)
  DeclareEvent(KEY_7, K7)
  DeclareEvent(KEY_8, K8)
  DeclareEvent(KEY_9, K9)

  // Letters: LowerCase || Upper Case

  DeclareEvent(KEY_A, KA)
  DeclareEvent(KEY_B, KB)
  DeclareEvent(KEY_C, KC)
  DeclareEvent(KEY_D, KD)
  DeclareEvent(KEY_E, KE)
  DeclareEvent(KEY_F, KF)
  DeclareEvent(KEY_G, KG)
  DeclareEvent(KEY_H, KH)
  DeclareEvent(KEY_I, KI)
  DeclareEvent(KEY_J, KJ)
  DeclareEvent(KEY_K, KK)
  DeclareEvent(KEY_L, KL)
  DeclareEvent(KEY_M, KM)
  DeclareEvent(KEY_N, KN)
  DeclareEvent(KEY_O, KO)
  DeclareEvent(KEY_P, KP)
  DeclareEvent(KEY_Q, KQ)
  DeclareEvent(KEY_R, KR)
  DeclareEvent(KEY_S, KS)
  DeclareEvent(KEY_T, KT)
  DeclareEvent(KEY_U, KU)
  DeclareEvent(KEY_V, KV)
  DeclareEvent(KEY_W, KW)
  DeclareEvent(KEY_X, KX)
  DeclareEvent(KEY_Y, KY)
  DeclareEvent(KEY_Z, KZ)

  // Called if any key is pressed
  DeclareEvent(KEY_ANY, KANY)

  /*********************************************/
  // Misc Events Created by Systems always
  /*********************************************/

  // Windows System Related Events

  //DeclareEvent(GAME_WINDOW)

  // Time Related Events

  DeclareEvent(UPDATEEVENT, U)

}
