/*****************************************************************************/
/*!
\file   Affine.cpp
\author Manas Sudhir Kulkarni
\par    Course        : GAM200
\brief
This file contains the implementation of the following functions

Functions include:

  # Constructors
    - Hcords - Default Constructor
    - Hcords - Non-Default Constructor
    - Point  - Default Constructor
    - Point  - Non-Default Constructor
    - Vector - Default Constructor
    - Vector - Non-Default Constructor
    - Affine - Default Constructor
    - Affine - Non-Default Constructor

  # Math Functions
    - Normalize
    - dot
    - abs
    - Rot
    - Trans
    - Scale       - Uniform
    - Scale       - Non-Uniform
    - operator +
    - operator -
    - operator *

*/
/*****************************************************************************/

#include "Affine.h"

// Default constructor
Hcoords::Hcoords (void)
{
  x = 0;
  y = 0;
  w = 0;
}

// Non Default Constructor
Hcoords::Hcoords (float X, float Y, float W)
{
  x = X;
  y = Y;
  w = W;
}

// Default constructor
Point::Point (void)
{
  x = 0;
  y = 0;
  w = 1;
}

// Non Default Constructor
Point::Point (float X, float Y)
{
  x = X;
  y = Y;
  w = 1;
}

// Default Constructor
Vector::Vector (void)
{
  x = 0;
  y = 0;
  w = 0;
}

// Non Default Constructor
Vector::Vector (float X, float Y)
{
  x = X;
  y = Y;
  w = 0;
}

// Default Constructor
Affine::Affine (void)
{
  for (int i = 0; i < sizeof (matrix) / sizeof (matrix [0]); ++i)
  {
    for (int j = 0; j < sizeof (matrix [0]) / sizeof (matrix [0][0]); ++j)
    {
      matrix [i][j] = 0;
    }
  }
}

// Non Default Constructor
Affine::Affine (const Vector& Lx, const Vector& Ly, const Point& disp)
{
  matrix [0][0] = Lx.x;
  matrix [1][0] = Lx.y;
  matrix [2][0] = Lx.w;

  matrix [0][1] = Ly.x;
  matrix [1][1] = Ly.y;
  matrix [2][1] = Ly.w;

  matrix [0][2] = disp.x;
  matrix [1][2] = disp.y;
  matrix [2][2] = disp.w;
}

// Normalizes the Vector
bool Vector::Normalize (void)
{
  if (Near (x, 0) && Near (y, 0))
  {
    return false;
  }
  
  x = cosf (x);
  y = sinf (y);
  return true;
}

// Returns dot product of u and v
float dot (const Vector& u, const Vector& v)
{
  return (u.x * v.x) + (u.y * v.y);
}

// Returns magnitude of vector v
float abs (const Vector& v)
{
  return sqrtf ((v.x + v.y) * (v.x + v.y));
}

// Returns the affine transformation for rotation by angle t
Affine Rot (float t)
{
  return Affine (
    Vector (cosf (t), sinf (t)),
    Vector (-sinf (t), cosf (t)),
    Point (0, 0)
    );
}

// Returns the affine transformation for translation by vector v
Affine Trans (const Vector& v)
{
  return Affine (
    Vector (1, 0),
    Vector (0, 1),
    Point (v.x, v.y)
    );
}

// Returns the affine transformation for uniform scaling by r
Affine Scale (float r)
{
  return Affine (
    Vector (r, 0),
    Vector (0, r),
    Point (0, 0)
    );
}

// Returns the affine transformation for non-uniform scaling by rx, ry
Affine Scale (float rx, float ry)
{
  return Affine (
    Vector (rx, 0),
    Vector (0, ry),
    Point (0, 0)
    );
}

// Returns sum of u and v
Hcoords operator+ (const Hcoords& u, const Hcoords& v)
{
  return Hcoords (u.x + v.x, u.y + v.y, u.w + v.w);
}

// Returns the difference of u and v
Hcoords operator- (const Hcoords& u, const Hcoords& v)
{
  return Hcoords (u.x - v.x, u.y - v.y, u.w - v.w);
}

// Returns negated value of v
Hcoords operator- (const Hcoords& v)
{
  return Hcoords (-v.x, -v.y, v.w);
}

// Returns product of r and v
Hcoords operator* (float r, const Hcoords& v)
{
  return Hcoords (v.x * r, v.y * r, v.w * r);
}

// Returns result of applying the affine transformation A to vector v
Hcoords operator* (const Affine& A, const Hcoords& v)
{
  return Hcoords (
    A.matrix [0][0] * v.x + A.matrix [0][1] * v.y + A.matrix [0][2] * v.w,
    A.matrix [1][0] * v.x + A.matrix [1][1] * v.y + A.matrix [1][2] * v.w,
    A.matrix [2][0] * v.x + A.matrix [2][1] * v.y + A.matrix [2][2] * v.w
    );
}

// Returns the composition of A and B
Affine operator* (const Affine& A, const Affine& B)
{
  return Affine (
    Vector (A.matrix [0][0] * B.matrix [0][0] + A.matrix [0][1] * B.matrix [1][0] + A.matrix [0][2] * B.matrix [2][0],
            A.matrix [1][0] * B.matrix [0][0] + A.matrix [1][1] * B.matrix [1][0] + A.matrix [1][2] * B.matrix [2][0]),

    Vector (A.matrix [0][0] * B.matrix [0][1] + A.matrix [0][1] * B.matrix [1][1] + A.matrix [0][2] * B.matrix [2][1],
            A.matrix [1][0] * B.matrix [0][1] + A.matrix [1][1] * B.matrix [1][1] + A.matrix [1][2] * B.matrix [2][1]),

    Point  (A.matrix [0][0] * B.matrix [0][2] + A.matrix [0][1] * B.matrix [1][2] + A.matrix [0][2] * B.matrix [2][2],
            A.matrix [1][0] * B.matrix [0][2] + A.matrix [1][1] * B.matrix [1][2] + A.matrix [1][2] * B.matrix [2][2])
    );
}


// Helper function to calculate determinant of 3X3 Matrix
static float Determinant (const Affine& A)
{
  return ( A.matrix [0][0] * (A.matrix [1][1] * A.matrix [2][2] - A.matrix [1][2] * A.matrix [2][1]) -
           A.matrix [0][1] * (A.matrix [1][0] * A.matrix [2][2] - A.matrix [1][2] * A.matrix [2][0]) +
           A.matrix [0][2] * (A.matrix [1][0] * A.matrix [2][1] - A.matrix [1][1] * A.matrix [2][0]));
}


// Calculates the inverse of the input affine transformation
Affine Inverse(const Affine& A)
{
  float D = 1 / Determinant (A);

  return Affine (Vector ( D * A.matrix [1][1], -D * A.matrix [1][0]),
                 Vector (-D * A.matrix [0][1],  D * A.matrix [0][0]),
                 Point  (0, 0)) * 
         Affine (Vector (1, 0), Vector (0, 1), Point (-A.matrix [0][2], -A.matrix [1][2]));
}
