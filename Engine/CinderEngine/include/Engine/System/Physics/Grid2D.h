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
      Grid2D (){ _data = nullptr; }

      explicit Grid2D(const Grid2D<T> &g2) = delete; // No Copies

      ~Grid2D ()
      {
        if (_data != nullptr)
        {
          delete [] _data;
          _data = nullptr;
        }
      }

      void allocate (const unsigned int sizeX, const unsigned int sizeY)
      {
        _width = sizeX;
        _height = sizeY;
        _length = sizeX * sizeY;
        _data = new T [_length];
      }

      T Get(const unsigned int x, const unsigned int y) const
      {
        return _data[y * _width + x];
      }

      void Set(const unsigned int x, const unsigned int y, const T value)
      {
        _data[y * _width + x] = value;
      }

      T* GetArray()
      {
        return _data;
      }

      vec2 getSize() const
      {
        return vec2(_width, _height);
      }

      unsigned int getLength() const
      {
        return _length;
      }

      void fill (T value)
      {
        for (unsigned int i = 0; i < _length; ++i)
          _data [i] = value;
      }

      void clean ()
      {
        if (_data != nullptr)
        {
          delete [] _data;
          _data = nullptr;
        }
      }

      Grid2D<T>& operator+=(T rhs)
      {
        for (unsigned int i = 0; i < _length; ++i)
          _data [i] += rhs;
        return *this;
      }

      Grid2D<T>& operator+=(Grid2D<T> &rhs)
      {
        for (unsigned int i = 0; i < _length; ++i)
          _data [i] += rhs._data [i];
        return *this;
      }

      Grid2D<T>& operator*=(T rhs)
      {
        for (unsigned int i = 0; i < _length; ++i)
          _data [i] *= rhs;
        return *this;
      }

      void addAndMultiply(const glm::ivec2 &start, const glm::ivec2 &end, const Grid2D<T> &add, const float v)
      {
        for (unsigned int i = start.x; i < end.x; ++i)
        {
          for (unsigned int j = start.y; j < end.y; ++j)
          {
            T value = this->Get(i, j);
            value += (add.Get(i, j) * v);
            this->Set(i, j, value);
          }
        }
      }

      //Load ?!?
      //Save ?!?

    private:
      T* _data;
      unsigned int _width, _height;
      unsigned int _length;
    };
  } //Physics
} //Framework

#include "grid2d.cpp"
