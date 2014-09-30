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



  //Old Event enumerations
  /*
  namespace Events
  {
    enum Types
    {
      Input,
      Engine,
      Graphics,
      Windows,
      Audio,
      Physics,
      Script,
    };

    enum InputKeys
    {
      //Mouse
      //LEFTMOUSE = 1,
      //RIGHTMOUSE = 2,
      //MIDDLEMOUSE = 3,
      //LEFTMOUSEBUTTON = 4,
      //RIGHTMOUSEBUTTON = 5,
      //MOUSESCROLLUP = 6,
      //MOUSESCROLLDOWN = 7,

      //BACKSPACE = 8,
      //TAB = 9,
      //ENTER = 10,

      //CTRLIN = 11,
      //CTRLOUT = 12,
      //ALTIN = 13,
      //ALTOUT = 14,
      //SHIFTOUT = 15,
      //SHIFTIN = 16,

      //Arrows
      //UPARROW = 17,
      //DOWNARROW = 18,
      //LEFTARROW = 19,
      //RIGHTARROW = 20,
      
      //ESCAPE = 27,
      //SPACE = 32,

      //Numbers
      //ZERO = 48,
      //ONE = 49,
      //TWO = 50,
      //THREE = 51,
      //FOUR = 52,
      //FIVE = 53,
      //SIX = 54,
      //SEVEN = 55,
      //EIGHT = 56,
      //NINE = 57,

      //Letters
      //GRAVEACCENT = 96,
      //A = 65, a = 97,
      //B = 66, b = 98,
      //C = 67, c = 99,
      //D = 68, d = 100,
      //E = 69, e = 101,
      //F = 70, f = 102,
      //G = 71, g = 103,
      //H = 72, h = 104,
      //I = 73, i = 105,
      //J = 74, j = 106,
      //K = 75, k = 107,
      //L = 76, l = 108,
      //M = 77, m = 109,
      //N = 78, n = 110,
      //O = 79, o = 111,
      //P = 80, p = 112,
      //Q = 81, q = 113,
      //R = 82, r = 114,
      //S = 83, s = 115,
      //T = 84, t = 116,
      //U = 85, u = 117,
      //V = 86, v = 118,
      //W = 87, w = 119,
      //X = 88, x = 120,
      //Y = 89, y = 121,
      //Z = 90, z = 122,
    };

  }
  */

}
