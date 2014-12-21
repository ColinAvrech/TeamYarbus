/******************************************************************************/
/*!
\file   Fluid_Engine.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <cstdlib>
#include "EventSystem.h"
#include "BaseEvent.h"
#include "KeyEvent.h"
#include "WindowSystem.h"
#include <iostream>
#include <sstream>
#include <string>
#include "Fluid_Neighbour_Particle.h"
#include "Fluid_Physics_Particle.h"
#include "Fluid_Physics_Level_Constants.h"
#include "Fluid_Physics_Engine.h"
#include "Marching_Squares.h"
#include "Cinder_Engine_Color.h"
#include "Cinder_Engine_Delta_Time.h"
#include "Fluid_Engine.h"
#include "Camera.h"
#include "Events.h"

namespace Framework
{
  const float LOCAL_number_of_pixels_width = 32.0f;
  const float LOCAL_number_of_pixels_height = 32.0f;

  std::vector <float> mesh;
  INT32 LOCAL_width;
  INT32 LOCAL_height;
  BOOL attracting = BOOLEAN_false;
  BOOL repelling = BOOLEAN_false;
  BOOL Viscoelasticity = BOOLEAN_false;
  BOOL Plasticity = BOOLEAN_true;
  BOOL Viscosity = BOOLEAN_false;
  BOOL PauseSimulation = BOOLEAN_false;
  INT32 GraphicalMode = 1;
  Fluid_Physics_Engine FluidPhysics;
  Fluid_Marching_Squares GraphicsMarchingSquares;
  enum RenderModes{ LargeParticleMode = 1, SmallParticleMode = 2, MarchingSquareMode = 3 };

  glm::mat4 vp;
  glm::mat4 wv;

  void Fluid_Engine::Render ()
  {
    INDEX temp_index_1, temp_index_2, start_position, end_position;

    if (GraphicalMode == LargeParticleMode)
    {
      glPointSize (
        FLUID_PHYSICS_PART_SIZE
        *FLUID_PHYSICS_PART_SIZE
        );
    }
    else if (GraphicalMode == SmallParticleMode)
    {
      glPointSize (FLUID_PHYSICS_PART_SIZE);
    }

    switch (GraphicalMode)
    {
    case 1:
    case 2:
      temp_index_2 = 0;
      glBegin (GL_POINTS);
      CINDER_LOOP_THROUGH_TABLE (
        temp_index_2,
        FluidPhysics.ParticleTable
        )
      {
        vec4 pos = vp * vec4 (FluidPhysics.ParticleTable [temp_index_2].GetPosition ().X,
          FluidPhysics.ParticleTable [temp_index_2].GetPosition ().Y, 0, 1);
        glColor4fv (Graphics_Color::Red ().GetRGBA ());
        glVertex4fv (glm::value_ptr(pos));
      }
      glEnd ();
      break;

    case MarchingSquareMode:
      temp_index_2 = 0;
      start_position = 0;
      end_position = 0;
      const Cinder_Index_Array& table = GraphicsMarchingSquares.GetPolygonVertexCountTable ();
      const MATH_POINT_2D_ARRAY& pointTable = GraphicsMarchingSquares.GetPointTable ();
      CINDER_LOOP_THROUGH_TABLE (
        temp_index_2,
        table
        )
      {
        end_position
          = start_position
          + table [temp_index_2] - 1;
        glBegin (GL_POLYGON);
        CINDER_LOOP_THROUGH_INDEX (
          temp_index_1,
          start_position,
          end_position
          )
        {
          vec2 pos = glm::mat2 (vp) * vec2 (pointTable [temp_index_1].X, pointTable [temp_index_1].Y);
          glColor4f (1, 1, 1, 1);
          glVertex2fv (glm::value_ptr (pos));
        }
        glEnd ();
        start_position += table [temp_index_2];
      }
      GraphicsMarchingSquares.Reset ();
      break;
    }

  }

  void Fluid_Engine::Update ()
  {
    INT32 index_1, index_2;
    INDEX particle_index;
    REAL32 delta_time;

    delta_time = 0.016f;

    if (!PauseSimulation)
    {

      FluidPhysics.Update (
        delta_time,
        Viscosity,
        Viscoelasticity,
        Plasticity
        );
    }

    if (GraphicalMode == MarchingSquareMode)
    {
      particle_index = 0;

      CINDER_LOOP_THROUGH_TABLE (
        particle_index,
        FluidPhysics.ParticleTable
        )
      {
        if (FluidPhysics.ParticleTable [particle_index].GetPosition ().X < 0.0f)
        {
          index_1 = INT32 (FluidPhysics.ParticleTable [particle_index].GetPosition ().X)
            - 1
            + (int)LOCAL_number_of_pixels_width;
        }
        else
        {
          index_1 = INT32 (FluidPhysics.ParticleTable [particle_index].GetPosition ().X)
            + (int)LOCAL_number_of_pixels_width;
        }

        if (FluidPhysics.ParticleTable [particle_index].GetPosition ().Y < 0.0f)
        {
          index_2 = INT32 (FluidPhysics.ParticleTable [particle_index].GetPosition ().Y)
            - 1
            - (int)LOCAL_number_of_pixels_height;
          index_2 *= -1;
        }
        else
        {
          index_2 = INT32 (FluidPhysics.ParticleTable [particle_index].GetPosition ().Y)
            - (int)LOCAL_number_of_pixels_height;
          index_2 *= -1;
        }

        GraphicsMarchingSquares.CalculatePoints (
          FluidPhysics.ParticleTable [particle_index].GetPosition (),
          INDEX (index_1),
          INDEX (index_2),
          4
          );
      }
      GraphicsMarchingSquares.GeneratePoints ();
    }
  }


  void OnKeyPressed (KeyEvent* key)
  {
    if (key->KeyDown)
    {
      switch (key->KeyValue)
      {
      case GLFW_KEY_E:
        if (!Plasticity)
        {
          FluidPhysics.ResetViscoelasticity ();
          Viscoelasticity = !Viscoelasticity;
        }
        break;

      case GLFW_KEY_P:
        if (!Viscoelasticity)
        {
          Plasticity = !Plasticity;
          if (Plasticity)
          {
            FluidPhysics.InitializePlasticity ();
          }
        }
        break;

      case GLFW_KEY_M:
        GraphicalMode = (GraphicalMode + 1 > 3) ? 1 : GraphicalMode + 1;
        break;

      case GLFW_KEY_SPACE:
        PauseSimulation = !PauseSimulation;
        break;
      }
    }
  }

  // ~~

  void motion (GLFWwindow* window, double x, double y)
  {
    LOCAL_width = WINDOWSYSTEM->Get_Width ();
    LOCAL_height = WINDOWSYSTEM->Get_Height ();
    REAL32 relx, rely;
    MATH_VECTOR_2D mouse;

    relx = (REAL32) (x - LOCAL_width / 2) / LOCAL_width;
    rely = -(REAL32) (y - LOCAL_height / 2) / LOCAL_height;
    mouse = MATH_VECTOR_2D (
      relx*LOCAL_number_of_pixels_width * 2,
      rely*LOCAL_number_of_pixels_height * 2);
    {
      FluidPhysics.SetAttractor (mouse);
    }
  }

  void mouse (GLFWwindow* window, INT32 button, INT32 state, INT32 mods)
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
      if (state == GLFW_PRESS)
      {
        FluidPhysics.SetMouse (BOOLEAN_true, BOOLEAN_false);
      }
      else
      {
        {
          FluidPhysics.SetMouse (BOOLEAN_false, BOOLEAN_false);
          FluidPhysics.SetAttractor (
            MATH_VECTOR_2D (
            REAL32 (LOCAL_number_of_pixels_width) * 99.0f,
            REAL32 (LOCAL_number_of_pixels_height) * 99.0f
            )
            );
        }
      }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
      if (state == GLFW_PRESS)
      {
        {
          FluidPhysics.SetMouse (BOOLEAN_false, BOOLEAN_true);
        }
      }
      else
      {
        {
          FluidPhysics.SetMouse (BOOLEAN_false, BOOLEAN_false);
          FluidPhysics.SetAttractor (
            MATH_VECTOR_2D (
            REAL32 (LOCAL_number_of_pixels_width) * 99.0f,
            REAL32 (LOCAL_number_of_pixels_height) * 99.0f
            )
            );
        }
      }
    }
  }


  void Fluid_Engine::Initialize ()
  {
    vp = glm::ortho
      (
        -LOCAL_number_of_pixels_width,
        LOCAL_number_of_pixels_width,
        -LOCAL_number_of_pixels_height,
        LOCAL_number_of_pixels_height,
        0.0f,
        1000.0f
      );

    glEnable (GL_POINT_SMOOTH);

    FluidPhysics.Initialize (
      LOCAL_number_of_pixels_width,
      LOCAL_number_of_pixels_height);

    EVENTSYSTEM->gConnect (Events::KEY_ANY, &OnKeyPressed);
    //glfwSetMouseButtonCallback (WINDOWSYSTEM->Get_Window (), &mouse);
    //glfwSetCursorPosCallback (WINDOWSYSTEM->Get_Window (), &motion);

    if (Plasticity)
    {
      FluidPhysics.InitializePlasticity ();
    }

    GraphicsMarchingSquares.Initialize (
      LOCAL_number_of_pixels_width,
      LOCAL_number_of_pixels_height
      );
  }
}