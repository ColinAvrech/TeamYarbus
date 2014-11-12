/******************************************************************************/
/*!
\file   Grid2D.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  2D grid for thermodynamics and fluid solver
*/
/******************************************************************************/
#pragma once

#include "Common.h"
#include "glm.hpp"

namespace Framework
{
  namespace Physics
  {
    /*---------------------------------------------------------------------------
    // Class
    ---------------------------------------------------------------------------*/
    template <class T>
    class Grid2D
    {
    public:
      Grid2D(const unsigned int sizeX, const unsigned int sizeY);
      Grid2D(const Grid2D &g2);
      ~Grid2D();

      T Get(const unsigned int x, const unsigned int y) const {
        return _data[y * _width + x];
      }

      void Set(const unsigned int x, const unsigned int y, const T value){
        _data[y * _width + x] = value;
      }

      T* GetArray(){
        return _data;
      }

      glm::vec2 getSize() const {
        return glm::vec2(_width, _height);
      }

      unsigned int getLength() const{
        return _lenght;
      }

      void fill(T value);

      Grid2D<T>& operator+=(T rhs);
      Grid2D<T>& operator+=(Grid2D<T> &rhs);
      Grid2D<T>& operator*=(T rhs);

      //Load ?!?
      //Save ?!?

    private:
      T* _data;
      const unsigned int _width, _height;
      const unsigned int _length;
    };
  } //Physics
} //Framework

#include "grid2d.cpp"
