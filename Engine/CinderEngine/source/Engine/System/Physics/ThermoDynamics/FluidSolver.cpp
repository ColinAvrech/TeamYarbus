/******************************************************************************/
/*!
\file   FluidSolver.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  NOT USED IN F2014 build
*/
/******************************************************************************/

#include <Precompiled.h>
#include "FluidSolver.h"

#define SWAP(x0,x) {auto *tmp = x0; x0 = x; x = tmp;}
#define NOT_OBSTACLE(x, y) y - y_offset[x] > 0 && Obstacles->Get(x, y) == AIR
#define OFFSET(y) y_offset[i] - y_offset[y]

namespace Framework
{
  namespace Physics
  {
    // Constructor
    FluidSolver::FluidSolver()
    {
      /*_u = nullptr;
      _u_prev = nullptr;
      _dens = nullptr;
      _dens_prev = nullptr;*/
      Obstacles = nullptr;
      y_offset = nullptr;

      start = glm::ivec2(0, 0);
      end = glm::ivec2(0, 0);
    }

    // Destructor
    FluidSolver::~FluidSolver()
    {
      //Do nothing
    }

    void FluidSolver::setLimits(const glm::ivec2& _start, const glm::ivec2& _end)
    {
      start = _start;
      end = _end;
    }

    //Update density grid from a source grid
    void FluidSolver::addSource(Grid2D<float> &x, Grid2D<float> &s, const float dt)
    {
      x.addAndMultiply(start, end, s, dt);
    }

    //Diffuse heat
    void FluidSolver::diffuse(Grid2D<float> &x, Grid2D<float> &x0, float diff, const float dt)
    {
      int i, j, k;
      float a = dt * diff * (x.getSize().x - 2) * (x.getSize().y - 2);

      for (k = 0; k < 1; ++k)
      {
        for (i = start.x + 1; i < end.x - 1; ++i)
        {
          //offset in y direction to ensure not crossing bounds
          for (j = start.y - y_offset[i] + 1; j < end.y - y_offset[i] - 1; ++j)
          {
            //if not in obstacle
            if (j > 0 && Obstacles->Get(i, j) == AIR)
            {
              if (a == 0)
                x.Set(i, j, x0.Get(i, j));
              else
              {
                if (j + y_offset[i] - y_offset[i - 1] > 0 && j + y_offset[i] - y_offset[i + 1] > 0)
                x.Set(i, j, (x0.Get(i, j) +
                  a * (x.Get(i - 1, j + y_offset[i] - y_offset[i - 1]) +
                  x.Get(i + 1, j + y_offset[i] - y_offset[i + 1]) +
                  x.Get(i, j - 1) +
                  x.Get(i, j + 1)))
                  / (1 + 4 * a));
              }
            } //if not in obstacle
          } //for vertical indices
        } //for horizontal indices
      } //for 10 passes
    } //function diffuse

    void FluidSolver::advect(Grid2D<float> &d, Grid2D<float> &d0, Grid2D<float> &u, Grid2D<float> &v, const float dt)
    {
      int i, j, i0, j0, i1, j1;
      float x, y, s0, t0, s1, t1;
      const int N_i = end.x - 1;
      const int N_j = end.y - 1;

      const int dt0_x = dt * (N_i - 1 - start.x);
      const int dt0_y = dt * (N_j - 1 - start.y);

      for (i = start.x + 1; i < N_i; ++i)
      {
        for (j = start.y - y_offset[i] + 1; j < N_j - y_offset[i]; ++j)
        {
          //if not in obstacle
          if (j > 0 && Obstacles->Get(i, j) == AIR)
          {
            x = i - dt0_x * u.Get(i, j);
            y = j - dt0_y * v.Get(i, j);

            if (x < 0.5f) x = 0.5f;
            if (x > N_i + 0.5f) x = N_i + 0.5f;

            i0 = (int)x;
            i1 = i0 + 1;

            if (y < 0.5f) y = 0.5f;
            if (y > N_j + 0.5f) y = N_j + 0.5f;

            j0 = (int)y;
            j0 += y_offset[i];
            j1 = j0 + 1;

            s1 = x - i0;
            s0 = 1 - s1;
            t1 = y - j0;
            t0 = 1 - t1;

            //If none of the resulting cells are obstacles
            if (
                 NOT_OBSTACLE(i0, j0) &&
                 NOT_OBSTACLE(i1, j1) &&
                 NOT_OBSTACLE(i0, j1) &&
                 NOT_OBSTACLE(i1, j0)
              )
            {
              if (j0 - y_offset[i0] > 0 && j1 - y_offset[i0] > 0 && j0 - y_offset[i1] > 0 && j1 - y_offset[i1] > 0)
              {
                  d.Set(i, j,
                    s0 * (t0 * d0.Get(i0, j0 - y_offset[i0]) +
                    t1 * d0.Get(i0, j1 - y_offset[i0])) +
                    s1 * (t0 * d0.Get(i1, j0 - y_offset[i1]) +
                    t1 * d0.Get(i1, j1 - y_offset[i1])));
              }
              else
              {
                d.Set(i, j, d0.Get(i, j));
              }
            } //If none of the resulting cells are obstacles
            //If one or more are obstacles
            else
            {
              d.Set(i, j, d0.Get(i, j));
            }
          } //if not in obstacle
        } //for vertical indices
      } //for horizontal indices
      //setBnd(b, d);
    } //function advect

