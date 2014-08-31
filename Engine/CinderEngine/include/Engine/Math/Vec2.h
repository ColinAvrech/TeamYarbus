
#pragma once
#include <iostream>

/** 
  \brief Vec2 is used for Position, Velocity, and Acceleration
  of objects. It has Lots of utility and should be used for the
  physics and graphics parts of our game.
*/
class Vec2
{
public:
  Vec2(void);
  Vec2(double x, double y);
  Vec2(const Vec2& copy);
  ~Vec2();

  double squarelength(void) const;
  double length(void) const;

  Vec2& normalize(void);

  Vec2 operator+(const Vec2& rhs) const;
  Vec2& operator+=(const Vec2& rhs);
  Vec2 operator-(const Vec2& rhs) const;
  Vec2& operator-=(const Vec2& rhs);
  Vec2& operator=(const Vec2& rhs);

  Vec2 operator*(const double scale) const;
  Vec2& operator*=(const double scale);

  double x_;
  double y_;

private:
};


/**
  \brief Output operator for Vec2 class.
*/
std::ostream& operator<<(std::ostream& os, const Vec2& rhs);