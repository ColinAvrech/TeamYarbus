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

  // TRANSFORMATIONS
  Affine ObjectToWorld;
  Affine WorldToCamera;
  Affine NDC;
  Affine WorldMatrix = Scale (1 / 12.0f, 1 / 2.0f);
  Affine camera;
  Affine staticCameraTransform = Scale (1.0f);
  Affine Square = Scale (0.4f);
  Affine movingCameraTransform = Scale (1);

  // MESH DATA
  // TEMPORARY
  // EACH GAME OBJECT WITH A RENDERER COMPONENT WILL CARRY IT'S OWN DATA

  const float pi = 4.0f*atan (1.0f);
  int time_last;

  // basic square
  const int vertex_count = 4;
  const Point vertices [vertex_count]
    = { Point (1, 1), Point (-1, 1), Point (-1, -1), Point (1, -1) };
  // Rotating Square
  float rate = 2 * pi / 5.0f;
  Point RotatingSquare (0.0f, 0.0f);
  const int face_count = 2;
  const struct { int index1, index2, index3; }
  faces [face_count] = { { 0, 1, 2 }, { 0, 2, 3 } };
  const int edge_count = 4;
  const struct { int index1, index2; }
  edges [edge_count] = { { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 } };

  Point O, R;

  // CAMERA
  // TEMPORARY
  // WILL BE REPLACED BY CAMERA COMPONENT
  float camera_width = 2.0f * float (width) / float (height),
    camera_height = 2.0f;
  Point camPosition;
  float camZoom = 1.0f, camRotation, movingCamWidth, movingCamHeight = 0.5f;
  bool use_camera;


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
  }


  void Draw (void)
  {
    // compute the time elapsed since the last call to 'Draw' (in seconds)
    int t = glutGet (GLUT_ELAPSED_TIME);
    double dt = 0.001*(t - time_last);
    time_last = t;
    rate += dt;
    // clear the screen
    glClearColor (1, 1, 1, 0);
    glClear (GL_COLOR_BUFFER_BIT);

    Point temp_points [4];
    Affine N = Scale (2.0f / camera_width, 2.0f / camera_height);
    if (use_camera)
    {
      camera = Inverse (movingCameraTransform);
    }
    else
    {
      camera = Inverse (staticCameraTransform);
    }
#pragma region DrawMesh
    float k = 0;
    for (int i = 0; i < 50; ++i)
    {
      int j = rand () % 25;
      ObjectToWorld = Trans (Vector ((-camera_width / 2) + k, 0.5f)) * Scale ((camera_width) / 50.0f, 0.5f);
      WorldToCamera = ObjectToWorld;
      NDC = camera * N * WorldToCamera;
      //k += 0.04f;
      for (int i = 0; i < vertex_count; ++i)
      {
        temp_points [i] = NDC * vertices [i];
      }
      k += camera_width / 50.0f;
      glColor3f (0.2f / j, 0.5f / j, 1.0f / j);

      for (int k = 0; k < face_count; ++k)
      {
        glBegin (GL_POLYGON);
        glVertex2f (temp_points [faces [k].index1].x, temp_points [faces [k].index1].y);
        glVertex2f (temp_points [faces [k].index2].x, temp_points [faces [k].index2].y);
        glVertex2f (temp_points [faces [k].index3].x, temp_points [faces [k].index3].y);
        glEnd ();
      }

      glColor3d (0, 0, 0);

      glBegin (GL_LINES);
      for (int k = 0; k < edge_count; ++k)
      {
        glVertex2f (temp_points [edges [k].index1].x, temp_points [edges [k].index1].y);
        glVertex2f (temp_points [edges [k].index2].x, temp_points [edges [k].index2].y);
      }
      glEnd ();
    }

    k = 0;
    for (int i = 0; i < 50; ++i)
    {
      int j = rand () % 25;
      ObjectToWorld = Trans (Vector ((-camera_width / 2) + k, -0.5f)) * Scale ((camera_width) / 50.0f, 0.5f);
      WorldToCamera = ObjectToWorld;
      NDC = camera * N * WorldToCamera;
      //k += 0.04f;
      for (int i = 0; i < vertex_count; ++i)
      {
        temp_points [i] = NDC * vertices [i];
      }
      k += camera_width / 50.0f;
      glColor3f (1.0f / j, 0.5f / j, 0.2f / j);

      for (int k = 0; k < face_count; ++k)
      {
        glBegin (GL_POLYGON);
        glVertex2f (temp_points [faces [k].index1].x, temp_points [faces [k].index1].y);
        glVertex2f (temp_points [faces [k].index2].x, temp_points [faces [k].index2].y);
        glVertex2f (temp_points [faces [k].index3].x, temp_points [faces [k].index3].y);
        glEnd ();
      }

      glColor3d (0, 0, 0);

      glBegin (GL_LINES);
      for (int k = 0; k < edge_count; ++k)
      {
        glVertex2f (temp_points [edges [k].index1].x, temp_points [edges [k].index1].y);
        glVertex2f (temp_points [edges [k].index2].x, temp_points [edges [k].index2].y);
      }
      glEnd ();
    }

#pragma  endregion

#pragma endregion
////////////////////////////////////////////////////////////////////////////////
#pragma region RotatingSquare

    WorldToCamera = Trans (R - O) * Rot (rate) * Trans (O - R) * Scale (0.4f);
    NDC = camera * N * WorldToCamera;

    for (int i = 0; i < vertex_count; ++i)
    {
      temp_points [i] = NDC * vertices [i];
    }

#pragma region DrawMesh

    glColor3f (0, 0, 1);

    for (int k = 0; k < face_count; ++k)
    {
      glBegin (GL_POLYGON);
      glVertex2f (temp_points [faces [k].index1].x, temp_points [faces [k].index1].y);
      glVertex2f (temp_points [faces [k].index2].x, temp_points [faces [k].index2].y);
      glVertex2f (temp_points [faces [k].index3].x, temp_points [faces [k].index3].y);
      glEnd ();
    }

    glColor3d (0, 0, 0);

    glBegin (GL_LINES);
    for (int k = 0; k < edge_count; ++k)
    {
      glVertex2f (temp_points [edges [k].index1].x, temp_points [edges [k].index1].y);
      glVertex2f (temp_points [edges [k].index2].x, temp_points [edges [k].index2].y);
    }
    glEnd ();

#pragma  endregion
////////////////////////////////////////////////////////////////////////////////
#pragma  endregion
////////////////////////////////////////////////////////////////////////////////
#pragma region Camera

    ObjectToWorld = movingCameraTransform = Trans (Vector (camPosition.x, camPosition.y)) * Rot (camRotation) * Scale (movingCamWidth / camZoom, movingCamHeight / camZoom);
    movingCameraTransform = N * movingCameraTransform;
    WorldToCamera = ObjectToWorld * camera;
    NDC = N * WorldToCamera;

    for (int i = 0; i < vertex_count; ++i)
    {
      temp_points [i] = NDC * vertices [i];
    }

#pragma region DrawMesh


    glColor3d (1, 0, 0);

    glBegin (GL_LINES);
    for (int k = 0; k < edge_count; ++k)
    {
      glVertex2f (temp_points [edges [k].index1].x, temp_points [edges [k].index1].y);
      glVertex2f (temp_points [edges [k].index2].x, temp_points [edges [k].index2].y);
    }
    glEnd ();

#pragma  endregion

#pragma endregion

    glutSwapBuffers ();
  }


  void KeyPressed (unsigned char c, int, int)
  {
    int t = glutGet (GLUT_ELAPSED_TIME);
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
    }
  }


  void Resized (int W, int H)
  {
    glViewport (0, 0, W, H);
    camera_width = 2.0f * float (W) / float (H);
    movingCamWidth = 0.5f * float (W) / float (H);
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
