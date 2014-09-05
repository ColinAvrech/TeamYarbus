/******************************************************************************/
/*!
\file   Graphics.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

/*------------------------------------------------------------------------------
// Includes
------------------------------------------------------------------------------*/
#pragma region Includes
#include "GraphicsSystem.h"

#pragma endregion

namespace Framework 
{
/*------------------------------------------------------------------------------
// Class Implementation
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
// Constructors
------------------------------------------------------------------------------*/
#pragma region Constructors

  GraphicsSystem :: GraphicsSystem ()
  {
    GRAPHICSSYSTEM = this;
  }


#pragma endregion

/*------------------------------------------------------------------------------
// Public Variables
------------------------------------------------------------------------------*/
#pragma region Public Variables

  GraphicsSystem* GRAPHICSSYSTEM = NULL;
  float time_last;
  float rate = 1;
  Affine map;

  // THIS IS HOW A TYPICAL GAMEOBJECT WILL LOOK AFTER COMPONENT BASED DESIGN
  // IS FULLY IMPLEMENTED
  Transform transform;      // HOLDS ALL TRANSFORMATION DATA
  SpriteRenderer renderer;  // RENDERS THE SPRITE OBJECT
  //////////////////////////////////////////////////////////////////////////
  SpriteRenderer renderer1;
  Point verts [] = { Point (3, 2), Point (-4, 5), Point (-6, -7), Point (5, -8) };
  SpriteRenderer renderer2 (verts);


#pragma endregion

/*----------------------------------------------------------------------------*/
// Public Structs
/*----------------------------------------------------------------------------*/
#pragma region Public Structs

#pragma endregion

/*------------------------------------------------------------------------------
// Public Functions
------------------------------------------------------------------------------*/
#pragma region Public Functions

  bool GraphicsSystem::Initialize ()
  {
    std::cout << GraphicsSystem::GetName () + " initialized";
    int argc = 1;
    char *argv [1] = { (char*)"Something" };
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition (0, 0);
    glutInitWindowSize (1024, 768);
    glutCreateWindow ("1666 Best Game Ever");
    glutDisplayFunc (Draw);
    glutKeyboardFunc (KeyPressed);
    glutReshapeFunc (Resized);
    glutIdleFunc (Loop);
    Init ();
    glutMainLoop ();
    return true;
  }


  void GraphicsSystem::Update (const double dt)
  {
  }


  void Init (void)
  {
    srand (unsigned (time (0)));
    time_last = glutGet (GLUT_ELAPSED_TIME);
    map = Scale (0.5f);
  }


  void Draw (void)
  {
    int t = glutGet (GLUT_ELAPSED_TIME);
    double dt = 0.001*(t - time_last);
    time_last = t;

    rate += dt;

    glClearColor (0, 0, 0, 0);
    glClear (GL_COLOR_BUFFER_BIT);

    //for (int i = 0; i < 4; ++i)
    //{
    //  renderer.temp [i] = Rot (-rate) * map * renderer.sprite.vertices [i];
    //}

    //for (int i = 0; i < 4; ++i)
    //{
    //  renderer1.temp [i] =  Rot (rate) * map * renderer1.sprite.vertices [i];
    //}

    for (int i = 0; i < 4; ++i)
    {
      renderer2.temp [i] = map * Scale (0.05f) * renderer2.sprite.vertices [i];
    }

    // DEFAULT PARAMETER IS ZERO
    // THIS PARAMETER WILL BE USED TO CHOOSE WHICH MATERIAL TO USE
    renderer.Render ();
    renderer1.Render ();
    renderer2.Render ();
    glutSwapBuffers ();

  }


  void KeyPressed (unsigned char c, int, int)
  {
   /* int t = glutGet (GLUT_ELAPSED_TIME);
    double dt = 0.001*(t - time_last);

    switch (c)
    {
    case '\x1b':
      exit (0);
      break;
    case ' ':
      use_camera = !use_camera;
      break;
    case 'd':
      camPosition.x += dt;
      break;
    case 'a':
      camPosition.x -= dt;
      break;
    case 'w':
      camPosition.y += dt;
      break;
    case 's':
      camPosition.y -= dt;
      break;
    case '1':
      camZoom += dt;
      break;
    case '2':
      if (camZoom > 0)
        camZoom -= dt;
      break;
    case 'x':
      camRotation += 2 / pi * dt;
      break;
    case 'z':
      camZoom = 1.0f;
      camRotation = 0.0f;
      camPosition = Point (0, 0);
      break;
    }*/
  }


  void Resized (int W, int H)
  {
    glViewport (0, 0, W, H);
    //camera_width = 2.0f * float (W) / float (H);
    //movingCamWidth = 0.5f * float (W) / float (H);
  }


  void Loop (void)
  {
    glutPostRedisplay ();
  }


#pragma endregion


/*------------------------------------------------------------------------------
// Static Public Variables
------------------------------------------------------------------------------*/
#pragma region Static Public Variables

#pragma endregion


/*------------------------------------------------------------------------------
// Static Public Functions
------------------------------------------------------------------------------*/
#pragma region Static Public Functions

#pragma endregion


/*------------------------------------------------------------------------------
// Destructor
------------------------------------------------------------------------------*/
#pragma region Destructor

  GraphicsSystem :: ~GraphicsSystem ()
  {
  }

#pragma endregion


/*------------------------------------------------------------------------------
// Private Variables
------------------------------------------------------------------------------*/
#pragma region Private Variables

#pragma endregion


/*------------------------------------------------------------------------------
// Private Structs
------------------------------------------------------------------------------*/
#pragma region Private Structs

#pragma endregion

/*------------------------------------------------------------------------------
// Private Functions
------------------------------------------------------------------------------*/
#pragma region Private Functions

#pragma endregion


/*------------------------------------------------------------------------------
// Static Functions
------------------------------------------------------------------------------*/
#pragma region Static Functions

#pragma endregion

}
//------------------------------------------------------------------------------
