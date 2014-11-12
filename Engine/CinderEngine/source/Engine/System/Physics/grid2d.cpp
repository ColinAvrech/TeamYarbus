/******************************************************************************/
/*!
\file   grid2d.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Implementation of the template grid2D class
*/
/******************************************************************************/

#include "Grid2D.h"

namespace Framework
{
  namespace Physics
  {
    template <class T>
    Grid2D<T>::Grid2D(const unsigned int sizeX, const unsigned int sizeY) :
      _width(sizeX), _height(sizeY), _length(sizeX * sizeY)
    {
      _data = new T[_length];
    }

    template <class T>
    Grid2D<T>::Grid2D(const Grid2D<T> &g2) :
      _width(g2._width), _height(g2._height), _length(g2._length)
    {
      _data = new T[_length];
      for (unsigned int i = 0; i < _length; ++i)
        _data[i] = g2._data[i];
    }

    template <class T>
    Grid2D<T>::~Grid2D(){
      delete[] _data;
    }

    template <class T>
    void Grid2D<T>::fill(T value)
    {
      for (unsigned int i = 0; i < _length; ++i)
        _data[i] = value;
    }

    template <class T>
    Grid2D<T>& Grid2D<T>::operator+= (T rhs)
    {
      for (unsigned int i = 0; i < _length; ++i)
        _data[i] += rhs;
      return *this;
    }

    template <class T>
    Grid2D<T>& Grid2D<T>::operator+= (Grid2D<T> &rhs)
    {
      for (unsigned int i = 0; i < _length; ++i)
        _data[i] += rhs._data[i];
      return *this;
    }

    template <class T>
    Grid2D<T>& Grid2D<T>::operator*= (T rhs)
    {
      for (unsigned int i = 0; i < _length; ++i)
        _data[i] *= rhs;
      return *this;
    }



  } //Physics
} //Framework
