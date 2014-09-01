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

namespace Framework
{
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
      LEFTMOUSE = 1,
      RIGHTMOUSE = 2,
      MIDDLEMOUSE = 3,
      LEFTMOUSEBUTTON = 4,
      RIGHTMOUSEBUTTON = 5,
      MOUSESCROLLUP = 6,
      MOUSESCROLLDOWN = 7,

      BACKSPACE = 8,
      TAB = 9,
      ENTER = 10,

      CTRLIN = 11,
      CTRLOUT = 12,
      ALTIN = 13,
      ALTOUT = 14,
      SHIFTOUT = 15,
      SHIFTIN = 16,

      //Arrows
      UPARROW = 17,
      DOWNARROW = 18,
      LEFTARROW = 19,
      RIGHTARROW = 20,
      
      ESCAPE = 27,
      SPACE = 32,

      //Numbers
      ZERO = 48,
      ONE = 49,
      TWO = 50,
      THREE = 51,
      FOUR = 52,
      FIVE = 53,
      SIX = 54,
      SEVEN = 55,
      EIGHT = 56,
      NINE = 57,

      //Letters
      GRAVEACCENT = 96,
      A = 65, a = 97,
      B = 66, b = 98,
      C = 67, c = 99,
      D = 68, d = 100,
      E = 69, e = 101,
      F = 70, f = 102,
      G = 71, g = 103,
      H = 72, h = 104,
      I = 73, i = 105,
      J = 74, j = 106,
      K = 75, k = 107,
      L = 76, l = 108,
      M = 77, m = 109,
      N = 78, n = 110,
      O = 79, o = 111,
      P = 80, p = 112,
      Q = 81, q = 113,
      R = 82, r = 114,
      S = 83, s = 115,
      T = 84, t = 116,
      U = 85, u = 117,
      V = 86, v = 118,
      W = 87, w = 119,
      X = 88, x = 120,
      Y = 89, y = 121,
      Z = 90, z = 122,
    };

  }


}

#include "BaseEvent.h"
#include "InputEvent.h"
