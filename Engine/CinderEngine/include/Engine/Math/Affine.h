
#ifndef CS200_AFFINE_H
#define CS200_AFFINE_H

#include <cmath>
#include <cassert>


struct Hcoords {
  float x, y, w;
  Hcoords(void);
  Hcoords(float X, float Y, float W);
  static bool Near(float x, float y) { return std::abs(x-y) < 1e-5f; }
};


struct Point : Hcoords {
  Point(void);
  Point(float X, float Y);
  Point(const Hcoords& v) : Hcoords(v) { assert(Near(w,1)); }
};


struct Vector : Hcoords {
  Vector(void);
  Vector(float X, float Y);
  Vector(const Hcoords& v) : Hcoords(v) { assert(Near(w,0)); }
  bool Normalize(void);
};


struct Affine {
  float matrix[3][3];
  Affine(void);
  Affine(const Vector& Lx, const Vector& Ly, const Point& disp);
};


Hcoords operator+(const Hcoords& u, const Hcoords& v);
Hcoords operator-(const Hcoords& u, const Hcoords& v);
Hcoords operator-(const Hcoords& v);
Hcoords operator*(float r, const Hcoords& v);
Hcoords operator*(const Affine& A, const Hcoords& v);
Affine operator*(const Affine& A, const Affine& B);
float dot(const Vector& u, const Vector& v);
float abs(const Vector& v);
Affine Rot(float t);
Affine Trans(const Vector& v);
Affine Scale(float r);
Affine Scale(float rx, float ry);
Affine Inverse(const Affine& A);


#endif

