/******************************************************************************/
/*!
\file   GridManager.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Manages allocation and access of Grid2D for thermo.
*/
/******************************************************************************/
#pragma once

#define GRID_X_SIZE 64

namespace Framework
{
  namespace Physics
  {
    /*---------------------------------------------------------------------------
    // Class
    ---------------------------------------------------------------------------*/
    template <class T>
    class GridManager
    {
    public:
      //constructor
      GridManager(int x, int y);
      //Destructor
      ~GridManager();

    private:
      int Map_X, Map_Y;
      //data
      //x & y are the start and end indices. z is the vertical offset of the adjacent grid
      std::list<std::pair<glm::ivec3, Grid2D<T>>>data;

      void allocate(const unsigned int sizeX, const unsigned int sizeY, const glm::ivec3 &offset)
      {
        Grid2D<T> newGrid;
        newGrid.allocate(sizeX, sizeY);
        data.push_back(std::make_pair(offset, newGrid));
      }
    };

    //Implementation
    template <typename T>
    GridManager<T>::GridManager(int x, int y) : Map_X(x), Map_Y(y)
    {
      int required_grids = Map_X / GRID_X_SIZE;
      for (int i = 0, unsigned int start = 0; i < required_grids; ++i, start += GRID_X_SIZE)
      {
        glm::ivec3 ind(start, start + GRID_X_SIZE - 1, 0);
        allocate(GRID_X_SIZE, Map_Y, ind);
      }
    }

  } //namespace physics
} //namespace framework