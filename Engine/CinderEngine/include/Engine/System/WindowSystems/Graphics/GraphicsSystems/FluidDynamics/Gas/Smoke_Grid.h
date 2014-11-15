/******************************************************************************/
/*!
\file   Smoke_Grid.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SMOKE_GRID_H
#define _SMOKE_GRID_H

namespace Framework
{
  class KeyEvent;


  class Smoke_Grid
  {
  public:

  Smoke_Grid ();
  virtual ~Smoke_Grid ();
  
  void Initialize ();
  void Update ();
  void Draw ();

  private:
    float* u, *v, *u_prev, *v_prev;
    float* dens, *dens_prev;

    int Width;
    int Height;

    void draw_density ();
    void draw_velocity ();
    void free_data (void);
    void clear_data (void);
    int allocate_data (void);
    void OnKeyPressed (KeyEvent* key);
    void get_from_UI ();
  };
}

#endif