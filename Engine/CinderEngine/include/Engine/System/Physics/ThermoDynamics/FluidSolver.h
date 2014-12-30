/******************************************************************************/
/*!
\file   FluidSolver.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _FLUID_SOLVER_H
#define _FLUID_SOLVER_H

namespace Framework
{
  namespace Physics
  {
    //Forward declaration
    template<class T>
    class Grid2D;

    class FluidSolver
    {
    public:
      FluidSolver();
      ~FluidSolver();
      //Never updates the entire grid, only 128 x 32
      void setLimits(const glm::ivec2 &start, const glm::ivec2 &end);
      void velStep(Grid2D<float> *u, Grid2D<float> *v, Grid2D<float> *u0, Grid2D<float> *v0, float visc, const float dt);
      void densStep(Grid2D<float> *x, Grid2D<float> *x0, Grid2D<float> *u, Grid2D<float> *v, float diff, const float dt);

      //pointers are initialized in thermo::Initialize function
      //Grid2D<float> *_u, *_v, *_u_prev, *_v_prev;
      //Grid2D<float> *_dens, *_dens_prev; //actually temperature
      Grid2D<Material> *Obstacles;
      int *y_offset;

    private:
      glm::ivec2 start;
      glm::ivec2 end;

      //Helper functions
      void addSource(Grid2D<float> &x, Grid2D<float> &s, const float dt);
      void diffuse(Grid2D<float> &x, Grid2D<float> &x0, float diff, const float dt);
      void advect(Grid2D<float> &d, Grid2D<float> &d0, Grid2D<float> &u, Grid2D<float> &v, const float dt);
      void project(Grid2D<float> &u, Grid2D<float> &v, Grid2D<float> &p, Grid2D<float> &div);
    };
  }
}

#endif