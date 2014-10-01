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

  DeclareEvent(KEY_UPARROW)
  DeclareEvent(KEY_DOWNARROW)
  DeclareEvent(KEY_LEFTARROW)
  DeclareEvent(KEY_RIGHTARROW)

  //Control Keys
  
  DeclareEvent(KEY_BACKSPACE)
  DeclareEvent(KEY_TAB)
  DeclareEvent(KEY_ENTER)
  DeclareEvent(KEY_ESCAPE)
  DeclareEvent(KEY_SPACE)
  DeclareEvent(KEY_GRAVEACCENT)

  // Numbers

  DeclareEvent(KEY_ZERO)
  DeclareEvent(KEY_ONE)
  DeclareEvent(KEY_TWO)
  DeclareEvent(KEY_THREE)
  DeclareEvent(KEY_FOUR)
  DeclareEvent(KEY_FIVE)
  DeclareEvent(KEY_SIX)
  DeclareEvent(KEY_SEVEN)
  DeclareEvent(KEY_EIGHT)
  DeclareEvent(KEY_NINE)

  // Letters: LowerCase || Upper Case

  DeclareEvent(KEY_A)  DeclareEvent(KEY_a)
  DeclareEvent(KEY_B)  DeclareEvent(KEY_b)
  DeclareEvent(KEY_C)  DeclareEvent(KEY_c)
  DeclareEvent(KEY_D)  DeclareEvent(KEY_d)
  DeclareEvent(KEY_E)  DeclareEvent(KEY_e)
  DeclareEvent(KEY_F)  DeclareEvent(KEY_f)
  DeclareEvent(KEY_G)  DeclareEvent(KEY_g)
  DeclareEvent(KEY_H)  DeclareEvent(KEY_h)
  DeclareEvent(KEY_I)  DeclareEvent(KEY_i)
  DeclareEvent(KEY_J)  DeclareEvent(KEY_j)
  DeclareEvent(KEY_K)  DeclareEvent(KEY_k)
  DeclareEvent(KEY_L)  DeclareEvent(KEY_l)
  DeclareEvent(KEY_M)  DeclareEvent(KEY_m)
  DeclareEvent(KEY_N)  DeclareEvent(KEY_n)
  DeclareEvent(KEY_O)  DeclareEvent(KEY_o)
  DeclareEvent(KEY_P)  DeclareEvent(KEY_p)
  DeclareEvent(KEY_Q)  DeclareEvent(KEY_q)
  DeclareEvent(KEY_R)  DeclareEvent(KEY_r)
  DeclareEvent(KEY_S)  DeclareEvent(KEY_s)
  DeclareEvent(KEY_T)  DeclareEvent(KEY_t)
  DeclareEvent(KEY_U)  DeclareEvent(KEY_u)
  DeclareEvent(KEY_V)  DeclareEvent(KEY_v)
  DeclareEvent(KEY_W)  DeclareEvent(KEY_w)
  DeclareEvent(KEY_X)  DeclareEvent(KEY_x)
  DeclareEvent(KEY_Y)  DeclareEvent(KEY_y)
  DeclareEvent(KEY_Z)  DeclareEvent(KEY_z)
  
  /*********************************************/
  // Misc Events Created by Systems always
  /*********************************************/

  // Windows System Related Events

  DeclareEvent(GAME_WINDOW)

  // Time Related Events

  DeclareEvent(LOGICUPDATE)

}
