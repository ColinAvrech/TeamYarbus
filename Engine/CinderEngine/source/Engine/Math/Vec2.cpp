/******************************************************************************/
/*!
\file   Vec2.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Class for all 2D math relating to position.
*/
/******************************************************************************/

#include "Vec2.h"

Vec2::Vec2(void)
: x_(0), y_(0)
{
}
Vec2::Vec2(double x, double y)
: x_(x), y_(y)
{
}
Vec2::Vec2(const Vec2& copy)
: x_(copy.x_), y_(copy.y_)
{
}

Vec2::~Vec2()
{
}


double Vec2::squarelength(void) const
{
  return (x_ * x_) + (y_ * y_);
}
double Vec2::length(void) const
{
  return sqrt(squarelength());
}

Vec2& Vec2::normalize(void)
{
  double veclength = length();
  x_ = x_ / veclength;
  y_ = y_ / veclength;
  return *this;
}

/*******************************
MEMBER OPERATORS
********************************/
Vec2 Vec2::operator+(const Vec2& rhs) const
{
  Vec2 temp(x_, y_);
  temp.x_ += rhs.x_;
  temp.y_ += rhs.y_;
  return temp;
}
Vec2& Vec2::operator+=(const Vec2& rhs)
{
  x_ += rhs.x_;
  y_ += rhs.y_;
  return *this;
}
Vec2 Vec2::operator-(const Vec2& rhs) const
{
  Vec2 temp(x_, y_);
  temp.x_ -= rhs.x_;
  temp.y_ -= rhs.y_;
  return temp;
}
Vec2& Vec2::operator-=(const Vec2& rhs)
{
  x_ -= rhs.x_;
  y_ -= rhs.y_;
  return *this;
}
Vec2& Vec2::operator=(const Vec2& rhs)
{
  x_ = rhs.x_;
  y_ = rhs.y_;
  return *this;
}

Vec2 Vec2::operator*(const double scale) const
{
  Vec2 temp(x_, y_);
  temp.x_ = temp.x_ * scale;
  temp.y_ = temp.y_ * scale;
  return temp;
}
Vec2& Vec2::operator*=(const double scale)
{
  x_ = x_ * scale;
  y_ = y_ * scale;
  return *this;
}


/********************************************
NON-MEMEBER FUNCTIONS
*********************************************/
Vec2 operator-(Vec2& rhs)
{
  Vec2 temp(-rhs.x_, -rhs.y_);
  return temp;
}
std::ostream& operator<<(std::ostream& os, const Vec2& rhs)
{
  os << "(" << rhs.x_ << "," << rhs.y_ << ")";
  return os;
}