    //Update density over a time step dt
    void FluidSolver::densStep(Grid2D<float> *x, Grid2D<float> *x0, Grid2D<float> *u, Grid2D<float> *v, float diff, const float dt)
    {
      addSource(*x, *x0, dt);
      SWAP(x0, x);
      diffuse(*x, *x0, diff, dt);
      SWAP(x0, x);
      advect(*x, *x0, *u, *v, dt);
    }

    //Compute velocity field
    void FluidSolver::project(Grid2D<float> &u, Grid2D<float> &v, Grid2D<float> &p, Grid2D<float> &div)
    {
      int i, j, k;
      float h_u;

      h_u = 1.0f / (end.x - 2 - start.x);

      for (i = start.x + 1; i < end.x - 1; ++i)
      {
        for (j = start.y - y_offset[i] + 1; j < end.y - y_offset[i] - 1; ++j)
        {
          if (j > 0 && Obstacles->Get(i, j) == AIR)
          {
            if (j + OFFSET(i + 1) > 0 && j + OFFSET(i - 1) > 0)
            {
              div.Set(i, j,
                -0.5f * h_u * (u.Get(i + 1, j + OFFSET(i + 1)) - u.Get(i - 1, j + OFFSET(i - 1)))
                - 0.5f * h_u * (v.Get(i, j + 1) - v.Get(i, j - 1))
                );
              p.Set(i, j, 0.f);
            }
          } //if not obstacle
        } //for vertical indices
      } //for horizontal indices

      //setBnd(0, div);
      //setBnd(0, p);

      for (k = 0; k < 1; ++k)
      {
        for (i = start.x + 1; i < end.x - 1; ++i)
        {
          for (j = start.y - y_offset[i] + 1; j < end.y - y_offset[i] - 1; ++j)
          {
            if (j > 0 && Obstacles->Get(i, j) == AIR)
            {
              if (j + OFFSET(i - 1) > 0 && j + OFFSET(i + 1) > 0)
              {
                p.Set(i, j,
                  (div.Get(i, j) +
                  p.Get(i - 1, j + OFFSET(i - 1)) +
                  p.Get(i + 1, j + OFFSET(i + 1)) +
                  p.Get(i, j - 1) +
                  p.Get(i, j + 1))
                  / 4);
              }
            } //if not in obstacle
          } //for vertical indices
        } //for horizontal indices
        //setBnd(0, p);
      } //for 10 passes

      for (i = start.x + 1; i < end.x - 1; ++i)
      {
        for (j = start.y - y_offset[i] + 1; j < end.y - y_offset[i] - 1; ++j)
        {
          if (j > 0 && Obstacles->Get(i, j) == AIR && j + OFFSET(i + 1) > 0 && j + OFFSET(i - 1) > 0)
          {
            u.Set(i, j, 
              u.Get(i, j) - 
              0.5f * (
              p.Get(i + 1, j + OFFSET(i + 1)) - 
              p.Get(i - 1, j + OFFSET(i - 1))
              ) / h_u);

            v.Set(i, j,
              v.Get(i, j) -
              0.5f * (
              p.Get(i, j + 1) -
              p.Get(i - 1, j)
              ) / h_u);
          } //if not an obstacle
        } //for vertical indices
      } //for horizontal indices
      //setBnd(1, u);
    } //function project

    void FluidSolver::velStep(Grid2D<float> *u, Grid2D<float> *v, Grid2D<float> *u0, Grid2D<float> *v0, float visc, const float dt)
    {
      addSource(*u, *u0, dt);
      addSource(*v, *v0, dt);
      SWAP(u0, u);
      diffuse(*u, *u0, visc, dt);
      SWAP(v0, v);
      diffuse(*v, *v0, visc, dt);

      project(*u, *v, *u0, *v0);
      SWAP(u0, u);
      SWAP(v0, v);
      advect(*u, *u0, *u0, *v0, dt);
      advect(*v, *v0, *u0, *v0, dt);

      project(*u, *v, *u0, *v0);
    }

  } //namespace Physics
} //namespace Framework 