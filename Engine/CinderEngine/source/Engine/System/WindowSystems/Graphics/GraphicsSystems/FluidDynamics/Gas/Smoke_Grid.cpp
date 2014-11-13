/******************************************************************************/
/*!
\file   Smoke_Grid.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Smoke_Grid.h"
#include "WindowSystem.h"
#include "EventSystem.h"
#include "KeyEvent.h"
#include <stdlib.h>
#include <stdio.h>
#include "solver.c"

namespace Framework
{

#define IX(i,j) ((i)+(Grid_Size+2)*(j))

  static int Grid_Size;
  static float dt, diff, visc;
  static float force, source;
  static int dvel;

  static int win_id;
  static int mouse_down [3];
  static int omx, omy, mx, my;

  Smoke_Grid::Smoke_Grid ()
  {}

  Smoke_Grid::~Smoke_Grid ()
  {
    free_data ();
  }


  void Smoke_Grid::free_data ()
  {
    if (u)
      delete [] (u);
    if (v)
      delete [] (v);
    if (u_prev)
      delete [] (u_prev);
    if (v_prev)
      delete [] (v_prev);
    if (dens)
      delete [] (dens);
    if (dens_prev)
      delete [] (dens_prev);
  }


  static void mouse_func (GLFWwindow* window, int button, int state, int mods)
  {
    mouse_down [button] = state == GLFW_PRESS;
  }

  static void motion_func (GLFWwindow* window, double x, double y)
  {
    omx = mx = (int)x;
    omx = my = (int)y;
  }


  void Smoke_Grid::Initialize ()
  {
    Grid_Size = 128;
    dt = 0.1f;
    diff = 0.0f;
    visc = 0.0f;
    force = 1.0f;
    source = 100.0f;

    if (!allocate_data ())
      throw("Failed to make grids");

    clear_data ();

    EVENTSYSTEM->mConnect<KeyEvent, Smoke_Grid> (Events::KEY_ANY, this, &Smoke_Grid::OnKeyPressed);
  }

  void Smoke_Grid::clear_data ()
  {
    int i, size = (Grid_Size + 2)*(Grid_Size + 2);

    for (i = 0; i < size; ++i)
    {
      u [i] = v [i] = u_prev [i] = v_prev [i] = dens [i] = dens_prev [i] = 0.0f;
    }
  }

  INT32 Smoke_Grid::allocate_data ()
  {
    int size = (Grid_Size + 2)*(Grid_Size + 2);

    u =         new float [size];
    v =         new float [size];
    u_prev =    new float [size];
    v_prev =    new float [size];
    dens =      new float [size];
    dens_prev = new float [size];

    if (!u || !v || !u_prev || !v_prev || !dens || !dens_prev)
    {
      fprintf (stderr, "cannot allocate data\n");
      return (0);
    }

    return (1);
  }


  static void pre_display ()
  {
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0.0, 1.0, 0.0, 1.0, 0, 100.0f);
  }

  void Smoke_Grid::draw_velocity (void)
  {
    int i, j;
    float x, y, h;

    h = 1.0f / Grid_Size;

    glColor3f (1.0f, 1.0f, 1.0f);
    glLineWidth (1.0f);

    glBegin (GL_LINES);

    for (i = 1; i <= Grid_Size; i++) {
      x = (i - 0.5f)*h;
      for (j = 1; j <= Grid_Size; j++) {
        y = (j - 0.5f)*h;

        glVertex2f (x, y);
        glVertex2f (x + u [IX (i, j)], y + v [IX (i, j)]);
      }
    }

    glEnd ();
  }

  void Smoke_Grid::draw_density (void)
  {
    int i, j;
    float x, y, h, d00, d01, d10, d11;

    h = 1.0f / Grid_Size;

    glBegin (GL_QUADS);

    for (i = 0; i <= Grid_Size; i++)
    {
      x = (i - 0.5f)*h;
      for (j = 0; j <= Grid_Size; j++)
      {
        y = (j - 0.5f)*h;

        d00 = dens [IX (i, j)];
        d01 = dens [IX (i, j + 1)];
        d10 = dens [IX (i + 1, j)];
        d11 = dens [IX (i + 1, j + 1)];

        glColor3f (d00, d00, d00);
        glVertex2f (x, y);
        glColor3f (d10, d10, d10);
        glVertex2f (x + h, y);
        glColor3f (d11, d11, d11);
        glVertex2f (x + h, y + h);
        glColor3f (d01, d01, d01);
        glVertex2f (x, y + h);
      }
    }

    glEnd ();
  }


  static void get_from_UI (float * d, float * u, float * v)
  {
    double x, y;
    if (glfwGetMouseButton (WINDOWSYSTEM->Get_Window (), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
      mouse_down [0] = 1;
      glfwGetCursorPos (WINDOWSYSTEM->Get_Window (), &x, &y);
      omx = mx = (int) x;
      omx = my = (int) y;
    }
    else
    {
      mouse_down [0] = 0;
    }

    if (glfwGetMouseButton (WINDOWSYSTEM->Get_Window (), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
      mouse_down [1] = 1;
      glfwGetCursorPos (WINDOWSYSTEM->Get_Window (), &x, &y);
      omx = mx = (int) x;
      omx = my = (int) y;
    }
    else
    {
      mouse_down [1] = 0;
    }
    int i, j, size = (Grid_Size + 2)*(Grid_Size + 2);

    for (i = 0; i < size; i++) {
      u [i] = v [i] = d [i] = 0.0f;
    }

    if (!mouse_down [0] && !mouse_down [1]) return;

    i = (int) ((mx / (float) WINDOWSYSTEM->Get_Width())*Grid_Size + 1);
    j = (int) (((WINDOWSYSTEM->Get_Height() - my) / (float) WINDOWSYSTEM->Get_Height())*Grid_Size + 1);

    if (i<1 || i>Grid_Size || j<1 || j>Grid_Size) return;

    if (mouse_down [0])
    {
      u [IX (i, j)] = force * (mx - omx);
      v [IX (i, j)] = force * (omy - my);
    }

    if (mouse_down [1])
    {
      d [IX (i, j)] = source;
    }

    omx = mx;
    omy = my;

    return;
  }

  void Smoke_Grid::OnKeyPressed (KeyEvent* key)
  {
    if (key->KeyDown)
    switch (key->KeyValue)
    {
    case GLFW_KEY_C:
      clear_data ();
      break;

    case GLFW_KEY_V:
      dvel = !dvel;
      break;
    }
  }


  void Smoke_Grid::Update (void)
  {
    get_from_UI (dens_prev, u_prev, v_prev);
    vel_step (Grid_Size, u, v, u_prev, v_prev, visc, dt);
    dens_step (Grid_Size, dens, dens_prev, u, v, diff, dt);
  }

  void Smoke_Grid::Draw (void)
  {
    pre_display ();

    if (dvel)
      draw_velocity ();
    else
      draw_density ();
  }
}