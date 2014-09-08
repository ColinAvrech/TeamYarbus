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
#include "WindowSystem.h"

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

  GraphicsSystem::GraphicsSystem()
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
    // Get Device Context for the window
    hDC = GetDC(WINDOWSYSTEM->hWnd);
    // Set Pixel Format for the window
    SetupPixelFormatWindows(hDC);
    // Get Rendering Context
    hRC = wglCreateContext(hDC);
    // Set the rendering context to the Device context
    wglMakeCurrent(hDC, hRC);
    // enables Depth Testing
    glEnable(GL_DEPTH_TEST);
    // Set OpenGL's Screen Size
    SetOpenGLScreenSize(WINDOWSYSTEM->WindowWidth, WINDOWSYSTEM->WindowHeight);
    return true;
  }

  /*!
  This function sets the Window's Pixelformat
  */
  void GraphicsSystem::SetupPixelFormatWindows(HDC hDC)
  {
    PIXELFORMATDESCRIPTOR PFD;
    int idpixelformat;

    PFD.nSize = sizeof(PIXELFORMATDESCRIPTOR);    // Specifies the size of this data structure. 
    PFD.nVersion = 1;                             // Specifies the version of this data structure. This value should be set to 1.

    // Set properties of PixelFormatDescriptor

    PFD.dwFlags = PFD_DRAW_TO_WINDOW || PFD_SUPPORT_OPENGL || PFD_DOUBLEBUFFER; // A set of bit flags that specify properties of the pixel buffer. 
    PFD.dwLayerMask = PFD_MAIN_PLANE;         // default plane
    PFD.iPixelType = PFD_TYPE_RGBA;           // Specifies the type of pixel data.
    PFD.cColorBits = CINDER_SCREEN_DEPTH;     // Specifies the number of color bitplanes in each color buffer.
    PFD.cDepthBits = CINDER_SCREEN_DEPTH;     // Specifies the depth of the depth (z-axis) buffer.
    PFD.cAccumBits = 0;                       // Specifies the total number of bitplanes in the accumulation buffer.
    PFD.cStencilBits = 0;                     // Specifies the depth of the stencil buffer.
    
    // Choose a pixelformat based on Descriptor
    idpixelformat = ChoosePixelFormat(hDC, &PFD);
    ErrorIf(idpixelformat == false, "Failed to ChoosePixelFormat");

    // 
    bool SPF = SetPixelFormat(hDC, idpixelformat, &PFD);
    ErrorIf(SPF == false, "Failed to SetPixelFormat");
  }


  // Place Holder for XLIB's functions
  bool GraphicsSystem::SetupPixelFormatXWindows(HDC hDC)
  {
<<<<<<< HEAD
    srand (unsigned (time (0)));
    time_last = glutGet (GLUT_ELAPSED_TIME);
    map = Scale (0.5f);
=======
    return false;
>>>>>>> origin/Micah
  }

  void GraphicsSystem::SetOpenGLScreenSize(unsigned width, unsigned height)
  {
    // if zero, it's 1, cause math
    if (height == 0){height = 1;}
    if (width == 0){ width = 1; }

    /*
    x, y
      Specify the lower left corner of the viewport rectangle, in pixels. The initial value is (0,0).

    width, height
      Specify the width and height of the viewport. When a GL context is first attached to a window, width and height are set to the dimensions of that window.
    */
    glViewport(0, 0, width, height); // Make the viewport the entire screen

    glMatrixMode(GL_PROJECTION);      // Set the rendering mode to Projection
    glLoadIdentity();                 // Reset the Projection matrix

    /*
    fovy
    Specifies the field of view angle, in degrees, in the y direction.

    aspect
    Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).

    zNear
    Specifies the distance from the viewer to the near clipping plane (always positive).

    zFar
    Specifies the distance from the viewer to the far clipping plane (always positive).
    */
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.5, 100.f); // 45 Degree fov, calculated Aspect ratio, MinDrawDist of 0.5, MaxDrawDist of 100

    glMatrixMode(GL_MODELVIEW);							// Set the rendering mode to Modelview
    glLoadIdentity();									      // Reset The Modelview Matrix
  }


  void GraphicsSystem::Update (const double dt)
  {
<<<<<<< HEAD
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
=======
    DrawDebug(dt);
    DrawSprites(dt);
    DrawParticles(dt);
  }

  void GraphicsSystem::DrawDebug(const double dt)
  {
    /* All colliders or objects wanting to have debug draw information
    need to add themself to the Debug DrawArray */



    /*
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

      glBegin(GL_POLYGON);
      for (int k = 0; k < face_count; ++k)
      {
        glVertex2f (temp_points [faces [k].index1].x, temp_points [faces [k].index1].y);
        glVertex2f (temp_points [faces [k].index2].x, temp_points [faces [k].index2].y);
        glVertex2f (temp_points [faces [k].index3].x, temp_points [faces [k].index3].y);
      }
      glEnd();

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
      glEnd ();*/
    }

  void GraphicsSystem::DrawSprites(const double dt)
  {

>>>>>>> origin/Micah

  }

  void GraphicsSystem::DrawParticles(const double dt)
  {
<<<<<<< HEAD
    glViewport (0, 0, W, H);
    //camera_width = 2.0f * float (W) / float (H);
    //movingCamWidth = 0.5f * float (W) / float (H);
  }
=======
>>>>>>> origin/Micah


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

  GraphicsSystem::~GraphicsSystem ()
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
